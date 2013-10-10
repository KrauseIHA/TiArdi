#pragma once
#include "iEventHandler.h"
#include "SOCK_Stream.h"
#include "TransportHandle.h"

class ServiceHandler :
	public iEventHandler
{
public:

	void handleEvent(std::string data){

	}

	std::shared_ptr<SocketHandle> getHandle(){
		std::shared_ptr<SocketHandle> handlePtr(&handle);
		return handlePtr;
	}

private:
	TransportHandle handle;

};

