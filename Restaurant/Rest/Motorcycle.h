#ifndef __MOTORCYCLE_H_
#define __MOTORCYCLE_H_


#include "..\Defs.h"

#pragma once
class Motorcycle	
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding motorcycle type 
	int speed;		//meters it can move in one clock tick (in one timestep)
	REGION	region;	//region of the motorcycle
	STATUS	status;	//idle or in-service
	double FT;
public:
	Motorcycle(int s, int id, ORD_TYPE t, REGION r);
	virtual ~Motorcycle();
	int getSpeed() const;
	ORD_TYPE getType()const;
	int getID()const;
	REGION getRegion()const;
	STATUS getStatus()const;
	void setStatus(STATUS S);
	void setFinishTime(double ft);
	double getFinishTime();

};

#endif