#pragma once
#include "iEventHandler.h"
#include "SOCK_Stream.h"
#include "TransportHandle.h"

class ServiceHandler :
	public iEventHandler
{
public:

	ServiceHandler(SOCK_Stream stream)
	{
		sockStream = SOCK_Stream(stream);
	}

	void handleEvent(std::string data){

	}

	void setHandle(SOCK_Stream stream)
	{
		sockStream = SOCK_Stream(stream);
	}

	std::shared_ptr<SocketHandle> getHandle(){
		std::shared_ptr<SocketHandle> handlePtr(sockStream.getSocket());
		return handlePtr;
	}

private:
	SOCK_Stream sockStream;

};

