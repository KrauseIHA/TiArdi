#pragma once

//#include "INET_Addr.h"
#include "SOCK_Stream.h"

#include <exception>
#include <string>
#include <map>


class SOCK_Acceptor
{
public:

	// Initialize a passive-mode acceptor socket.
	SOCK_Acceptor() {
	}

	SOCK_Acceptor(const u_short port) {
		initialize(port);
	}

	void initialize(const u_short port){
		listenSocket.reset(new SocketHandle());
		listenSocket->initialize(port);
		listenSocket->bind();
		listenSocket->listen();
	}

	std::shared_ptr<SocketHandle> getListeningSocket(){
		return listenSocket;
	}


	std::string recive(){
		return newestSockStream->recive();
	}

	std::string recive(std::string ip){
		return clientSOCKs[ip]->recive();
	}


	void send(std::string sendBuff){
		newestSockStream->send(sendBuff);
	}


	SOCK_Stream accept() {
		std::shared_ptr<SocketHandle> socket = listenSocket->accept();
		std::string address = socket->getAddr()->getIpAddr();
		newestSockStream = std::make_shared<SOCK_Stream>(socket);
		std::pair<std::string, std::shared_ptr<SOCK_Stream>> _pair(address, newestSockStream);
		clientSOCKs.insert(_pair);
		return *newestSockStream;
	}

	void closeConnection(std::string address){
		clientSOCKs.erase(address);
	}

private:

	std::shared_ptr<SocketHandle> listenSocket;
	std::map<std::string, std::shared_ptr<SOCK_Stream>> clientSOCKs;
	std::shared_ptr<SOCK_Stream> newestSockStream;

};

