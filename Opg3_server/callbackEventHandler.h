#pragma once

#include "PatientEventHandler.h"

typedef void(*EventHandlerCB)(std::string data);

class callbackEventHandler :
	public PatientEventHandler
{
public:

	callbackEventHandler(EventHandlerCB eventCB, EVENT_TYPE eventType)
		: _eventType(eventType), eventHandler(eventCB){

	}

	void handleEvent(std::string data){
		(*eventHandler)(data);
	};

	const EVENT_TYPE getEventType(){
		return _eventType;
	};


private:
	const EVENT_TYPE _eventType;
	const EventHandlerCB eventHandler;

};

