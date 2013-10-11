#pragma once
#include "../winsock2_wrapper/iEventHandler.h"
#include "../winsock2_wrapper/SOCK_Stream.h"
#include "../winsock2_wrapper/Reactor.h"
#include "EventType.h"
#include "Dispatcher.h"

class ServiceHandler :
	public iEventHandler
{
public:
	ServiceHandler(){

	}

	ServiceHandler(std::shared_ptr<Reactor> _dispatcher) : dispatcher(_dispatcher)	{

	}

	virtual void handleEvent(std::string data) = 0;

	void handleEvent() {

		string *data = new string(sockStream.recive());

		handleEvent(*data);
	}

	void setStream(SOCK_Stream stream)
	{
		sockStream = SOCK_Stream(stream);
	}

	std::shared_ptr<SocketHandle> getHandle(){
		return sockStream.getSocket();
	}

	virtual void open() = 0;

protected:
	SOCK_Stream sockStream;
	std::shared_ptr<Reactor> dispatcher;


};

