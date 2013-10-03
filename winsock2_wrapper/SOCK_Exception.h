#pragma once

#include <exception>
#include <string>

class SOCK_Exception :
	public std::exception
{
public:
	const char * message;
	int errorCode;

	SOCK_Exception(const int _errorCode, const char * message = ""){
		this->message = message;
		this->errorCode = _errorCode;

		// Uh oh!  Something bad happened.  Let's
		// get the error code...

		// ..and the human readable error string!!
		// Interesting:  Also retrievable by net helpmsg 10060
		LPWSTR errString = NULL;  // will be allocated and filled by FormatMessage

		if (this->errorCode != 0){

			int size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM, // use windows internal message table
				0,       // 0 since source is internal message table
				errorCode, // this is the error code returned by WSAGetLastError()
				0,       // auto-determine language to use
				(LPWSTR)&errString, // this is WHERE we want FormatMessage to plunk the error string.
				0,                 // min size for buffer
				0);               // 0, since getting message from system tables
			//printf("Error code %d:  %s\n\nMessage was %d bytes, in case you cared to know this.\n\n", errCode, errString, size);

			errorString.assign(errString);

			LocalFree(errString); // if you don't do this, you will get an
			// ever so slight memory leak, since we asked
			// FormatMessage to FORMAT_MESSAGE_ALLOCATE_BUFFER,
			// and it does so using LocalAlloc
			// Gotcha!  I guess.

		}


	}

	void displayError(){
		const int MAX_LENGTH = 50;
		int size = MultiByteToWideChar(CP_ACP, 0, message, -1, NULL, 0);

		PWSTR wChar = (PWSTR)HeapAlloc(GetProcessHeap(), 0, size * sizeof(wchar_t));
		MultiByteToWideChar(0, 0, message, -1, wChar, size);

		std::wstring myStr(wChar);

		HeapFree(GetProcessHeap(), 0, wChar);

		MessageBox(NULL,
			(myStr + L"\n\r" + std::to_wstring(errorCode) + L": " + errorString).c_str(),
			L"Uncourght exception",
			MB_OK | MB_ICONWARNING
			);

	}


	~SOCK_Exception(){

	}
private:
	std::wstring errorString;
};

