#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include "iTaskHandler.h"


using namespace std;

class Queue
{
public:
	static shared_ptr<Queue> Instance();
	void Enqueue(iTaskHandler* eh, void *message);
	pair<iTaskHandler *, void *> Dequeue();
	bool Empty();
private:
	Queue() { };
	static shared_ptr<Queue> _instance;
	queue<pair<iTaskHandler *, void *>> _queue;
	mutex _sm;
};
