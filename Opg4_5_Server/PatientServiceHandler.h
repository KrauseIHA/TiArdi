#pragma once
#include "../Opg4AcceptorConnector/ServiceHandler.h"
#include <iostream>

class PatientServiceHandler : ServiceHandler
{
public:

	PatientServiceHandler()
	{
	}

	~PatientServiceHandler()
	{
	}

	void handleEvent()
	{
		cout << "String recived from client: " << sockStream.recive() << endl;
	}

	void open()
	{
		dispatcher->registerHandler(make_shared<PatientServiceHandler>(this), EventType::PATIENTINFOEVENT);
	}
};

