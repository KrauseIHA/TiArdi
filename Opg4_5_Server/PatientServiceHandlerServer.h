#pragma once
#include "../Opg4AcceptorConnector/ServiceHandler.h"
#include <iostream>
#include <map>
#include "../Opg5HalfSyncHalfAsync/Queue.h"
#include "../Opg5HalfSyncHalfAsync/iTaskHandler.h"

class PatientServiceHandlerServer : public ServiceHandler, public iTaskHandler
{
public:

	PatientServiceHandlerServer(std::shared_ptr<Reactor> _dispatcher) : ServiceHandler(_dispatcher)	{
		patientDatabase = make_shared <map<string, string>>();

		pair<string, string> patient1("123456-1111", "Patient 1 info");
		pair<string, string> patient2("123456-1222", "Patient 2 info");
		pair<string, string> patient3("123456-1333", "Patient 3 info");
		pair<string, string> patient4("123456-1444", "Patient 4 info");
		pair<string, string> patient5("123456-1555", "Patient 5 info");
		pair<string, string> patient6("123456-1666", "Patient 6 info");

		patientDatabase->insert(patient1);
		patientDatabase->insert(patient2);
		patientDatabase->insert(patient3);
		patientDatabase->insert(patient4);
		patientDatabase->insert(patient5);
		patientDatabase->insert(patient6);
	}

	void runTask(void *data){
		string cprNr(*((string*)data));
		delete data;

		string patientInfo = (*patientDatabase)[cprNr];

		cout << "String recived from client: " << cprNr << endl;

		cout << "Sending response: " << patientInfo << endl;

		sockStream.send(patientInfo);

	}

	void handleEvent(string cprNr)
	{
		(*PatientServiceHandlerServer::queue).Enqueue(this, new string(cprNr));
	}

	void open()
	{
		dispatcher->registerHandler(make_shared<PatientServiceHandlerServer>(*this), EventType::PATIENTINFOEVENT);
	}

	EventType getType() {
		return EventType::PATIENTINFOEVENT;
	}

private:
	shared_ptr<map<string, string>> patientDatabase;
	static shared_ptr<Queue> queue;

};

