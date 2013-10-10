#pragma once

#include "iEventHandler.h"
#include <tuple>
#include <memory>

class Reactor
{
public:

	virtual void handleEvents(int timeout) = 0;

	virtual void registerHandler(std::shared_ptr<iEventHandler> eh, int et){
		std::pair<int, std::shared_ptr<iEventHandler>> element(et, eh);
		registeredHandlers.push_back(element);
	}

	virtual void removeHandler(std::shared_ptr<iEventHandler> eh, int et){
		auto it = registeredHandlers.begin();
		while (++it != registeredHandlers.end()){
			if (it->first == et && it->second.get() == eh.get())
				return;
		}

		throw std::exception("eventhandler was not found");
	}

protected:
	std::list< std::pair< int, std::shared_ptr<iEventHandler> > > registeredHandlers;

private:

};

