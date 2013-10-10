#pragma once
#include "../Opg4AcceptorConnector/ServiceHandler.h"

#include <iostream>

class PatientServiceHandlerClient :public ServiceHandler
{
public:

	PatientServiceHandlerClient(std::shared_ptr<Reactor> _dispatcher) : ServiceHandler(_dispatcher)	{

	}

	void handleEvent() {

		std::cout << sockStream.recive() << std::endl;
	}

	void open() {
		dispatcher->registerHandler(make_shared<PatientServiceHandlerClient>(*this), EventType::PATIENTINFOEVENT);
		sockStream.send("123456-1111");
	}

	EventType getType() {
		return EventType::PATIENTINFOEVENT;
	}
};

