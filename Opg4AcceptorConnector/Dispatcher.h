#pragma once

//For the acceptor, the dispatcher demultiplexes connection indication events received
//on one or more transport handles that encapsulate transport endpoints.Multiple
//acceptors can register with a dispatcher, which listens on their behalf for connection
//requests to arrive from peer connectors

//For the connector, the dispatcher demultiplexes completion events that arrive in
//response to connections that were initiated asynchronously.To handle this situation, a
//connector registers itself with a dispatcher to receive these connection completion
//events.The dispatcher then runs its event loop.When a completion event arrives it
//notifies the corresponding connector.The connector can then invoke the designated
//service handler's activation hook method to allow the service handler to initialize itself. A
//single dispatcher and connector can therefore initiate and complete connections
//asynchronously on behalf of multiple service handlers.

#include "ServiceHandler.h"
#include "../winsock2_wrapper/Reactor.h"
#include <memory>


using namespace std;

class Dispatcher : public Reactor
{
public:

	void handleEvents(){
		bool running = true;

		while (running){
			running = internalHandleEvent();
		}
	}


protected:
	virtual void callHandelevent(pair<int, shared_ptr<iEventHandler>> eventHandler){
		eventHandler.second->handleEvent();
	}

private:

	shared_ptr < list < shared_ptr < SocketHandle >> > createListOfSockets(){
		auto output = make_shared < list < shared_ptr < SocketHandle >> >();

		auto it = registeredHandlers.begin();

		if (registeredHandlers.size() == 0){
			return output;
		}

		do{
			output->push_front(it->second->getHandle());
		} while (++it != registeredHandlers.end());

		return output;
	}

	bool internalHandleEvent(){
		auto readSockets = createListOfSockets();

		SocketHandle::select(*(readSockets.get()), emptylist, emptylist);

		if (readSockets->size() == 0)
			return true;

		auto itSockets = readSockets->begin();

		do{

			auto itHandlers = registeredHandlers.begin();
			do{
				if ((*itSockets) == itHandlers->second->getHandle()){
					try{
						callHandelevent(*itHandlers);
					}
					catch (SOCK_Exception &e){
						if (e.errorCode == 0)
							registeredHandlers.erase(itHandlers--);
						else
						{
							throw e;
						}
					}

				}
			} while (++itHandlers != registeredHandlers.end());

		} while (++itSockets != readSockets->end());

		return true;

	}

	std::list<std::shared_ptr<SocketHandle>> emptylist;

};

