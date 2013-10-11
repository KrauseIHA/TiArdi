#pragma once

#include <queue>
#include "..\winsock2_wrapper\iEventHandler.h"
#include <memory>
#include <mutex>

using namespace std;

class Queue
{
public:
	static shared_ptr<Queue> Instance();
	void Enqueue(pair<int, shared_ptr<iEventHandler>> eh);
	pair<int, shared_ptr<iEventHandler>> Dequeue();
	bool Empty();
private:
	Queue() { };
	static shared_ptr<Queue> _instance;
	queue<pair<int, shared_ptr<iEventHandler>>> _queue;
	mutex _sm;
};
