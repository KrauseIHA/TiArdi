#pragma once

//http://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define FD_SETSIZE 64 //this is the default size and it needs to be defined before includeing winsock2.h
#include <winsock2.h>

#include "INET_Addr.h"
#include <string>
#include <list>

class SOCK_Stream
{
public:

	// Default.
	SOCK_Stream() {
		socketHandle = INVALID_SOCKET;
	}

	SOCK_Stream(INET_Addr &addr) {
		socketHandle = INVALID_SOCKET;
		addr.copyInitialize(addr);
		initialise();
	}

	SOCK_Stream(const u_short port, const char * address = NULL) {
		socketHandle = INVALID_SOCKET;
		initialise(port, address);
	}

	void initialise(const u_short port, const char * address = NULL){
		addr.initialise(port, address);
		initialise();
	}

	void initialise(){

		if (socketHandle != INVALID_SOCKET)
			throw SOCK_Exception(0, "Socket has already been initialised! Needs to be cleaned first");

		initializeSocket();

		INET_Addr::openSockets++;
	}

	void initialise(SOCK_Stream &from){
		this->socketHandle = from.socketHandle;
		from.socketHandle = INVALID_SOCKET;
		this->addr = from.addr;
	}


	void bind(){
		if (socketHandle == INVALID_SOCKET)
			throw SOCK_Exception(0, "Socket Error");

		addrinfo *result = addr.addr_;
		// Setup the TCP listening socket
		iResult = ::bind(socketHandle, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			//printf("bind failed with error: %d\n", WSAGetLastError());
			delete this;
			throw SOCK_Exception(WSAGetLastError(), "Bind error");
		}

	}

	void listen(){
		if (socketHandle == INVALID_SOCKET)
			throw SOCK_Exception(0, "Socket Error");

		iResult = ::listen(socketHandle, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			//printf("listen failed with error: %d\n", WSAGetLastError());
			delete this;
			throw SOCK_Exception(WSAGetLastError(), "Listen error");
		}
	}

	SOCK_Stream *accept(){
		if (socketHandle == INVALID_SOCKET)
			throw SOCK_Exception(0, "Socket Error");

		sockaddr_in  connecingHost;
		int sizeConnectingHost = sizeof (connecingHost);
		ZeroMemory(&connecingHost, sizeConnectingHost);

		sockaddr * _connectinghost = (sockaddr *) &connecingHost;
		SOCKET clientSocket = ::WSAAccept(socketHandle, _connectinghost, &sizeConnectingHost, NULL, NULL);

		int errorNo = WSAGetLastError();

		if (errorNo == WSAEWOULDBLOCK){
			return NULL;
		}

		INET_Addr address(connecingHost.sin_port, inet_ntoa(connecingHost.sin_addr));
		if (clientSocket == INVALID_SOCKET) {
			throw SOCK_Exception(errorNo, "accept error");
		}

		return new SOCK_Stream(clientSocket, address);
	}

	void static select(std::list<SOCK_Stream*> &read, std::list<SOCK_Stream*> &write, std::list<SOCK_Stream*> &error, u_long timeout_usec){

		const timeval _timeout = { 0, timeout_usec };

		return select(read, write, error, &_timeout);
	}

	void static select(std::list<SOCK_Stream*> &read, std::list<SOCK_Stream*> &write, std::list<SOCK_Stream*> &error){
		const timeval *_timeout = NULL;
		return select(read, write, error, _timeout);
	}

	const INET_Addr getAddr(){
		return addr;
	}

	void connect(){
		if (socketHandle == INVALID_SOCKET)
			throw SOCK_Exception(0, "Socket Error");

		int socketError = SOCKET_ERROR;
		iResult = ::WSAConnect(socketHandle, addr.addr_->ai_addr, addr.addr_->ai_addrlen, NULL, NULL, NULL, NULL);
		if (iResult == socketError) {
			int errorCode = WSAGetLastError();
			throw SOCK_Exception(errorCode, "connect error");
		}

		// Should really try the next address returned by getaddrinfo
		// if the connect call failed
		// But for this simple example we just free the resources
		// returned by getaddrinfo and print an error message

		if (socketHandle == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			throw SOCK_Exception(0, "connect error");
		}
	}




	void send(std::string &sendBuff, int flags = 0){
		send(sendBuff.c_str(), flags);
	}

	void send(const char sendBuff[], int flags = 0){
		int length = strlen(sendBuff);

		iResult = ::send(socketHandle, sendBuff, length, flags);
		if (iResult == SOCKET_ERROR) {
			throw SOCK_Exception(WSAGetLastError(), "Error Sending message");
		}
	}

	std::string recive(int falgs = 0){
		if (socketHandle == INVALID_SOCKET) {
			return NULL;
		}

		char recvBuff[DEFAULT_BUFLEN + 1]; // extraspace for null termination
		iResult = ::recv(socketHandle, recvBuff, DEFAULT_BUFLEN, falgs);

		if (iResult == 0){
			throw SOCK_Exception(WSAGetLastError(), "Socket has been gracefully closed, delete me!");
		}

		if (iResult <= 0)
			return std::string();

		recvBuff[iResult] = 0; //add null Termination

		return std::string(recvBuff);
	}

	void setNonBlocking(bool enable = true){
		u_long nonblocking = enable;
		
		::ioctlsocket(socketHandle, FIONBIO, &nonblocking);
	}

	void cleanUp(){
		if (socketHandle != INVALID_SOCKET){
			closesocket(socketHandle);
			INET_Addr::openSockets--;
			socketHandle = INVALID_SOCKET;
		}

		if (INET_Addr::openSockets == 0)
			WSACleanup();

	}

	// Automatically close the handle on destruction.
	~SOCK_Stream() {
		cleanUp();
	}

private:
	// Socket handle for exchanging socket data.
	const static int DEFAULT_BUFLEN = 512;

	fd_set static *convertListToFdSet(const std::list<SOCK_Stream*> &sockets){
		fd_set *fdSetSockets = new fd_set;

		int i = 0;
		for (std::list<SOCK_Stream*>::const_iterator iter = sockets.cbegin(); iter != sockets.end(); ++iter){
			fdSetSockets->fd_array[i] = (*iter)->socketHandle;
			i++;
		}

		fdSetSockets->fd_count = i;
		return fdSetSockets;
	}

	void static select(std::list<SOCK_Stream*> &read, std::list<SOCK_Stream*> &write, std::list<SOCK_Stream*> &error, const timeval *timeout){

		fd_set *_read = convertListToFdSet(read);
		fd_set *_write = convertListToFdSet(write);
		fd_set *_error = convertListToFdSet(error);

		int returnVal = ::select(0, _read, _write, _error, timeout);

		delete _read;
		delete _write;
		delete _error;

		if (returnVal == SOCKET_ERROR){
			throw SOCK_Exception(WSAGetLastError(), "Select returned error.");
		}
	}


	void initializeSocket(){

		// Create a SOCKET for the server to listen for client connections
		socketHandle = ::WSASocket(addr.addr_->ai_family, addr.addr_->ai_socktype, addr.addr_->ai_protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (socketHandle == INVALID_SOCKET) {

			throw SOCK_Exception(WSAGetLastError(), "Socket error");
		}

	}

	SOCK_Stream(SOCKET socketHandle, INET_Addr addr) : socketHandle(socketHandle) {
		this->socketHandle = socketHandle;
		this->addr = addr;
		INET_Addr::openSockets++;
	}

	SOCK_Stream(const SOCK_Stream& soc){ throw SOCK_Exception(0, "This Should never happon"); }

	int iResult;
	INET_Addr addr;
	SOCKET socketHandle;
};



