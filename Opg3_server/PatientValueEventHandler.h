#pragma once
#include "..\winsock2_wrapper\iEventHandler.h"

#include <iostream>

using namespace std;

class PatientValueEventHandler :
	public iEventHandler
{
public:

	PatientValueEventHandler()
	{
	}

	~PatientValueEventHandler()
	{
	}


	void handleEvent(string data){
		cout << "Patient value event " << cnt << ": " << endl;
		cout << data.c_str() << endl;
		cout << endl;
		cnt++;
	}

	void getHandle(){

	}

private:
	static int cnt;

};

