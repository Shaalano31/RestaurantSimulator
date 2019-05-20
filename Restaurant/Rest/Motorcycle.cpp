#include "Motorcycle.h"


Motorcycle::Motorcycle(int s, int id, ORD_TYPE t, REGION r)
{
	ID = id;
	speed = s;
	type = t;
	region = r;
	status = IDLE;
}


Motorcycle::~Motorcycle()
{
}
int Motorcycle::getSpeed() const
{
	return speed;
}
int Motorcycle::getID() const
{
	return ID;
}
REGION Motorcycle::getRegion()const
{
	return region;
}
ORD_TYPE Motorcycle:: getType()const
{
	return type;
}
STATUS Motorcycle:: getStatus()const
{
	return status;
}
void Motorcycle:: setStatus(STATUS S)
{
	status=S;
}
void Motorcycle :: setFinishTime(double ft)
{
	FT=ft;
}
double Motorcycle:: getFinishTime()
{
	return FT;
}