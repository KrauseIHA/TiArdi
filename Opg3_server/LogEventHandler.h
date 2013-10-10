#pragma once
#include "PatientEventHandler.h"

#include <iostream>

using namespace std;

class LogEventHandler :
	public PatientEventHandler
{
public:

	LogEventHandler(){
	}

	void handleEvent(string data){
		cout << "Log event " << cnt << ": " << endl;
		cout << data.c_str() << endl;
		cout << endl;
		cnt++;
	}

	static const EVENT_TYPE getEventType(){
		return EVENT_LOG;
	};


private:
	static int cnt;
};

