#pragma once
#include "PatientEventHandler.h"

#include <iostream>

using namespace std;

class PatientValueEventHandler :
	public PatientEventHandler
{
public:

	PatientValueEventHandler(){

	}

	void handleEvent(string data){
		cout << "Patient value event " << cnt << ": " << endl;
		cout << data.c_str() << endl;
		cout << endl;
		cnt++;
	}

	static const EVENT_TYPE getEventType(){
		return EVENT_PATIENT_VALUE;
	};

private:
	static int cnt;
};

