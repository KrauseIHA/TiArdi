#pragma once

//An acceptor is a factory that implements a strategy for passively establishing a connected
//transport endpoint, and creating and initializing its associated transport handle and service
//handler.An acceptor provides two methods, connection initialization and connection
//completion, that perform these steps with the help of a passive - mode transport endpoint.

//When its initialization method is called, an acceptor binds its passive - mode transport
//endpoint to a particular transport address, such as a TCP port number and IP host address,
//that listens passively for the arrival of connection requests.

//Firstly
//It uses its passive - mode transport endpoint to create a connected transport
//endpoint and encapsulate the endpoint with a transport handle.

//Secondly
//It creates a service handler that will process data requests emanating from
//its peer service handler via their connected transport endpoints.

//Thirdly
//It stores the transport handle in its associated service handler and invokes the
//service handler's activation hook method, which allows the service handler to finish 
//initializing itself.

//Networked applications and services can be built by subclassing and instantiating the
//generic participants of the Acceptor - Connector pattern described above to create the
//following concrete components

#include "iEventHandler.h"
#include "SOCK_Acceptor.h"
#include "Dispatcher.h"

enum INITIATION_MODE
{
	ACTIVE,
	PASSIVE
};

class Acceptor :
	public iEventHandler
{
public:

	Acceptor(){
		
	}

	void initialize(const u_short port, INITIATION_MODE = PASSIVE){
		peerAcceptor.initialize(port);

		peerAcceptor.getListeningSocket().setNonBlocking(true);
		
	}


	~Acceptor(){
	}

	void setDispatcher(std::shared_ptr<Dispatcher> dispatcher){
		this->dispatcher = dispatcher;
	}

	void accept(){
		SOCK_Stream stream = peerAcceptor.accept();
	}

	void handleEvent(std::string data){

	}

private:
	SOCK_Acceptor peerAcceptor;
	std::shared_ptr<Dispatcher> dispatcher;

};
