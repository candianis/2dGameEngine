#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "../Logger/Logger.h"

class EventBus {
public:
	EventBus() {
		Logger::Log("EventBus constructor called!");
	}
	~EventBus() {
		Logger::Log("EventBus destructor called!");
	}

	//Subscribe OR listen to an event <T>


	//Emit an event of <T> and execute all the listener callback functions


};

#endif // !EVENTBUS_H
