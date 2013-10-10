#pragma once
#include "../winsock2_wrapper/Reactor.h"
#include "../winsock2_wrapper/ConcreteReactor.h"
#include "../winsock2_wrapper/SOCK_Acceptor.h"

#include "PatientEventHandler.h"

#include <memory>

template <class HandlerClass>
class AcceptEventHandler :
	public iEventHandler
{
public:
	AcceptEventHandler(std::shared_ptr<Reactor> reactor)
		:reactor(reactor){

			acceptor.initialize(HandlerClass::getEventType());
			acceptor.getListeningSocket()->setNonBlocking();
	}

	void handleEvent(){
		auto handler = std::make_shared<HandlerClass>();
		handler->setHandle(acceptor.accept().getSocket());

		reactor->registerHandler(handler, _eventType);

	}

	std::shared_ptr<SocketHandle> getHandle(){
		return acceptor.getListeningSocket();
	}

private:
	const EVENT_TYPE _eventType = ACCEPTOR;
	std::shared_ptr<Reactor> reactor;
	SOCK_Acceptor acceptor;

};

