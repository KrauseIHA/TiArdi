#include "../Opg4AcceptorConnector/Connector.h"
#include "PatientServiceHandlerClient.h"
#include "../Opg4AcceptorConnector/EventType.h"
#include <conio.h>

int main(){

	cout << "hej fra client" << endl;

	string address = "localhost";

	cout << "input server address (press enter for localhost): ";
	int firstChar = _getch();

	if (firstChar != 13){
		cout << (char)firstChar;
		cin >> address;
		address = (char) firstChar + address;
		cout << endl;
	}
	else{
		cout << address << endl;
	}
	//1: Open socket connection to server

	try{
		auto dispatcher = make_shared<Dispatcher>();

		Connector<PatientServiceHandlerClient> connector(address.c_str(), EventType::PATIENTINFOEVENT, dispatcher);

		connector.connect(true);

		dispatcher->handleEvents();
	}
	catch (SOCK_Exception &e){
		e.displayError();
	}
	//2: Client inputs CPR number on the client PC

	//3: Send GetPatientInfo(CPR number) request to server via TCP/IP

	//wait for responce

	//6: receives responce and displays received record

	//must be possible to enter a new CPR number (repeat step 2-6).
	//see server for 4 & 5.

	//close application
}