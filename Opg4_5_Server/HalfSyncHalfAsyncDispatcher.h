#pragma once
#include "..\Opg4AcceptorConnector\Dispatcher.h"
#include "..\Opg5HalfSyncHalfAsync\Queue.h"

class HalfSyncHalfAsyncDispatcher :
	public Dispatcher
{
public:

	HalfSyncHalfAsyncDispatcher()
	{
	}

	void callHandelevent(pair < int, shared_ptr < iEventHandler >> eventHandler){
		shared_ptr<Queue> queue = Queue::Instance();
		queue->Enqueue(eventHandler);
	}
};

