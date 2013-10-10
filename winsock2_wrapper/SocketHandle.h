#pragma once

//http://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define FD_SETSIZE 64 //this is the default size and it needs to be defined before includeing winsock2.h
#include "INET_Addr.h"

#include <winsock2.h>
#include <string>
#include <list>
#include <memory>

class SocketDeleter {
public:

	void operator()(SOCKET *socket){
		::closesocket(*socket);
		INET_Addr::openSockets--;
	}
};

class SocketHandle
{
public:

	// Default.
	SocketHandle() {
	}

	SocketHandle(const u_short port, const char * address = NULL) {
		initialize(port, address);
	}

	SocketHandle(const SocketHandle& soc){
		copyInitialise(soc);
	}

	void copyInitialise(const SocketHandle &from){
		this->socketRef = from.socketRef;
	}

	void initialize(const u_short port, const char * address = NULL){
		INET_Addr addr;
		addr.initialize(port, address);
		initialize(addr);
	}

	void initialize(INET_Addr &addr){
		isNonBlockingVal = false;
		int count = socketRef.use_count();
		if (socketRef.use_count() > 0 && *socketRef == INVALID_SOCKET)
			throw SOCK_Exception(0, "Socket has already been initialised! Needs to be cleaned first");

		initializeSocket(addr);

		INET_Addr::openSockets++;
	}

	void bind(){

		if (socketRef.use_count() == 0 || *socketRef == INVALID_SOCKET)
			throw SOCK_Exception(SOCKET_ERROR, "Socket Error");

		addrinfo *addr = &(*inetAddrRef->addrRef);

		// Setup the TCP listening socket
		iResult = ::bind(*socketRef, addr->ai_addr, (int)addr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			throw SOCK_Exception(WSAGetLastError(), "Bind error");
		}

	}

	void listen(){

		if (socketRef.use_count() == 0 || *socketRef == INVALID_SOCKET)
			throw SOCK_Exception(0, "Socket Error");

		iResult = ::listen(*socketRef, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			throw SOCK_Exception(WSAGetLastError(), "Listen error");
		}
	}

	std::shared_ptr<SocketHandle> accept(){
		if (socketRef.use_count() == 0 || (*socketRef) == INVALID_SOCKET)
			throw SOCK_Exception(0, "Socket Error");

		std::shared_ptr<SocketHandle> socketPtr;
		
		sockaddr_in  connecingHost;
		int sizeConnectingHost = sizeof (connecingHost);
		ZeroMemory(&connecingHost, sizeConnectingHost);

		sockaddr * _connectinghost = (sockaddr *)&connecingHost;
		SOCKET clientSocket = ::WSAAccept(*socketRef, _connectinghost, &sizeConnectingHost, NULL, NULL);

		int errorNo = WSAGetLastError();

		if (errorNo == WSAEWOULDBLOCK){
			return socketPtr; //empty uninitialized handle
		}

		INET_Addr address(connecingHost.sin_port, inet_ntoa(connecingHost.sin_addr));
		if (clientSocket == INVALID_SOCKET) {
			throw SOCK_Exception(errorNo, "accept error");
		}
		
		socketPtr.reset(new SocketHandle(clientSocket, address));
		return socketPtr;
	}

	void static select(std::list < std::shared_ptr < SocketHandle >> &read, std::list < std::shared_ptr < SocketHandle >> &write, std::list < std::shared_ptr < SocketHandle >> &error, u_long timeout_usec){

		const timeval _timeout = { 0, timeout_usec };

		return select(read, write, error, &_timeout);
	}

	void static select(std::list < std::shared_ptr < SocketHandle >> &read, std::list < std::shared_ptr < SocketHandle >> &write, std::list < std::shared_ptr < SocketHandle >> &error){
		const timeval *_timeout = NULL;
		return select(read, write, error, _timeout);
	}

	const std::shared_ptr<INET_Addr> getAddr(){
		return inetAddrRef;
	}

	void connect(){
		if (socketRef.use_count() == 0 || (*socketRef) == INVALID_SOCKET)
			throw SOCK_Exception(SOCKET_ERROR, "Socket Error");

		addrinfo *addrRef = &(*inetAddrRef->addrRef);

		iResult = ::WSAConnect((*socketRef), addrRef->ai_addr, addrRef->ai_addrlen, NULL, NULL, NULL, NULL);
		if (iResult == SOCKET_ERROR) {
			int errorCode = WSAGetLastError();
			throw SOCK_Exception(errorCode, "connect error");
		}

		// Should really try the next address returned by getaddrinfo
		// if the connect call failed
		// But for this simple example we just free the resources
		// returned by getaddrinfo and print an error message

		if (socketRef.use_count() == 0 || (*socketRef) == INVALID_SOCKET){
			printf("Unable to connect to server!\n");
			throw SOCK_Exception(0, "connect error");
		}
	}


