#pragma once


#include "SocketHandle.h"

#include <string>
#include <list>

class SOCK_Stream
{
public:

	// Default.
	SOCK_Stream() {

	}

	SOCK_Stream(SocketHandle &socket) {
		initialize(socket);
	}

	~SOCK_Stream() {

	}

	void initialize(const u_short port, const char *ip){
		socket.initialize(port, ip);
		socket.connect();
	}

	void initialize(SocketHandle &socket){
		this->socket = socket;
	}

	void send(std::string sendbuf){
		send(sendbuf.c_str());
	}

	void send(const char * sendbuf){
		socket.send(sendbuf);
	}

	std::string recive(){
		return socket.recive();
	}

	SocketHandle *getSocket(){
		return &socket;
	}
	
private:
	SocketHandle socket;
};



