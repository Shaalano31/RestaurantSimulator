#include"PromoteEvent.h"

PromoteEvent::PromoteEvent(int eTime,int ordID,int Ex):Event(eTime,ordID)
{ Extra=Ex;
}
void PromoteEvent::Execute(Restaurant*pRest)
{ 
	pRest->promotion(OrderID,Extra);
}

PromoteEvent::~PromoteEvent()
{
}