	void send(std::string &sendBuff, int flags = 0){
		send(sendBuff.c_str(), flags);
	}

	void send(const char sendBuff[], int flags = 0){
		int length = strlen(sendBuff);

		iResult = ::send(*socketRef, sendBuff, length, flags);
		if (iResult == SOCKET_ERROR) {
			throw SOCK_Exception(WSAGetLastError(), "Error Sending message");
		}
	}

	std::string recive(int falgs = 0){
		if (socketRef.use_count() > 0)
			SOCKET s = *socketRef;

		if (socketRef.use_count() == 0 || (*socketRef) == INVALID_SOCKET){
			throw SOCK_Exception(SOCKET_ERROR, "Invalid Socket");
		}

		char recvBuff[DEFAULT_BUFLEN + 1]; // extraspace for null termination
		iResult = ::recv(*socketRef, recvBuff, DEFAULT_BUFLEN, falgs);

		if (iResult == 0){
			throw SOCK_Exception(WSAGetLastError(), "Socket has been gracefully closed, while waiting to recive data.");
		}

		if (iResult <= 0)
			throw SOCK_Exception(WSAGetLastError(), "Recive Error");


		recvBuff[iResult] = 0; //add null Termination

		return std::string(recvBuff);
	}

	void setNonBlocking(bool enable = true){
		u_long nonblocking = enable;
		isNonBlockingVal = enable;

		::ioctlsocket(*socketRef, FIONBIO, &nonblocking);
	}

	bool isNonBlocking(){
		return isNonBlockingVal;
	}

	bool isValid(){
		if (socketRef.use_count() == 0)
			return false;
		if ((*socketRef) == INVALID_SOCKET)
			return false;

		return true;
	}


private:
	// Socket handle for exchanging socket data.
	const static int DEFAULT_BUFLEN = 512;
	bool isNonBlockingVal;


	static void matchListToFd_set(fd_set *fdset, std::list < std::shared_ptr < SocketHandle >> &list){
		auto it = list.begin();
		bool remove = true;
		while (it != list.end())
		{
			remove = true;
			for (u_int i = 0; i < fdset->fd_count; i++){
				if (*it->get()->socketRef == fdset->fd_array[i]){
					remove = false;
					break;
				}
			}

			if (remove){
				list.erase(it++);
			}
			else{
				it++;
			}
		}
	}

	static fd_set *listToFd_set(const std::list < std::shared_ptr < SocketHandle >> &sockets){
		fd_set *fdSetSockets = new fd_set;

		int i = 0;
		for (auto iter = sockets.cbegin(); iter != sockets.end(); ++iter){
			fdSetSockets->fd_array[i] = *(*iter)->socketRef.get();
			i++;
		}

		fdSetSockets->fd_count = i;
		return fdSetSockets;
	}

	void static select(std::list < std::shared_ptr < SocketHandle >> &read, std::list < std::shared_ptr < SocketHandle >> &write, std::list < std::shared_ptr < SocketHandle >> &error, const timeval *timeout){

		fd_set *_read = listToFd_set(read);
		fd_set *_write = listToFd_set(write);
		fd_set *_error = listToFd_set(error);

		int returnVal = ::select(0, _read, _write, _error, timeout);

		matchListToFd_set(_read, read);
		matchListToFd_set(_write, write);
		matchListToFd_set(_error, error);

		delete _read;
		delete _write;
		delete _error;

		if (returnVal == SOCKET_ERROR){
			throw SOCK_Exception(WSAGetLastError(), "::select returned error.");
		}
	}

	void socketDeleter(SOCKET socket){
		::closesocket(socket);
	}


	void initializeSocket(INET_Addr &addr){

		addrinfo *_addr = &(*addr.addrRef);

		// Create a SOCKET for the server to listen for client connections
		SOCKET socket = ::WSASocket(_addr->ai_family, _addr->ai_socktype, _addr->ai_protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (socket == INVALID_SOCKET) {
			throw SOCK_Exception(WSAGetLastError(), "Socket error");
		}

		socketRef.reset(new SOCKET(socket), SocketDeleter());
		inetAddrRef.reset(new INET_Addr(addr));
	}

	SocketHandle(SOCKET socket, INET_Addr &addr) :
		socketRef(new SOCKET(socket), SocketDeleter()),
		inetAddrRef(new INET_Addr(addr)) {

			INET_Addr::openSockets++;
	}

	int iResult;
	std::shared_ptr<SOCKET> socketRef;
	std::shared_ptr<INET_Addr> inetAddrRef;

};



