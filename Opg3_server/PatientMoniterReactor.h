#pragma once
#include "../winsock2_wrapper/iReactor.h"
#include "../winsock2_wrapper/SOCK_Acceptor.h"

#include <string>
#include <list>


enum Event_Type{
	INVALID_EVENT1,
	INVALID_EVENT2,
	EVENT_ALARM,
	EVENT_PATIENT_VALUE,
	EVENT_LOG
};

class PatientMoniterReactor
	:
	public iReactor
{
public:

	PatientMoniterReactor(const u_short port){
		SOCK_Stream *listenSock = acceptor.initialize(port);
		listenSock->setNonBlocking();
		readSockets.push_back(listenSock);
	}

	~PatientMoniterReactor(){

	}

	void removeHandler(iEventHandler *eventHandler, Event_Type eventType){
			switch (eventType){
			case EVENT_ALARM:
				alarmEventCbs.remove(eventHandler);
				break;
			case EVENT_PATIENT_VALUE:
				patientValueEventCbs.remove(eventHandler);
				break;
			case EVENT_LOG:
				logEventCbs.remove(eventHandler);
				break;
			}

	}

	void registerHandler(iEventHandler *eventHandler, Event_Type eventType){
			switch (eventType){
			case EVENT_ALARM:
				alarmEventCbs.push_back(eventHandler);
				break;
			case EVENT_PATIENT_VALUE:
				patientValueEventCbs.push_back(eventHandler);
				break;
			case EVENT_LOG:
				logEventCbs.push_back(eventHandler);
				break;
			}
	}

	void handleEvents(int timeout = 0){
		bool running = true;

		while (running){
			running = internalHandleEvent(timeout);
		}

	}


private:

	bool internalHandleEvent(int timeout){
		const std::string conAccepted = "connection accepted from: ";
		SOCK_Stream::select(readSockets, writeSockets, errorSockets);

		SOCK_Stream *client = readSockets.front()->accept();

		if (client != NULL){
			client->setNonBlocking();
			readSockets.push_back(client);
			//callEvents(EVENT_LOG, "Recived connection from: " +  std::string(client->getAddr().getIpAddr()));
		}
		else if (readSockets.size() > 1){
			std::string rtnValue = readCloseSockets();
			if (rtnValue.empty()){
				//callEvents(EVENT_LOG, "Closed a socket");
			}
			else{
				std::list<std::string> packages = unwrapPacakges(rtnValue);
				std::list<std::string>::iterator iter;
				for (iter = packages.begin(); iter != packages.end(); ++iter){
					Event_Type type = decodeEventType(*iter);

					callEvents(type, (*iter).substr(1, (*iter).length() - 2));

				}
			}

		}
		else{
			throw std::exception("This should never happon");
		}

		return true;
	}

	std::list<std::string> unwrapPacakges(std::string data){
		std::list<std::string> packages;

		char terminator = '\1';

		bool startFound = false;
		size_t startPos = std::string::npos;
		size_t endPos = -1;
		do{
			startPos = data.find_first_of(terminator, endPos +1);
			endPos = data.find_first_of(terminator, startPos + 1);

			if (startPos != std::string::npos && endPos != std::string::npos){
				std::string package = data.substr(startPos + 1, endPos - startPos - 1);
				packages.push_back(package);
			}

		} while (startPos != std::string::npos && endPos != std::string::npos);
		return packages;
	}

	Event_Type decodeEventType(const std::string &data){
		Event_Type type = (Event_Type)data[0];
		return type;
	}

	std::string readCloseSockets(){
		std::list<SOCK_Stream*>::iterator iter;
		//first socket is only for listening
		for (iter = ++readSockets.begin(); iter != readSockets.end(); ++iter){
			try{
				std::string recived = (*iter)->recive();
				if (!recived.empty())
					return recived;
			}
			catch (SOCK_Exception &e){
				if (e.errorCode == 0){//closed socket?
					delete(*iter);
					readSockets.erase(iter++);
					return std::string();
					//return std::string("closed a socket");
				}
				else
					throw e;
			}
		}
		return std::string();
	}

	void callEvents(Event_Type eventType, std::string data){
		std::list<iEventHandler*> eventHandlers;
		switch (eventType){
		case EVENT_ALARM:
			eventHandlers = alarmEventCbs;
			break;
		case EVENT_PATIENT_VALUE:
			eventHandlers = patientValueEventCbs;
			break;
		case EVENT_LOG:
		default:
			eventHandlers = logEventCbs;
			break;
		}

		std::list<iEventHandler*>::const_iterator iter;
		for (iter = eventHandlers.cbegin(); iter != eventHandlers.end(); ++iter){
			(*iter)->handleEvent(data);
		}
	}

	SOCK_Acceptor acceptor;

	std::list<SOCK_Stream*> readSockets;
	std::list<SOCK_Stream*> writeSockets;
	std::list<SOCK_Stream*> errorSockets;

	std::list<iEventHandler*> alarmEventCbs;
	std::list<iEventHandler*> patientValueEventCbs;
	std::list<iEventHandler*> logEventCbs;


};

