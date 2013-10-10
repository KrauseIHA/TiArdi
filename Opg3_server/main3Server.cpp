

#include "../winsock2_wrapper/ConcreteReactor.h"

#include "AcceptEventHandler.h"
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
		//PatientMoniterReactor reactor(50400);
		auto reactor = std::make_shared<ConcreateReactor>();

		auto alarmEvent = std::make_shared<AcceptEventHandler<AlarmEventHandler>>(reactor);
		auto logEvent = std::make_shared <AcceptEventHandler<LogEventHandler>>(reactor);
		auto valueEvent = std::make_shared <AcceptEventHandler<PatientValueEventHandler>>(reactor);

		reactor->registerHandler(alarmEvent, ACCEPTOR);
		reactor->registerHandler(logEvent, EVENT_ALARM);
		reactor->registerHandler(valueEvent, EVENT_PATIENT_VALUE);
		//reactor.registerHandler((iEventHandler*)new LogEventHandler, EVENT_LOG);

		//reactor.registerHandler(new callbackEventHandler(logCallback), EVENT_LOG);
		
		reactor->handleEvents();

	}
	catch (SOCK_Exception &e){
		e.displayError();
	}

}


