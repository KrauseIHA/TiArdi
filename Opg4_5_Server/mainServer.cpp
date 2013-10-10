#include <map>
#include <list>
#include <memory>
#include <iostream>
#include <string>

using namespace std;

void main(){
	auto patientDatabase = make_shared <map<string, string>>();

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

	//initiate server

	//4: receives request and searches for patient info in a file

	string CPR_number = "";

	cin >> CPR_number;

	string value = (*patientDatabase)[CPR_number];
	
	if (value == "") {
		cout << "Not found!" << endl;
		return;
	}

	cout << value << endl;

	//5: respond with PatientInfo

}
