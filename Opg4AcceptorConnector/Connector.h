#pragma once

//A connector[13] is a factory that implements the strategy for actively establishing a connected
//transport endpoint and initializing its associated transport handle and service handler.It
//provides two methods, connection initiation and connection completion, that perform these
//steps.

//The connection initiation method is passed an existing service handler and establishes a
//connected transport endpoint for it with an acceptor.This acceptor must be listening for
//connection requests to arrive on a particular transport address, as described above.


//Separating the connector's connection initiation method from its completion method allows a
//connector to support both synchronous and asynchronous connection establishmentz
//transparently :

//-synchronous case-
// the connector initiating the connection request blocks its
//caller until the transport endpoints are connected.At this point, the connector calls the
//service handler's activation hook method directly.

//-asynchronous case-
//the connection request runs asynchronously and the
//connector's initiation method returns immediately. The service handler is activated by
//the connection completion method only after the connector is notified that the transport
//endpoint has finished connecting asynchronously

#include "../winsock2_wrapper/iEventHandler.h"
#include "../winsock2_wrapper/SOCK_Connector.h"

#include "ServiceHandler.h"
#include "../winsock2_wrapper/Reactor.h"

template<class tServiceHandler>
class Connector :
	public iEventHandler
{
public:

	Connector(std::string ip, const u_short port, std::shared_ptr<Reactor> dispatcher)
		: _dispatcher(dispatcher){
			initialize(ip, port);
	}

	void initialize(std::string ip, const u_short port){
		connector.initialize(ip.c_str(), port);
	}

	void handleEvent(){
		_dispatcher->removeActor(this, EventType::CONNECTOR);
		complete();
	}

	void connect(bool isAsyncronus){
		this->_isAsyncronus = isAsyncronus;
		connector.getHandle()->setNonBlocking(_isAsyncronus);
		auto stream = connector.connect();

		if (!_isAsyncronus)
			complete();
		else
			_dispatcher->registerActor(make_shared<Connector>(*this), EventType::CONNECTOR);
	}

	void complete(){
		auto socket = connector.getHandle();
		std::shared_ptr<ServiceHandler> serviceHandler = std::make_shared<tServiceHandler>(_dispatcher);
		serviceHandler->setStream(socket);
		serviceHandler->open();

	}

	std::shared_ptr<SocketHandle> getHandle(){
		return connector.getHandle();
	}

private:
	bool _isAsyncronus;
	SOCK_Connector connector;
	std::shared_ptr<Reactor> _dispatcher;
};

