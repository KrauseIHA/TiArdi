#pragma once
#include "../Opg4AcceptorConnector/ServiceHandler.h"

class PatientServiceHandlerClient :public ServiceHandler
{
public:

	PatientServiceHandlerClient()
	{
	}

	PatientServiceHandlerClient(std::shared_ptr<Reactor> _dispatcher) : ServiceHandler(_dispatcher)	{

	}

	void handleEvent() {
		sockStream.send("flaf from client");
	}

	void open() {
		dispatcher->registerHandler(make_shared<PatientServiceHandlerClient>(*this), EventType::PATIENTINFOEVENT);
	}

	EventType getType() {
		return EventType::PATIENTINFOEVENT;
	}
};

