#include <map>
#include <list>
#include <memory>
#include <iostream>
#include <string>

#include "../Opg4AcceptorConnector/Acceptor.h"
#include "../Opg4AcceptorConnector/Dispatcher.h"

#include "PatientServiceHandlerServer.h"

using namespace std;

void main(){


	//initiate server

	//4: receives request and searches for patient info in a file

	//string CPR_number = "";

	//cin >> CPR_number;

	//string value = (*patientDatabase)[CPR_number];
	//
	//if (value == "") {
	//	cout << "Not found!" << endl;
	//	return;
	//}

	//cout << value << endl;

	std::shared_ptr<Reactor> dispatcher = make_shared<Dispatcher>();

	auto accepter = make_shared < Acceptor<PatientServiceHandlerServer>>(dispatcher, EventType::PATIENTINFOEVENT);

	dispatcher->registerHandler(accepter, EventType::ACCEPTOR);

	dispatcher->handleEvents();
	//5: respond with PatientInfo

}
