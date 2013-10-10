#pragma once

#include "SocketHandle.h"


class TransportHandle : SocketHandle

{
public:

	void static select(std::list<SocketHandle> &read, std::list<SocketHandle> &write, std::list<SocketHandle> &error, u_long timeout_usec){
		SocketHandle::select(read, write, error, timeout_usec);
	}

	void static select(std::list<SocketHandle> &read, std::list<SocketHandle> &write, std::list<SocketHandle> &error){
		SocketHandle::select(read, write, error);
	}


private:

};

