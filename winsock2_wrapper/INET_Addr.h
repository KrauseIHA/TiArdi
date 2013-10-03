#pragma once

//http://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <Ws2tcpip.h>

#include "SOCK_Exception.h"
//#include <cstdio>

#pragma comment(lib, "Ws2_32.lib")

class INET_Addr
{
public:

	INET_Addr(){}

	INET_Addr(const INET_Addr &clone){
		copyInitialize(clone);
	}

	INET_Addr(const u_short port, const char *address = NULL, addrinfo conectionInfo = INET_Addr::createAddinfo()){
		initialise(port, address, conectionInfo);
	}

	INET_Addr(const u_short port, addrinfo conectionInfo = INET_Addr::createAddinfo()){
		initialise(port, NULL, conectionInfo);
	}

	/**
	*	port numbers are defined using 16 bits, same as u_short
	**/
	void initialise(const u_short port, addrinfo conectionInfo){
		initialise(port, NULL, conectionInfo);
	}

	void initialise(const u_short port, const char *address = NULL, addrinfo conectionInfo = INET_Addr::createAddinfo()){
		char portStr[MAX_LENGTH_OF_PORTNR + 1 ]; // 1 extra for terminating 0

		this->port = port;
		if (address != NULL){
			this->address = (char*)malloc(100);
			strcpy(this->address, address);
		}else
			this->address = NULL;
		
		std::sprintf(portStr, "%u", port);

		initializeWinSock();

		// Resolve the local address and port to be used by the server
		iResult = ::getaddrinfo(address, portStr, &conectionInfo, &addr_);
		if (iResult != 0) {
			printf("getaddrinfo failed: %d\n", iResult);
			//close();
			throw SOCK_Exception(WSAGetLastError(), "Address error");
		}
		noDelete = false;

	}

	static addrinfo createAddinfo(){
		struct addrinfo hints;
		char portStr[MAX_LENGTH_OF_PORTNR];

		ZeroMemory(&hints, sizeof (hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;
		return hints;
	}

	INET_Addr(const u_short port)
	{
		INET_Addr(port, NULL);
	}

	u_short get_port() const {
		return port;
	}

	char *getIpAddr() const{
		return address;
	}

	size_t size() const
	{
		return sizeof (addr_);
	}

	~INET_Addr(void){
		if (noDelete)
			return;

		freeaddrinfo(addr_);
	}

	static void initializeWinSock(){
		if (openSockets > 0)
			return;

		WSADATA wsaData;

		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed: %d\n", iResult);
			throw SOCK_Exception(WSAGetLastError(), "WSAStartup failed");
		}
	}

private:
	void copyInitialize(const INET_Addr &from){
		this->addr_ = from.addr_;
		this->port = from.port;
		this->address = from.address;

		noDelete = true;

		initializeWinSock(); // we don't know if this is a very old object, and cleanup has hos already been called
	}

	const static int MAX_LENGTH_OF_PORTNR = 5;

	char * address;
	u_short port;
	addrinfo *addr_;

	int iResult;
	bool noDelete;
	static unsigned int openSockets;

	friend class SOCK_Stream;
};

