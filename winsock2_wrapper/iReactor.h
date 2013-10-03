#pragma once

#include "iEventHandler.h"

class iReactor
{
public:
	
	virtual void handleEvents(int timeout) = 0;

	//virtual void registerHandler(iEventHandler *eh, Event_Type et) = 0;
	//virtual void removeHandler(iEventHandler *eh, Event_Type et) = 0;

};

