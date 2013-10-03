#pragma once
#include "..\winsock2_wrapper\iEventHandler.h"

#include <iostream>

using namespace std;

class LogEventHandler :
	public iEventHandler
{
public:

	LogEventHandler()
	{
	}

	~LogEventHandler()
	{
	}

	void handleEvent(string data){
		cout << "Log event " << cnt << ": " << endl;
		cout << data.c_str() << endl;
		cout << endl;
		cnt++;
	}

	void getHandle(){

	}

private:
	static int cnt;
};

