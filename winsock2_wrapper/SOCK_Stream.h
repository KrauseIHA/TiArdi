#pragma once


#include "SocketHandle.h"

#include <string>
#include <list>

class SOCK_Stream
{
public:

	// Default.
	SOCK_Stream() {
		socket = std::make_shared<SocketHandle>();
	}

	SOCK_Stream(SocketHandle socket){
		initialize(socket);
	}

	void initialize(const u_short port, const char *ip){
		socket->initialize(port, ip);
		socket->connect();
	}

	void initialize(SocketHandle &socket){
		this->socket = std::make_shared<SocketHandle>(socket);
	}

	void send(std::string sendbuf){
		send(sendbuf.c_str());
	}

	void send(const char * sendbuf){
		socket->send(sendbuf);
	}

	std::string recive(){
		return socket->recive();
	}

	std::shared_ptr<SocketHandle> getSocket(){
		return socket;
	}
	
private:

	std::shared_ptr<SocketHandle> socket;

};



