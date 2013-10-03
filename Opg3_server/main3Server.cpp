

#include "PatientMoniterReactor.h"

#include "AlarmEventHandler.h"
#include "LogEventHandler.h"
#include "PatientValueEventHandler.h"
#include "callbackEventHandler.h"

#include <conio.h>
#include <iostream>
#include <string>

using namespace std;

static int logCnt = 1;

void logCallback(std::string data){
	cout << "Log event " << logCnt << ": " << endl;
	cout << data << endl;
	cout << endl;
	logCnt++;
}

int main(){
	try{
		PatientMoniterReactor reactor(50400);

		
		reactor.registerHandler((iEventHandler*)new AlarmEventHandler, EVENT_ALARM);
		reactor.registerHandler((iEventHandler*)new PatientValueEventHandler, EVENT_PATIENT_VALUE);
		//reactor.registerHandler((iEventHandler*)new LogEventHandler, EVENT_LOG);

		reactor.registerHandler((iEventHandler*)new callbackEventHandler(logCallback), EVENT_LOG);
		
		reactor.handleEvents();

	}
	catch (SOCK_Exception &e){
		e.displayError();
	}

}


