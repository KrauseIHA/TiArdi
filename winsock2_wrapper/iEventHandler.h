#pragma once

#include <string>

class iEventHandler
{
public:
	virtual void handleEvent(std::string data) = 0;

};
