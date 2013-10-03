#pragma once
#include "..\winsock2_wrapper\iEventHandler.h"

#include <iostream>

using namespace std;

class AlarmEventHandler :
	public iEventHandler
{
public:

	AlarmEventHandler()
	{
	}

	~AlarmEventHandler()
	{
	}

	void handleEvent(string data){
		cout << "Alarm event " << cnt << ": " << endl;
		cout << data.c_str() << endl;
		cout << endl;
		cnt++;
	}

	void getHandle(){

	}

private:
	static int cnt;
};

