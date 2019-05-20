#pragma once
#include "Events/Event.h"
#include "Rest/Restaurant.h"

class CancelEvent : public Event
{
public:
	CancelEvent(int eTime, int oID);
	void Execute(Restaurant *pRest);
};

