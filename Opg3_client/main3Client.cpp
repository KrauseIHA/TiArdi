

#include "../winsock2_wrapper/SOCK_Connector.h"
#include "../Opg3_server/PatientMoniterReactor.h"

#include <conio.h>
#include <iostream>
#include <map>


using namespace std;

string wrapPackage(EVENT_TYPE type, std::string data){
	//char _type[3];
	//_type[0] = ('\1');
	//_type[1] = ((char)type);
	//_type[2] = '\0';
	//string rtn = _type + data + '\1';
	//const char* _rtn = rtn.c_str();
	//return _rtn;
	return data;
}

int main(){
	const u_int NUMBER_OF_CLIENTS = 4;

	string address;

	cout << "Input server address: ";
	//cin >> address;
	cout << endl;

	if (address.length() == 0)
		address = "localhost";

	//auto connection = make_shared<SOCK_Connector>(address.c_str(), EVENT_ALARM);

	//string dataToBeSent = "my data package went through yeah!";
	//string patientValue = wrapPackage(EVENT_PATIENT_VALUE, dataToBeSent);
	//connection->send(patientValue.c_str());

	//return 1;
	shared_ptr<SOCK_Connector> connection[3];
	shared_ptr<SOCK_Connector> connections[3][NUMBER_OF_CLIENTS];

	try{
		//Stress test

		for (int i = 0; i < NUMBER_OF_CLIENTS; i++){
			connections[0][i] = make_shared<SOCK_Connector>(address.c_str(), EVENT_ALARM);
			connections[1][i] = make_shared<SOCK_Connector>(address.c_str(), EVENT_LOG);
			connections[2][i] = make_shared<SOCK_Connector>(address.c_str(), EVENT_PATIENT_VALUE);
		}

		for (int i = 0; i < NUMBER_OF_CLIENTS; i++){
			string dataToBeSent = "my data package went through yeah!";
			string patientValue = dataToBeSent + to_string(i + 1);
			connections[0][i]->send(patientValue.c_str());
			connections[1][i]->send(patientValue.c_str());
			connections[2][i]->send(patientValue.c_str());
		}


		cout << "Press any key to start test 2" << endl;
		_getch();

		for (int i = 0; i < NUMBER_OF_CLIENTS; i++){
			string dataToBeSent = "my data package went through yeah!";
			connection[0] = make_shared<SOCK_Connector>(address.c_str(), EVENT_ALARM);
			connection[1] = make_shared<SOCK_Connector>(address.c_str(), EVENT_LOG);
			connection[2] = make_shared<SOCK_Connector>(address.c_str(), EVENT_PATIENT_VALUE);

			string msg = dataToBeSent + to_string(i + 1);

			connection[0]->send(msg.c_str());
			connection[1]->send(msg.c_str());
			connection[2]->send(msg.c_str());
		}

	}
	catch (SOCK_Exception &e){
		e.displayError();
	}
	cout << "Press any key to exit" << endl;
	_getch();


}