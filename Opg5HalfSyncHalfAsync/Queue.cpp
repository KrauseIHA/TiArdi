#include "Queue.h"

shared_ptr<Queue> Queue::Instance()
{
	static shared_ptr<Queue> _instance = shared_ptr<Queue>();

	if (_instance.use_count() == 0)
	{
		_instance.reset(new Queue);
	}
	return _instance;
}

void Queue::Enqueue(iTaskHandler *eh, void *message)
{
	_sm.lock();
	pair < iTaskHandler*, void*> element(eh, message);
	_queue.push(element);
	_sm.unlock();
}

pair<iTaskHandler *, void*> Queue::Dequeue()
{
	_sm.lock();
	if (_queue.empty()){
		_sm.unlock();
		return pair<iTaskHandler *, void*>(NULL, NULL);
	}

	auto popped = _queue.front();
	_queue.pop();
	_sm.unlock();
	return popped;
}

bool Queue::Empty()
{
	return _queue.empty();
}