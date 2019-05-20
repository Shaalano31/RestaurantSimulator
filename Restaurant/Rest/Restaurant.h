#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\List.h"
#include "..\SortedList.h"
#include "..\Events\Event.h"
#include <fstream>
#include "Motorcycle.h"
#include "Order.h"
#include "..\CancelEvent.h"
#include "..\PromoteEvent.h"

// it is the maestro of the project
class Restaurant  
{	
private:

	GUI *pGUI;
	ifstream InputFile;
	ofstream OutputFile;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	SortedList<Order*> A_VIP;   /// ==> 
	SortedList<Order*> B_VIP;   /// ==> 
	SortedList<Order*> C_VIP;  /// ==> 
	SortedList<Order*> D_VIP; /// ==> 
	Queue<Order*> A_Frozen;
	Queue<Order*> B_Frozen;
	Queue<Order*> C_Frozen;
	Queue<Order*> D_Frozen;
	List<Order*> A_Normal;
	List<Order*> B_Normal;
	List<Order*> C_Normal;
	List<Order*> D_Normal;
	// Memebers for printing

	int CurrentTimeStep = 1;
	int NormalA , VIPA , FrozenA ;
	int NormalB , VIPB , FrozenB ;
	int NormalC , VIPC , FrozenC ;
	int NormalD , VIPD , FrozenD ;

	int ActiveAN , ActiveAV , ActiveAF ;
	int ActiveBN , ActiveBV , ActiveBF ;
	int ActiveCN , ActiveCV , ActiveCF ;
	int ActiveDN , ActiveDV , ActiveDF ;
	int AutoPromotion;

	// Fast Motorcycles
	SortedList<Motorcycle*> A_Fast_Motorcycles;
	SortedList<Motorcycle*> B_Fast_Motorcycles;
	SortedList<Motorcycle*> C_Fast_Motorcycles;
	SortedList<Motorcycle*> D_Fast_Motorcycles;
	// Normal Motorcycles
	SortedList<Motorcycle*> A_Normal_Motorcycles;
	SortedList<Motorcycle*> B_Normal_Motorcycles;
	SortedList<Motorcycle*> C_Normal_Motorcycles;
	SortedList<Motorcycle*> D_Normal_Motorcycles;
	// Frozen Motorcycles
	SortedList<Motorcycle*> A_Frozen_Motorcycles;
	SortedList<Motorcycle*> B_Frozen_Motorcycles;
	SortedList<Motorcycle*> C_Frozen_Motorcycles;
	SortedList<Motorcycle*> D_Frozen_Motorcycles;


	SortedList<Order*> AServList;
	SortedList<Order*> BServList;
	SortedList<Order*> CServList;
	SortedList<Order*> DServList;

	int ServAN , ServAV , ServAF ;
	int ServBN , ServBV , ServBF ;
	int ServCN , ServCV , ServCF ;
	int ServDN , ServDV , ServDF ;
	
	int MotorAN, MotorAF, MotorAV;
	int MotorBN, MotorBF, MotorBV;
	int MotorCN, MotorCF, MotorCV;
	int MotorDN, MotorDF, MotorDV;
	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	//Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	
	Restaurant();
	~Restaurant();
	void Insert(Order* order, SortedList<Order*> &List);
	void InsertMotorcycle(Motorcycle* order, SortedList<Motorcycle*> &List);
	void AddEvent(Event* pE);	//adds a new event to the queue of events
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void cancelation(int id);
	void StepByStep();
	void SilentMode();
	bool NotEmpty();
	void Interactive();
	void Simulation();
	void Load();
	void Output();
	GUI* getGUI();
	void AddOrder(Order*order);
	void Autopromotion();
	void promotion(int id,int mon);
	bool Assigning(Order*pOrd,Motorcycle* &pMoto);
	void Unassigning();
	/// ==> 
	///  DEMO-related functions. Should be removed in phases 1&2
	//void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	//void AddtoDemoQueue(Order* po);	//adds an order to the demo queue
	//Order* getDemoOrder();			//return the front order from demo queue
	/// ==> 


	//
	// TODO: Add More Member Functions As Needed
	//

};

#endif