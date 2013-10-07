#pragma once

//http://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "SOCK_Stream.h"
#include "INET_Addr.h"
#include <Ws2tcpip.h>
#include <cstdio>
#include <string>


#pragma comment(lib, "Ws2_32.lib")
class SOCK_Connector
{
public:

	typedef INET_Addr PEER_ADDR;
	typedef SOCK_Stream PEER_STREAM;

	SOCK_Connector(const char * ip, const u_short port)
	{
		serverStream.initialize(port, ip);
	}

	void send(const char * sendbuf){
		serverStream.send(sendbuf);
	}

	std::string recive(){
		return serverStream.recive();
	}

	~SOCK_Connector(){

	}

private:

	SOCK_Stream serverStream;


};

