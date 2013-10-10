#pragma once

#include <string>
#include "SocketHandle.h"

class iEventHandler
{
public:

	virtual void handleEvent() = 0;

	virtual std::shared_ptr<SocketHandle> getHandle() = 0;


};
