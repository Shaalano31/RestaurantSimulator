#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type, REGION r_region, double totalM, int ArrT, int Dis)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	totalMoney = totalM;
	ArrTime = ArrT;
	Distance = Dis;
	type = r_Type;
	Region = r_region;
	PriorityEquation();
	waitTime=0;
	FinishTime=0;
}

Order::Order()
{}

Order::Order(Order*& pOrd) {
	pOrd->setFT(FinishTime);
	pOrd->SetDistance(Distance);
	pOrd->SetServTime(ServTime);
	pOrd->setWaitTime(waitTime);
	pOrd->setordertype(type);
	pOrd->setarrtime(ArrTime);
}
void Order::setarrtime(int t) {
	ArrTime = t;
}


void Order::setmoney(int mon)
{  totalMoney=totalMoney+mon;
}
void Order::setordertype(ORD_TYPE Type)
{ type=Type;
}

Order::~Order()
{
}

void Order::SetID(int id)
{
	ID = id;
}

int Order::GetID()
{
	return ID;
}


int Order::GetType() const
{
	return type;
}

REGION Order::GetRegion() const
{
	return Region;
}

void Order::SetDistance(int d)
{
	Distance = d>0?d:0;
}

int Order::GetDistance() const
{
	return Distance;
}

double Order::GetPriority()
{
	return Priority;
}

void Order::PriorityEquation()
{
	if (type == TYPE_VIP)
		Priority = (double)totalMoney / ((double)Distance* (double)ArrTime);
	else
		Priority = 0;
}

bool Order::operator==(int id)
{
	if (this->ID == id)
	{
		return true;
	}
	else
	{
		return false;
	}
}



int Order:: getFT ()
{
	return FinishTime;
}



int Order:: getArrTime()
{
	return ArrTime;
}
double Order::getMoney()
{
	return totalMoney;
}




void Order:: setFT(int ft)
{
	FinishTime=ft;
}
void Order:: SetServTime(int t)
{
	ServTime=t;
}
int Order::getServTime()
{
	return ServTime;
}

void Order:: setWaitTime(int w)
{
	waitTime=w;
}
int Order::getwaittime()
{
	return waitTime;
}