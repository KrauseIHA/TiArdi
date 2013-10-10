#pragma once

#include "Queue.h"

class SyncService
{
public:
	SyncService();
	~SyncService();
	void startProcessing();
private:
	shared_ptr<Queue> queue;
};

