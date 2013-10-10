#include "SyncService.h"
#include "Queue.h"
#include <thread>
#include <iostream>


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
			shared_ptr<iEventHandler> handlerPtr = queuePtr->Dequeue();
			handlerPtr->handleEvent();
			cout << "Event handled by thread " << threadName << endl << endl;
		}

		_sleep(100);
	}
}

void SyncService::startProcessing()
{
	thread t1 = thread(task, "1");
	thread t2 = thread(task, "2");

	t1.detach();
	t2.detach();
}



