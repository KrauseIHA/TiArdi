#pragma once
#include "../Opg4AcceptorConnector/ServiceHandler.h"

class PatientServiceHandler : ServiceHandler
{
public:

	PatientServiceHandler()
	{
	}

	~PatientServiceHandler()
	{
	}

	void handleEvent() {
		sockStream.send("flaf from client");
	}

	void open() {
		dispatcher->registerHandler(make_shared<PatientServiceHandler>(this), EventType::PATIENTINFOEVENT);
	}

	EventType getType() {
		return EventType::PATIENTINFOEVENT;
	}
};

