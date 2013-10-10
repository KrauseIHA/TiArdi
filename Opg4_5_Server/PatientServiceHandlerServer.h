#pragma once
#include "../Opg4AcceptorConnector/ServiceHandler.h"
#include <iostream>

class PatientServiceHandlerServer : public ServiceHandler
{
public:

	PatientServiceHandlerServer(){

	}

	PatientServiceHandlerServer(std::shared_ptr<Reactor> _dispatcher) : ServiceHandler(_dispatcher)	{

	}


	void handleEvent()
	{
		cout << "String recived from client: " << sockStream.recive() << endl;
	}

	void open()
	{
		dispatcher->registerHandler(make_shared<PatientServiceHandlerServer>(*this), EventType::PATIENTINFOEVENT);
	}

	EventType getType() {
		return EventType::PATIENTINFOEVENT;
	}
};

