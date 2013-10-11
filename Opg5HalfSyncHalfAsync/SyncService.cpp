#include "SyncService.h"
#include "Queue.h"
#include <thread>
#include <iostream>
#include "../Opg4AcceptorConnector/Dispatcher.h"


SyncService::SyncService()
{
	queue = Queue::Instance();
}


SyncService::~SyncService()
{
}

void task(string threadName)
{
	cout << "Thread " << threadName << " started processing" << endl;
	shared_ptr<Queue> queuePtr = Queue::Instance();

	while (true)
	{
		if (!queuePtr->Empty())
		{
			pair<int, shared_ptr<iEventHandler>> handlerPair = queuePtr->Dequeue();

			try{
				handlerPair.second->handleEvent();
				cout << "Event handled by thread " << threadName << endl << endl;

			}
			catch (SOCK_Exception &e){
				if (e.errorCode == 0)
				{
					Dispatcher dispatcher = Dispatcher();
					dispatcher.removeHandler(handlerPair.second.get(), handlerPair.first);
				}	
				throw e;
			}
		}

		Sleep(100);
	}
}

void SyncService::startProcessing()
{
	thread t1 = thread(task, "1");
	thread t2 = thread(task, "2");
	thread t3 = thread(task, "3");

	t1.detach();
	t2.detach();
	t3.detach();
}



