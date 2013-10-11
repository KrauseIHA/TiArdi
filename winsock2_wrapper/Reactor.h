#pragma once

#include "iEventHandler.h"
#include <tuple>
#include <memory>

class Reactor
{
public:

	virtual void handleEvents() = 0;

	virtual void registerHandler(std::shared_ptr<iEventHandler> eh, int et){
		std::pair<int, std::shared_ptr<iEventHandler>> element(et, eh);
		registeredHandlers.push_back(element);
	}

	virtual void registerActor(std::shared_ptr<iEventHandler> eh, int et){
		std::pair<int, std::shared_ptr<iEventHandler>> element(et, eh);
		registeredActors.push_back(element);
	}

	virtual void removeHandler(iEventHandler *eh, int et){
		for (auto handler : registeredHandlers){

			if (handler.first == et && handler.second.get() == eh){
				registeredHandlers.remove(handler);
				return;
			}
		}

		throw std::exception("eventhandler was not found");
	}

	virtual void removeActor(iEventHandler *eh, int et){

		for (auto actor : registeredActors){

			if (actor.first == et && actor.second.get() == eh){
				registeredActors.remove(actor);
				return;
			}
		}

		throw std::exception("eventActor was not found");
	}


protected:
	std::list< std::pair< int, std::shared_ptr<iEventHandler> > > registeredHandlers;
	std::list< std::pair< int, std::shared_ptr<iEventHandler> > > registeredActors;

private:

};

