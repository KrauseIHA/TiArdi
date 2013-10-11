#pragma once
#include "../Opg4AcceptorConnector/ServiceHandler.h"
#include <time.h>
#include <string>

#include <iostream>

class PatientServiceHandlerClient :public ServiceHandler
{
public:

	PatientServiceHandlerClient(std::shared_ptr<Reactor> _dispatcher) : ServiceHandler(_dispatcher)	{

	}

	void handleEvent(string data) {
		std::cout << data << std::endl;
		end = clock();
		cout << "Time elapsed: " << diffclock(end, begin) << " ms" << endl;
	}

	void open() {
		string cpr_nr;
		cin >> cpr_nr;

		begin = clock();
		dispatcher->registerHandler(make_shared<PatientServiceHandlerClient>(*this), EventType::PATIENTINFOEVENT);
		sockStream.send(cpr_nr);
	}

	EventType getType() {
		return EventType::PATIENTINFOEVENT;
	}

private:
	double diffclock(clock_t clock1, clock_t clock2)
	{
		double diffticks = clock1 - clock2;
		double diffms = (diffticks * 1000) / CLOCKS_PER_SEC;
		return diffms;
	}
	clock_t end;
	clock_t begin;
};

