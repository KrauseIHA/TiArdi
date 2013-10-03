

#include "../winsock2_wrapper/SOCK_Connector.h"
#include "../Opg3_server/PatientMoniterReactor.h"

#include <conio.h>
#include <iostream>

using namespace std;

string wrapPackage(Event_Type type, std::string data){
	char _type[3];
	_type[0] = ('\1');
	_type[1] = ((char)type);
	_type[2] = '\0';
	string rtn = _type + data + '\1';
	const char* _rtn = rtn.c_str();
	return _rtn;
}

int main(){

	try{
		const u_int NUMBER_OF_CLIENTS = 4;

		string address = "localhost";

		cout << "Input server address: ";
		cin >> address;
		cout << endl;

		SOCK_Connector *connection;
		SOCK_Connector *connections[NUMBER_OF_CLIENTS];
		//Stress test

		for (int i = 0; i < NUMBER_OF_CLIENTS; i++){
			connections[i] = new SOCK_Connector(address.c_str(), 50400);
		}

		for (int i = 0; i < NUMBER_OF_CLIENTS; i++){
			string dataToBeSent = "my data package went through yeah!";
			string patientValue = wrapPackage(EVENT_PATIENT_VALUE, dataToBeSent + to_string(i + 1));
			connections[i]->send(patientValue.c_str());
		}
		for (int i = 0; i < NUMBER_OF_CLIENTS; i++){
			delete connections[i];
		}

		cout << "Press any key to start test 2" << endl;
		getch();

		for (int i = 0; i < NUMBER_OF_CLIENTS; i++){
			string dataToBeSent = "my data package went through yeah!";
			connection = new SOCK_Connector(address.c_str(), 50400);

			string alarmMsg = wrapPackage(EVENT_ALARM, dataToBeSent + to_string(i + 1));
			connection->send(alarmMsg.c_str());

			string logMsg = wrapPackage(EVENT_LOG, dataToBeSent + to_string(i + 1));
			connection->send(logMsg.c_str());


			string patientValue = wrapPackage(EVENT_PATIENT_VALUE, dataToBeSent + to_string(i + 1));
			connection->send(patientValue.c_str());
			delete connection;
		}

	}
	catch (SOCK_Exception &e){
		e.displayError();
	}
	cout << "Press any key to exit" << endl;
	getch();


}