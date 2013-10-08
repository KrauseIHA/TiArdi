#pragma once

#include "../winsock2_wrapper/iEventHandler.h"

//A connector[13] is a factory that implements the strategy for actively establishing a connected
//transport endpoint and initializing its associated transport handle and service handler.It
//provides two methods, connection initiation and connection completion, that perform these
//steps.

//The connection initiation method is passed an existing service handler and establishes a
//connected transport endpoint for it with an acceptor.This acceptor must be listening for
//connection requests to arrive on a particular transport address, as described above.


//Separating the connector's connection initiation method from its completion method allows a
//connector to support both synchronous and asynchronous connection establishment
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

class Connector :
	public iEventHandler
{
public:

	Connector(){

	}

	~Connector(){

	}

	void handleEvent(std::string data){

	}

	void connect(){

	}

	void compleate(){

	}

};
