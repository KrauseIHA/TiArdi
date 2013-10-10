#pragma once

#include "PatientEventHandler.h"

#include <iostream>

using namespace std;

class AlarmEventHandler :
	public PatientEventHandler
{
public:


	void handleEvent(string data){
		cout << "Alarm event " << cnt << ": " << endl;
		cout << data.c_str() << endl;
		cout << endl;
		cnt++;
	}

	const static EVENT_TYPE getEventType(){
		return EVENT_ALARM;
	};


private:
	static int cnt;
};

