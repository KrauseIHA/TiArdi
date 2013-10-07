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
		listenSocket.initialize(port);
		listenSocket.bind();
		listenSocket.listen();
	}

	SocketHandle getListeningSocket(){
		return listenSocket;
	}


	std::string recive(){
		return newestSockStream.recive();
	}

	std::string recive(std::string ip){
		return clientSOCKs[ip].recive();
	}


	void send(std::string sendBuff){
		newestSockStream.send(sendBuff);
	}


	std::string accept() {
		SocketHandle socket = listenSocket.accept();
		std::string address = socket.getAddr()->getIpAddr();
		newestSockStream = SOCK_Stream(socket);
		clientSOCKs.insert(std::pair<std::string, SOCK_Stream>(address, newestSockStream));
		return address;
	}

	void closeConnection(std::string address){
		SOCK_Stream socket = clientSOCKs[address];
		clientSOCKs.erase(address);
	}


	~SOCK_Acceptor(){

	}

private:

	SocketHandle listenSocket;
	std::map<std::string, SOCK_Stream> clientSOCKs;
	SOCK_Stream newestSockStream;
};

