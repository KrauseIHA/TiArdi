#pragma once

//http://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "INET_Addr.h"
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

	SOCK_Stream *initialize(const u_short port){
		listenSOCK.initialise(port);
		listenSOCK.bind();
		listenSOCK.listen();
		return &listenSOCK;
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


	std::string accept() {
		newestSockStream = listenSOCK.accept();
		std::string address = newestSockStream->getAddr().getIpAddr();
		clientSOCKs.insert(std::pair<std::string, SOCK_Stream*>( address, newestSockStream));
		return address;
	}

	void closeConnection(std::string address){
		SOCK_Stream *socket = clientSOCKs[address];
		clientSOCKs.erase(address);
		delete socket;
	}


	~SOCK_Acceptor(){
		std::map<std::string, SOCK_Stream*>::iterator iter;

		for (iter = clientSOCKs.begin(); iter != clientSOCKs.end(); ++iter){
			delete (*iter).second;
		}

	}

private:

	SOCK_Stream listenSOCK;
	std::map<std::string, SOCK_Stream*> clientSOCKs;
	SOCK_Stream *newestSockStream;
};

