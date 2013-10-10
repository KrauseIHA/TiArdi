#pragma once

//For the acceptor, the dispatcher demultiplexes connection indication events received
//on one or more transport handles that encapsulate transport endpoints.Multiple
//acceptors can register with a dispatcher, which listens on their behalf for connection
//requests to arrive from peer connectors

//For the connector, the dispatcher demultiplexes completion events that arrive in
//response to connections that were initiated asynchronously.To handle this situation, a
//connector registers itself with a dispatcher to receive these connection completion
//events.The dispatcher then runs its event loop.When a completion event arrives it
//notifies the corresponding connector.The connector can then invoke the designated
//service handler's activation hook method to allow the service handler to initialize itself. A
//single dispatcher and connector can therefore initiate and complete connections
//asynchronously on behalf of multiple service handlers.

#include "ServiceHandler.h"

#include "TransportHandle.h";
#include <memory>

class Dispatcher
{
public:

	std::shared_ptr<Dispatcher> getInstance(){
		if (instance.use_count() == 0){
			instance.reset( new Dispatcher() );
		}

		return instance;
	}

	~Dispatcher(){

	}

	void handleEvents(){

	}

	void registerHandler(iEventHandler *eventHandle){
		auto endpoint = std::make_shared<TransportHandle>(eventHandle->getHandle());
	}

	void removeHandler(iEventHandler *eventHandle){

	}

	void select(){
		std::list<SocketHandle> list;

		TransportHandle::select(list, list, list);
	}

private:
	Dispatcher(){

	}

	static std::shared_ptr<Dispatcher> instance;

	std::list<SocketHandle> observed;

};

