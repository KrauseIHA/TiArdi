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

	SOCK_Stream(std::shared_ptr<SocketHandle> socket){
		initialize(socket);
	}

	void initialize(std::shared_ptr<SocketHandle> socket){
		this->socket = socket;
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



