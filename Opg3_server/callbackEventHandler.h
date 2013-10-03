#pragma once
#include "C:\Users\Stefan\Documents\Visual Studio 2013\Projects\TI-Arti-Opg1\winsock2_wrapper\iEventHandler.h"

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

