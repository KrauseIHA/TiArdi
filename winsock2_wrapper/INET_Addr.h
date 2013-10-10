#pragma once

//http://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS


#include <winsock2.h>
#include <Ws2tcpip.h>

#include <memory>

#include "SOCK_Exception.h"


#pragma comment(lib, "Ws2_32.lib")

class AddrinfoDeleter {
public:

	void operator()(addrinfo *addr){
		::freeaddrinfo(addr);
	}
};


class INET_Addr
{
public:

	INET_Addr(){}

	INET_Addr(const u_short port, const char *address = NULL, addrinfo conectionInfo = INET_Addr::createAddinfo()){
		initialize(port, address, conectionInfo);
	}

	INET_Addr(const u_short port, addrinfo conectionInfo = INET_Addr::createAddinfo()){
		initialize(port, NULL, conectionInfo);
	}

	INET_Addr(const u_short port){
		INET_Addr(port, NULL);
	}

	/**
	*	port numbers are defined using 16 bits, same as u_short
	**/
	void initialize(const u_short port, addrinfo conectionInfo){
		initialize(port, NULL, conectionInfo);
	}

	void initialize(const u_short port, const char *address = NULL, addrinfo conectionInfo = INET_Addr::createAddinfo()){
		char portStr[MAX_LENGTH_OF_PORTNR + 1]; // 1 extra for terminating 0

		this->port = port;
		if (address != NULL){
			const u_int SIZE = 100;
			this->address = (char*)malloc(SIZE);
			strcpy_s(this->address, SIZE, address);
		}
		else
			this->address = NULL;

		sprintf_s(portStr, "%u", port);

		initializeWinSock();

		addrinfo *result;

		// Resolve the local address and port to be used by the server
		iResult = ::getaddrinfo(address, portStr, &conectionInfo, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed: %d\n", iResult);
			//close();
			throw SOCK_Exception(WSAGetLastError(), "Address error");
		}

		addrRef.reset(result, AddrinfoDeleter());

	}

	static addrinfo createAddinfo(){
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof (hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;
		return hints;
	}

	u_short get_port() const {
		return port;
	}

	char *getIpAddr() const{
		return address;
	}

	size_t size() const
	{
		return sizeof (*addrRef);
	}

	~INET_Addr(void){
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
	const static int MAX_LENGTH_OF_PORTNR = 5;

	char * address;
	u_short port;

	std::shared_ptr<addrinfo> addrRef;

	int iResult;
	static unsigned int openSockets;

	friend class SocketHandle;
};

