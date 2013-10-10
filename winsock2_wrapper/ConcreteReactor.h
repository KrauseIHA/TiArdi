#pragma once
#include "Reactor.h"
#include "SOCK_Acceptor.h"

#include <string>
#include <list>

using namespace std;

class ConcreateReactor : public Reactor
{
public:

	void handleEvents(){
		bool running = true;

		while (running){
			running = internalHandleEvent();
		}

	}


private:

	shared_ptr < list < shared_ptr < SocketHandle >> > createListOfSockets(){
		auto output = make_shared < list < shared_ptr < SocketHandle >> >();

		auto it = registeredHandlers.begin();

		if (registeredHandlers.size() == 0){
			return output;
		}

		do{
			output->push_front(it->second->getHandle());
		} while (++it != registeredHandlers.end());

		return output;
	}

	bool internalHandleEvent(){
		auto readSockets = createListOfSockets();

		SocketHandle::select(*(readSockets.get()), emptylist, emptylist);

		if (readSockets->size() == 0)
			return true;

		auto itSockets = readSockets->begin();

		do{

			auto itHandlers = registeredHandlers.begin();
			do{
				if ((*itSockets) == itHandlers->second->getHandle()){
					try{
						itHandlers->second->handleEvent();
					}
					catch (SOCK_Exception &e){
						if (e.errorCode == 0)
							registeredHandlers.erase(itHandlers--);
					}

				}
			} while (++itHandlers != registeredHandlers.end());

		} while (++itSockets != readSockets->end());

		return true;

	}

	std::list<std::shared_ptr<SocketHandle>> emptylist;

};

