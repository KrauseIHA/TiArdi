#pragma once
#include "..\winsock2_wrapper\iEventHandler.h"

typedef void(*EventHandlerCB)(std::string data);

class callbackEventHandler :
	public iEventHandler
{
public:

	callbackEventHandler(EventHandlerCB eventCB){
		eventHandler = eventCB;
	}

	~callbackEventHandler(){
	}

	void handleEvent(std::string data){
		(*eventHandler)(data);
	};


private:

	EventHandlerCB eventHandler;

};

