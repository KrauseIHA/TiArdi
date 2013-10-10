#pragma once

#include "SOCK_Stream.h"
#include "../winsock2_wrapper/iEventHandler.h"
#include "INET_Addr.h"
#include <cstdio>
#include <string>

class SOCK_Connector
{
public:
	SOCK_Connector(){
	}

	SOCK_Connector(const char * ip, const u_short port)
	{
		initialize(ip, port);
	}

	void initialize(const char * ip, const u_short port){

		socket = std::make_shared<SocketHandle>(port, ip);
	}

	std::shared_ptr<SOCK_Stream> connect(){
		socket->connect();
		return std::make_shared<SOCK_Stream>(socket);
	}

	void send(const char * sendbuf){
		socket->send(sendbuf);
	}

	//std::string recive(){
	//	return socket->recive();
	//}

	std::shared_ptr<SocketHandle>getHandle(){
		return socket;
	}

private:
	std::shared_ptr<SocketHandle> socket;


};

