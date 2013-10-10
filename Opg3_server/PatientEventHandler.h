#pragma once
#include "..\winsock2_wrapper\iEventHandler.h"
#include "..\winsock2_wrapper\SOCK_Stream.h"

#include <cstdio>

enum EVENT_TYPE{
	INVALID_EVENT1,
	INVALID_EVENT2,
	ACCEPTOR,
	EVENT_ALARM = 50401,
	EVENT_PATIENT_VALUE = 50402,
	EVENT_LOG = 50403
};


class PatientEventHandler :
	public iEventHandler
{
public:

	void setHandle(std::shared_ptr<SocketHandle> handle){
		this->handle = handle;
	}

	virtual void handleEvent(std::string data) = 0;

	void handleEvent(){
		std::string data = handle->recive();
		handleEvent(data);
	}

	std::shared_ptr<SocketHandle> getHandle(){
		return handle;
	}

	//virtual const EVENT_TYPE getEventType() = 0;

private:
	std::shared_ptr<SocketHandle> handle;
};

