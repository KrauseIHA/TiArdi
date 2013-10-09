#include "..\Opg5HalfSyncHalfAsync\Queue.h"
#include "..\Opg5HalfSyncHalfAsync\SyncService.h"
#include "..\Opg3_server\AlarmEventHandler.cpp"

int main()
{
	SyncService service = SyncService();
	service.startProcessing();

	shared_ptr<Queue> queuePtr = Queue::Instance();

	shared_ptr<AlarmEventHandler> handlerPtr1 = shared_ptr<AlarmEventHandler>();
	handlerPtr1.reset(new AlarmEventHandler);

	shared_ptr<AlarmEventHandler> handlerPtr2 = shared_ptr<AlarmEventHandler>();
	handlerPtr2.reset(new AlarmEventHandler);

	queuePtr->Enqueue(handlerPtr1);
	queuePtr->Enqueue(handlerPtr2);

	while (true) {}
}