#include "CancelEvent.h"



CancelEvent::CancelEvent(int eTime, int oID) : Event(eTime, oID)
{
}

void CancelEvent::Execute(Restaurant *pRest)
{
	pRest->cancelation(OrderID);
	pRest->getGUI()->RemoveOrder(OrderID);
}
