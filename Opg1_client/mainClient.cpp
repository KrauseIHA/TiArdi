

#include <iostream>
#include <conio.h>

#include "../winsock2_wrapper/SOCK_Connector.h"

using namespace std;

int main(){
	try{

		int port = 9952;
		char ip[]= "127.0.0.1";
		char testString[] = "Test string";

		cout << "Ready to open connections." <<endl;
		getch();

		SOCK_Connector *connection = new SOCK_Connector(ip, port);
		cout << "conncted to " << ip << " on port " << port << endl;
		getch();

		cout << "sending String : " << testString << endl;
		connection->send(testString);

		cout << "Press any key to close connection"<< endl;
		getch();
		delete connection;
	}catch(SOCK_Exception &e){
		//e.displayError();
		DebugBreak();
	}
}

