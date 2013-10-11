#include <map>
#include <list>
#include <memory>
#include <iostream>
#include <string>

#include "../Opg4AcceptorConnector/Acceptor.h"
#include "../Opg5HalfSyncHalfAsync/SyncService.h"

#include "PatientServiceHandlerServer.h"

using namespace std;

void main(){

	cout << "hej fra server" << endl;

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
	SyncService service = SyncService();
	service.startProcessing();

	try{
		auto dispatcher = make_shared<Dispatcher>();

		auto accepter = make_shared < Acceptor<PatientServiceHandlerServer>>(dispatcher, EventType::PATIENTINFOEVENT);

		dispatcher->handleEvents();
		//5: respond with PatientInfo
	}
	catch (SOCK_Exception &e){
		e.displayError();
	}
}
