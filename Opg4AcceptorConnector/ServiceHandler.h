#pragma once
#include "iEventHandler.h"
#include "SOCK_Stream.h"
#include "TransportHandle.h"
#include "EventType.h"

class ServiceHandler :
	public iEventHandler
{
public:

	ServiceHandler(shared_ptr<Dispatcher> _dispatcher)
	{
		dispatcher = _dispatcher;
	}

	void handleEvent(std::string data){

	}

	void handleEvent() {
		
	}

	void setStream(SOCK_Stream stream)
	{
		sockStream = SOCK_Stream(stream);
	}

	std::shared_ptr<SocketHandle> getHandle(){
		std::shared_ptr<SocketHandle> handlePtr(sockStream.getSocket());
		return handlePtr;
	}

	virtual EventType getType() = 0;

	virtual void open() = 0;

private:
	SOCK_Stream sockStream;
	shared_ptr<Dispatcher> dispatcher;
};

