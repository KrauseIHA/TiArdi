#pragma once
#include "iEventHandler.h"
#include "SOCK_Stream.h"
#include "TransportHandle.h"

class ServiceHandler :
	public iEventHandler
{
public:

	ServiceHandler(){

	}

	~ServiceHandler(){

	}

	void handleEvent(std::string data){

	}

	TransportHandle getHandle(){
		return handle;
	}

private:
	TransportHandle handle;

};

