#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"
#include "Motorcycle.h"
class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, Frozen, VIP
	REGION Region;  //Region of this order
	int Distance;	//The distance (in meters) between the order location and the resturant 
	                
	double totalMoney;	//Total order money

	int ArrTime, ServTime;	//arrival, service starts
	double Priority;
	int waitTime;
	void PriorityEquation();
	double FinishTime;

	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order(int id, ORD_TYPE r_Type, REGION r_region, double totalM, int ArrT, int Dis);
	Order();
	Order(Order* & pOrd);
	virtual ~Order();
	int getArrTime();

	int GetID();
	void SetID(int id);

	void SetServTime(int t);
	void setarrtime(int t);
	int getServTime();

	int GetType() const;
	REGION GetRegion() const;
	void SetDistance(int d);
	int GetDistance() const;

	double GetPriority();
	bool operator==(int id);
	void setmoney(int mon);
	void setordertype(ORD_TYPE Type);
	double getMoney();
	void setFT(int ft);
	int getFT();


	int getwaittime();
	void setWaitTime(int w);
	// TODO: Add More Member Functions As Needed
	//

};

#endif