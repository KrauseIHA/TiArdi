#include "Queue.h"

shared_ptr<Queue> Queue::Instance()
{
	if (_instance.use_count() == 0)
	{
		_instance.reset(new Queue);
	}
	return _instance;
}

void Queue::Enqueue(shared_ptr<iEventHandler> eh)
{
	_sm.lock();
	_queue.push(eh);
	_sm.unlock();
}

shared_ptr<iEventHandler> Queue::Dequeue()
{
	_sm.lock();
	auto popped = _queue.front();
	_queue.pop();
	_sm.unlock();
	return popped;
}