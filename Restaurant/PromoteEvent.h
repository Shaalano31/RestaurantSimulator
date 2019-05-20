#pragma once
#include "Events\Event.h"
#include "Rest\Restaurant.h"

class PromoteEvent : public Event
{
private:
	int Extra;
public:
	PromoteEvent(int eTime, int ordID, int Ex);
	void Execute(Restaurant *pRest);
	~PromoteEvent();
};

