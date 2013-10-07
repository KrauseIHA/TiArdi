
#include <iostream>
#include <conio.h>
#include <atlbase.h>

#include "../winsock2_wrapper/SOCK_Acceptor.h"

//#include <WinBase.h>

using namespace std;

int main(){
	try{
		int ret = 1;
		int port = 9952;

		SOCK_Acceptor connection(port);
		cout << "Waiting for connection on port: " << port << endl;

		connection.accept();

		cout << "Incomming connection." << endl;

		string recived;
		while (ret == 1)
		{
			recived = connection.recive();

			if (recived.length() <= 0)
				break;
			else{
				cout << "Recived: ";

				cout << recived << endl;

			}
		}

		Sleep(2000);

		return ret;
	}
	catch (SOCK_Exception &e){
		if (e.errorCode != 0){
			e.displayError();
			DebugBreak();
		}
		else{
			printf(e.message);
		}
	}
}


