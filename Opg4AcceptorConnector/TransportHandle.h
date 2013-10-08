#pragma once

#include "TransportEndpoint.h"

#include <memory>

class TransportHandle
{
public:

	TransportHandle(){
	}

	~TransportHandle(){

	}

private:

	std::unique_ptr<TransportEndpoint> transportEndpoint;

};

