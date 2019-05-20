#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include <cmath>


Restaurant::Restaurant() 
{
	pGUI = NULL;
	NormalA=0,NormalB=0,NormalC=0,NormalD=0;
	VIPA=0,VIPB=0,VIPC=0,VIPD=0;
	FrozenA=0,FrozenB=0,FrozenC=0,FrozenD=0;
	ActiveAN = 0, ActiveAV = 0, ActiveAF = 0;
	ActiveBN = 0, ActiveBV = 0, ActiveBF = 0;
	ActiveCN = 0, ActiveCV = 0, ActiveCF = 0;
	ActiveDN = 0, ActiveDV = 0, ActiveDF = 0;
	ServAN = 0 , ServAV = 0 , ServAF = 0 ;
	ServBN = 0 , ServBV = 0 , ServBF = 0;
	ServCN = 0, ServCV = 0 , ServCF = 0;
	ServDN = 0 , ServDV = 0 , ServDF = 0 ;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
	
	Load();
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		Interactive();
		break;
	case MODE_STEP:
		StepByStep();
		break;
	case MODE_SLNT:
		SilentMode();
		break;
	//case MODE_DEMO:
		//Just_A_Demo();
	};
	Output();
}



//////////////////////////////////  Event handling functions   /////////////////////////////
void Restaurant::AddEvent(Event* pE)	//adds a new event to the queue of events
{
	EventsQueue.enqueue(pE);
}

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current time
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}

void Restaurant::cancelation(int id)
{
	if (A_Normal.Delete(id))
	{
		ActiveAN--;
		return;
	}
	else if (B_Normal.Delete(id))
	{
		ActiveBN--;
		return;
	}
	else if (C_Normal.Delete(id))
	{
		ActiveCN--;
		return;
	}
	else if (D_Normal.Delete(id))
	{
		ActiveDN--;
		return;
	}
}

void Restaurant::Insert(Order* order, SortedList<Order*> &List)
{
	if (List.isEmpty())
	{
		List.InsertBeg(order);
		//ServAV++;
	}
	else
	{
		Order* Ord;
		bool inserted = 0;
		for (int i = 0; i < List.getCount(); i++)
		{
			Ord = List.getOrder(i);
			if (Ord->getFT() > order->getFT())
			{
				List.Insert(i - 1, order);
				//ServAV++;
				inserted = 1;
				break;
			}
			/*else if (Ord->getFT() == order->getFT())
			{
				int currentFT = Ord->getFT();
				while (Ord->getFT() == order->getFT())
				{
					if (Ord->getServTime() > order->getServTime())
					{
						List.Insert(i - 1, order);
						inserted = 1;
						break;
					}

					i++;
					Ord = List.getOrder(i);
				}

				break;
			}*/
		}
		if (inserted == 0)
		{
			List.Insert(List.getCount() - 1, order);
			//ServAV++;
		}
	}

	//pGUI->RemoveOrder(order->GetID());
	//ActiveAV--;
}

void Restaurant::InsertMotorcycle(Motorcycle* order, SortedList<Motorcycle*> &List)
{
	if (List.isEmpty())
	{
		List.InsertBeg(order);
		//ServAV++;
	}
	else
	{
		Motorcycle* Ord;
		bool inserted = 0;
		for (int i = 0; i < List.getCount(); i++)
		{
			Ord = List.getMotorcycle(i);
			if (Ord->getSpeed() > order->getSpeed())
			{
				List.Insert(i - 1, order);
				//ServAV++;
				inserted = 1;
				break;
			}
		}
		if (inserted == 0)
		{
			List.Insert(List.getCount() - 1, order);
			//ServAV++;
		}
	}

	//pGUI->RemoveOrder(order->GetID());
	//ActiveAV--;
}

void Restaurant::Unassigning()
{
	//******************************** UnAssigning Motorcycle  A ********************************************
	for (int i = 0; i < A_Fast_Motorcycles.getCount(); i++)
	{
		int F = ceil(A_Fast_Motorcycles.getMotorcycle(i)->getFinishTime());
		if (A_Fast_Motorcycles.getMotorcycle(i)->getStatus() == SERV && F == CurrentTimeStep)
		{
			A_Fast_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			VIPA++;
		}
	}
	for (int i = 0; i < B_Fast_Motorcycles.getCount(); i++)
	{
		if (B_Fast_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(B_Fast_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			B_Fast_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			VIPB++;
		}
	}
	for (int i = 0; i < C_Fast_Motorcycles.getCount(); i++)
	{
		if (C_Fast_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(C_Fast_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			C_Fast_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			VIPC++;
		}
	}
	for (int i = 0; i < D_Fast_Motorcycles.getCount(); i++)
	{
		if (D_Fast_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(D_Fast_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			D_Fast_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			VIPD++;
		}
	}
	//********************************* Unassigning Motorcycle Normal ***************************************
	for (int i = 0; i < A_Normal_Motorcycles.getCount(); i++)
	{
		if (A_Normal_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(A_Normal_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			A_Normal_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			NormalA++;
		}
	}
	for (int i = 0; i < B_Normal_Motorcycles.getCount(); i++)
	{
		if (B_Normal_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(B_Normal_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			B_Normal_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			NormalB++;
		}
	}
	for (int i = 0; i < C_Normal_Motorcycles.getCount(); i++)
	{
		if (C_Normal_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(C_Normal_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			C_Normal_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			NormalC++;
		}
	}
	for (int i = 0; i < D_Normal_Motorcycles.getCount(); i++)
	{
		if (D_Normal_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(D_Normal_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			D_Normal_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			NormalD++;
		}
	}
	//******************************** UnAssigning Motorcycle  Frozen ********************************************
	for (int i = 0; i < A_Frozen_Motorcycles.getCount(); i++)
	{
		if (A_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(A_Frozen_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			A_Frozen_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			FrozenA++;
		}
	}
	for (int i = 0; i < B_Frozen_Motorcycles.getCount(); i++)
	{
		if (B_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(B_Frozen_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			B_Frozen_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			FrozenB++;
		}
	}
	for (int i = 0; i < C_Frozen_Motorcycles.getCount(); i++)
	{
		if (C_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(C_Frozen_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			C_Frozen_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			FrozenC++;
		}
	}
	for (int i = 0; i < D_Frozen_Motorcycles.getCount(); i++)
	{
		if (D_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(D_Frozen_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
		{
			D_Frozen_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
			FrozenD++;
		}
	}
}

void Restaurant::Simulation()
{
	Autopromotion();
	bool serv = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Order *pOrd;
	Motorcycle*pMoto;
	// VIP orders
	string LastAVV;
	string LastBVV;
	string LastCVV;
	string LastDVV;
	string LastAVF;
	string LastBVF;
	string LastCVF;
	string LastDVF;
	string LastAVN;
	string LastBVN;
	string LastCVN;
	string LastDVN;
	// Normal Orders
	string LastANV;
	string LastBNV;
	string LastCNV;
	string LastDNV;
	string LastANN;
	string LastBNN;
	string LastCNN;
	string LastDNN;
	string LastANF;
	// Frozen Order
	string LastAFF;
	string LastBFF;
	string LastCFF;
	string LastDFF;

	int FT, ST;
	while (A_VIP.Remove(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());
			if (pMoto->getType() == TYPE_VIP)
			{
				LastAVV = " V" + MID + "(V" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_NRM)
			{
				LastAVN = " N" + MID + "(V" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_FROZ)
			{
				LastAVF = " F " + MID + "(V" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);

			Insert(pOrd, AServList);
			pGUI->RemoveOrder(pOrd->GetID());
			ActiveAV--;
			ServAV++;

		}
		else
		{
			A_VIP.InsertBeg(pOrd);
			break;
		}
	}
	while (B_VIP.Remove(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());
			if (pMoto->getType() == TYPE_VIP)
			{
				LastBVV = " V" + MID + "(V" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_NRM)
			{
				LastBVN = " N" + MID + "(V" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_FROZ)
			{
				LastBVF = " F " + MID + "(V" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);

			Insert(pOrd, BServList);
			pGUI->RemoveOrder(pOrd->GetID());
			ActiveBV--;
			ServBV++;

		}
		else
		{
			B_VIP.InsertBeg(pOrd);
			break;
		}
	}
	while (C_VIP.Remove(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());
			if (pMoto->getType() == TYPE_VIP)
			{
				LastCVV = " V" + MID + "(V" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_NRM)
			{
				LastCVN = " N" + MID + "(V" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_FROZ)
			{
				LastCVF = " F " + MID + "(V" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);

			Insert(pOrd, CServList);
			pGUI->RemoveOrder(pOrd->GetID());
			ActiveCV--;
			ServCV++;

		}
		else
		{
			C_VIP.InsertBeg(pOrd);
			break;
		}
	}
	while (D_VIP.Remove(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());
			if (pMoto->getType() == TYPE_VIP)
			{
				LastDVV = " V" + MID + "(V" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_NRM)
			{
				LastDVN = " N" + MID + "(V" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_FROZ)
			{
				LastDVF = " F " + MID + "(V" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);

			Insert(pOrd, DServList);
			pGUI->RemoveOrder(pOrd->GetID());
			ActiveDV--;
			ServDV++;

		}
		else
		{
			D_VIP.InsertBeg(pOrd);
			break;
		}
	}


	while (A_Frozen.peekFront(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());

			if (pMoto->getType() == TYPE_FROZ)
			{
				LastAFF = " F " + MID + "(F" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);

			Insert(pOrd, AServList);
			pGUI->RemoveOrder(pOrd->GetID());

			A_Frozen.dequeue(pOrd);
			ActiveAF--;
			ServAF++;
		}
		break;
	}
	while (B_Frozen.peekFront(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());

			if (pMoto->getType() == TYPE_FROZ)
			{
				LastBFF = " F " + MID + "(F" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);
			Insert(pOrd, BServList);
			pGUI->RemoveOrder(pOrd->GetID());
			B_Frozen.dequeue(pOrd);
			ActiveBF--;
			ServBF++;
		}
		break;
	}
	while (C_Frozen.peekFront(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());

			if (pMoto->getType() == TYPE_FROZ)
			{
				LastCFF = " F " + MID + "(F" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);
			Insert(pOrd, CServList);
			pGUI->RemoveOrder(pOrd->GetID());
			C_Frozen.dequeue(pOrd);
			ActiveCF--;
			ServCF++;
		}
		break;
	}
	while (D_Frozen.peekFront(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());

			if (pMoto->getType() == TYPE_FROZ)
			{
				LastDFF = " F " + MID + "(F" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->SetServTime(ST);
			pOrd->setFT(FT);
			pMoto->setFinishTime(FT);
			Insert(pOrd, DServList);
			pGUI->RemoveOrder(pOrd->GetID());
			D_Frozen.dequeue(pOrd);
			ActiveDF--;
			ServDF++;
		}
		break;
	}


	while (A_Normal.remove(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());
			if (pMoto->getType() == TYPE_NRM)
			{
				LastANV = " N " + MID + "(N" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_VIP)
			{
				LastANN = " V " + MID + "(N" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_FROZ)
			{
				LastANF = " F " + MID + "(N" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);

			Insert(pOrd, AServList);
			pGUI->RemoveOrder(pOrd->GetID());
			ActiveAN--;
			ServAN++;

		}
		else
		{
			A_Normal.InsertBeg(pOrd);
			break;
		}
	}
	while (B_Normal.remove(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());
			if (pMoto->getType() == TYPE_NRM)
			{
				LastBNV = " N " + MID + "(N" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_VIP)
			{
				LastBNN = " V " + MID + "(N" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);
			Insert(pOrd, BServList);
			pGUI->RemoveOrder(pOrd->GetID());
			ActiveBN--;
			ServBN++;

		}
		else
		{
			B_Normal.InsertBeg(pOrd);
			break;
		}
	}
	while (C_Normal.remove(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{

			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());
			if (pMoto->getType() == TYPE_NRM)
			{
				LastCNV = " N " + MID + "(N" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_VIP)
			{
				LastCNN = " V " + MID + "(N" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->SetServTime(ST);
			pOrd->setFT(FT);
			pMoto->setFinishTime(FT);
			Insert(pOrd, CServList);
			pGUI->RemoveOrder(pOrd->GetID());
			ActiveCN--;
			ServCN++;

		}
		else
		{
			C_Normal.InsertBeg(pOrd);
			break;
		}
	}
	while (D_Normal.remove(pOrd))
	{
		serv = Assigning(pOrd, pMoto);
		if (serv == 1)
		{
			string MID = to_string(pMoto->getID());
			string ID = to_string(pOrd->GetID());
			if (pMoto->getType() == TYPE_NRM)
			{
				LastDNV = " N " + MID + "(N" + ID + ")";
			}
			else if (pMoto->getType() == TYPE_VIP)
			{
				LastDNN = " V " + MID + "(N" + ID + ")";
			}
			ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
			pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
			FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
			pOrd->setFT(FT);
			pOrd->SetServTime(ST);
			pMoto->setFinishTime(FT);

			Insert(pOrd, DServList);
			pGUI->RemoveOrder(pOrd->GetID());
			ActiveDN--;
			ServDN++;
		}
		else
		{
			D_Normal.InsertBeg(pOrd);

			break;
		}
	}

	Unassigning();
	////******************************** UnAssigning Motorcycle  A ********************************************
	//for (int i = 0; i < A_Fast_Motorcycles.getCount(); i++)
	//{
	//	int F = ceil(A_Fast_Motorcycles.getMotorcycle(i)->getFinishTime());
	//	if (A_Fast_Motorcycles.getMotorcycle(i)->getStatus() == SERV && F == CurrentTimeStep)
	//	{
	//		A_Fast_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		VIPA++;
	//	}
	//}
	//for (int i = 0; i < B_Fast_Motorcycles.getCount(); i++)
	//{
	//	if (B_Fast_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(B_Fast_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		B_Fast_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		VIPB++;
	//	}
	//}
	//for (int i = 0; i < C_Fast_Motorcycles.getCount(); i++)
	//{
	//	if (C_Fast_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(C_Fast_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		C_Fast_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		VIPC++;
	//	}
	//}
	//for (int i = 0; i < D_Fast_Motorcycles.getCount(); i++)
	//{
	//	if (D_Fast_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(D_Fast_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		D_Fast_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		VIPD++;
	//	}
	//}
	////********************************* Unassigning Motorcycle Normal ***************************************
	//for (int i = 0; i < A_Normal_Motorcycles.getCount(); i++)
	//{
	//	if (A_Normal_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(A_Normal_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		A_Normal_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		NormalA++;
	//	}
	//}
	//for (int i = 0; i < B_Normal_Motorcycles.getCount(); i++)
	//{
	//	if (B_Normal_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(B_Normal_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		B_Normal_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		NormalB++;
	//	}
	//}
	//for (int i = 0; i < C_Normal_Motorcycles.getCount(); i++)
	//{
	//	if (C_Normal_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(C_Normal_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		C_Normal_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		NormalC++;
	//	}
	//}
	//for (int i = 0; i < D_Normal_Motorcycles.getCount(); i++)
	//{
	//	if (D_Normal_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(D_Normal_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		D_Normal_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		NormalD++;
	//	}
	//}
	////******************************** UnAssigning Motorcycle  Frozen ********************************************
	//for (int i = 0; i < A_Frozen_Motorcycles.getCount(); i++)
	//{
	//	if (A_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(A_Frozen_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		A_Frozen_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		FrozenA++;
	//	}
	//}
	//for (int i = 0; i < B_Frozen_Motorcycles.getCount(); i++)
	//{
	//	if (B_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(B_Frozen_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		B_Frozen_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		FrozenB++;
	//	}
	//}
	//for (int i = 0; i < C_Frozen_Motorcycles.getCount(); i++)
	//{
	//	if (C_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(C_Frozen_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		C_Frozen_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		FrozenC++;
	//	}
	//}
	//for (int i = 0; i < D_Frozen_Motorcycles.getCount(); i++)
	//{
	//	if (D_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == SERV && ceil(D_Frozen_Motorcycles.getMotorcycle(i)->getFinishTime()) == CurrentTimeStep)
	//	{
	//		D_Frozen_Motorcycles.getMotorcycle(i)->setStatus(IDLE);
	//		FrozenD++;
	//	}
	//}


	ExecuteEvents(CurrentTimeStep);
	pGUI->UpdateInterface();


	////////////////////////////////////////////////////Printing information new////////////////////////////////////
	string time = to_string(CurrentTimeStep);
	string NA = to_string(NormalA);
	string FA = to_string(FrozenA);
	string VA = to_string(VIPA);
	string AN = to_string(ActiveAN);
	string AF = to_string(ActiveAF);
	string AV = to_string(ActiveAV);
	string SAV = to_string(ServAV);
	string SAF = to_string(ServAF);
	string SAN = to_string(ServAN);
	string regionA = "Motorcycles in region A (Normal-Frozen-VIP)  " + NA + "  " + FA + "  " + VA + "  Active Order(N-F-V)" + AN + "  " + AF + "  " + AV + "   Served Orders(N-F-V)" + SAN + "  " + SAF + "  " + SAV + " Last Motorcycle & Ord were Assigned" + LastAVV + LastAVN + LastAVF + LastANN + LastANV + LastANF + LastAFF;

	string NB = to_string(NormalB);
	string FB = to_string(FrozenB);
	string VB = to_string(VIPB);
	string BN = to_string(ActiveBN);
	string BF = to_string(ActiveBF);
	string BV = to_string(ActiveBV);
	string SBV = to_string(ServBV);
	string SBF = to_string(ServBF);
	string SBN = to_string(ServBN);
	string regionB = "Motorcycles in region B (Normal-Frozen-VIP)  " + NB + "  " + FB + "  " + VB + "  Active Order(N-F-V)" + BN + "  " + BF + "  " + BV + "   Served Orders(N-F-V)" + SBN + "  " + SBF + "  " + SBV + " Last Motorcycle & Ord were Assigned" + LastBVV + LastBVN + LastBVF + LastBNN + LastBNV + LastBFF;

	string NC = to_string(NormalC);
	string FC = to_string(FrozenC);
	string VC = to_string(VIPC);
	string CN = to_string(ActiveCN);
	string CF = to_string(ActiveCF);
	string CV = to_string(ActiveCV);
	string SCV = to_string(ServCV);
	string SCF = to_string(ServCF);
	string SCN = to_string(ServCN);

	string regionC = "Motorcycles in region C (Normal-Frozen-VIP)  " + NC + "  " + FC + "  " + VC + "  Active Order(N-F-V)" + CN + "  " + CF + "  " + CV + "   Served Orders(N-F-V)" + SCN + "  " + SCF + "  " + SCV + " Last Motorcycle & Ord were Assigned" + LastCVV + LastCVN + LastCVF + LastCNN + LastCNV + LastCFF;

	string ND = to_string(NormalD);
	string FD = to_string(FrozenD);
	string VD = to_string(VIPD);
	string DN = to_string(ActiveDN);
	string DF = to_string(ActiveDF);
	string DV = to_string(ActiveDV);
	string SDV = to_string(ServDV);
	string SDF = to_string(ServDF);
	string SDN = to_string(ServDN);
	string regionD = "Motorcycles in region D (Normal-Frozen-VIP)  " + ND + "  " + FD + "  " + VD + "  Active Order(N-F-V)" + DN + "  " + DF + "  " + DV + "   Served Orders(N-F-V)" + SDN + "  " + SDF + "  " + SDV + " Last Motorcycle & Ord were Assigned" + LastDVV + LastDVN + LastDVF + LastDNN + LastDNV + LastDFF;;

	string message = "Time is " + time;
	pGUI->PrintMessage(message, regionA, regionB, regionC, regionD);


	// VIP orders
	LastAVV = "/0";
	LastBVV = "/0";
	LastCVV = "/0";
	LastDVV = "/0";
	LastAVF = "/0";
	LastBVF = "/0";
	LastCVF = "/0";
	LastDVF = "/0";
	LastAVN = "/0";
	LastBVN = "/0";
	LastCVN = "/0";
	LastDVN = "/0";
	// Normal Orders
	LastANV = "/0";
	LastBNV = "/0";
	LastCNV = "/0";
	LastDNV = "/0";
	LastANN = "/0";
	LastBNN = "/0";
	LastCNN = "/0";
	LastDNN = "/0";
	// Frozen Order
	LastAFF = "/0";
	LastBFF = "/0";
	LastCFF = "/0";
	LastDFF = "/0";

}

bool Restaurant::NotEmpty()
{
	if (!A_VIP.isEmpty() ||
		!B_VIP.isEmpty() ||
		!C_VIP.isEmpty() ||
		!D_VIP.isEmpty() ||
		!A_Frozen.isEmpty() ||
		!B_Frozen.isEmpty() ||
		!C_Frozen.isEmpty() ||
		!D_Frozen.isEmpty() ||
		!A_Normal.isEmpty() ||
		!B_Normal.isEmpty() ||
		!C_Normal.isEmpty() ||
		!D_Normal.isEmpty())
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

void Restaurant::SilentMode()
{
	while (!EventsQueue.isEmpty() || NotEmpty())
	{
		Order *pOrd;
		Motorcycle*pMoto;
		bool serv = 0;
		Autopromotion();
		int FT, ST;
		while (A_VIP.Remove(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);

				Insert(pOrd, AServList);
				pGUI->RemoveOrder(pOrd->GetID());
				ActiveAV--;
				ServAV++;

			}
			else
			{
				A_VIP.InsertBeg(pOrd);
				break;
			}
		}
		while (B_VIP.Remove(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);

				Insert(pOrd, BServList);
				pGUI->RemoveOrder(pOrd->GetID());
				ActiveBV--;
				ServBV++;

			}
			else
			{
				B_VIP.InsertBeg(pOrd);
				break;
			}
		}
		while (C_VIP.Remove(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);

				Insert(pOrd, CServList);
				pGUI->RemoveOrder(pOrd->GetID());
				ActiveCV--;
				ServCV++;

			}
			else
			{
				C_VIP.InsertBeg(pOrd);
				break;
			}
		}
		while (D_VIP.Remove(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);

				Insert(pOrd, DServList);
				pGUI->RemoveOrder(pOrd->GetID());
				ActiveDV--;
				ServDV++;

			}
			else
			{
				D_VIP.InsertBeg(pOrd);
				break;
			}
		}


		while (A_Frozen.peekFront(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);

				Insert(pOrd, AServList);
				pGUI->RemoveOrder(pOrd->GetID());

				A_Frozen.dequeue(pOrd);
				ActiveAF--;
				ServAF++;
			}
			break;
		}
		while (B_Frozen.peekFront(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);
				Insert(pOrd, BServList);
				pGUI->RemoveOrder(pOrd->GetID());
				B_Frozen.dequeue(pOrd);
				ActiveBF--;
				ServBF++;
			}
			break;
		}
		while (C_Frozen.peekFront(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);
				Insert(pOrd, CServList);
				pGUI->RemoveOrder(pOrd->GetID());
				C_Frozen.dequeue(pOrd);
				ActiveCF--;
				ServCF++;
			}
			break;
		}
		while (D_Frozen.peekFront(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->SetServTime(ST);
				pOrd->setFT(FT);
				pMoto->setFinishTime(FT);
				Insert(pOrd, DServList);
				pGUI->RemoveOrder(pOrd->GetID());
				D_Frozen.dequeue(pOrd);
				ActiveDF--;
				ServDF++;
			}
			break;
		}


		while (A_Normal.remove(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);

				Insert(pOrd, AServList);
				pGUI->RemoveOrder(pOrd->GetID());
				ActiveAN--;
				ServAN++;

			}
			else
			{
				A_Normal.InsertBeg(pOrd);
				break;
			}
		}
		while (B_Normal.remove(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);
				Insert(pOrd, BServList);
				pGUI->RemoveOrder(pOrd->GetID());
				ActiveBN--;
				ServBN++;

			}
			else
			{
				B_Normal.InsertBeg(pOrd);
				break;
			}
		}
		while (C_Normal.remove(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->SetServTime(ST);
				pOrd->setFT(FT);
				pMoto->setFinishTime(FT);
				Insert(pOrd, CServList);
				pGUI->RemoveOrder(pOrd->GetID());
				ActiveCN--;
				ServCN++;

			}
			else
			{
				C_Normal.InsertBeg(pOrd);
				break;
			}
		}
		while (D_Normal.remove(pOrd))
		{
			serv = Assigning(pOrd, pMoto);
			if (serv == 1)
			{
				ST = ceil(2 * ((double)pOrd->GetDistance() / (double)pMoto->getSpeed()));
				pOrd->setWaitTime(CurrentTimeStep - pOrd->getArrTime());
				FT = pOrd->getwaittime() + pOrd->getArrTime() + ST;
				pOrd->setFT(FT);
				pOrd->SetServTime(ST);
				pMoto->setFinishTime(FT);

				Insert(pOrd, DServList);
				pGUI->RemoveOrder(pOrd->GetID());
				ActiveDN--;
				ServDN++;
			}
			else
			{
				D_Normal.InsertBeg(pOrd);

				break;
			}
		}

		Unassigning();
		ExecuteEvents(CurrentTimeStep);
		pGUI->UpdateInterface();
		CurrentTimeStep++;
	}
	pGUI->UpdateInterface();
}

void Restaurant::Interactive()
{
	while (!EventsQueue.isEmpty() || NotEmpty())
	{
		Simulation();
		pGUI->waitForClick();
		CurrentTimeStep++;
	}
	pGUI->UpdateInterface();
}

void Restaurant::StepByStep()
{
	while (!EventsQueue.isEmpty() || NotEmpty())
	{
		Simulation();
		Sleep(1000);
		CurrentTimeStep++;
	}
	pGUI->UpdateInterface();
}

void Restaurant::Load()
{
	Event* pEv;
	string filename;
	pGUI->PrintMessage("Enter file name");
	filename = pGUI->GetString() + ".txt";
	int  EventN;
	int NN, FN, VN;
	int SpeedF, SpeedN, SpeedFrozen;

	InputFile.open(filename);
	if (InputFile.is_open())
	{
		InputFile >> NN >> FN >> VN;
		MotorAN = NN;
		MotorAF = FN;
		MotorAV = VN;
		NormalA = NN;
		FrozenA = FN;
		VIPA = VN;
		for (int i = 0; i < NN; i++)
		{
			InputFile>> SpeedN;
			Motorcycle* N = new Motorcycle(SpeedN, i + 1, TYPE_NRM, A_REG);
			Motorcycle* pMot;
			//Insert();
			InsertMotorcycle(N, A_Normal_Motorcycles);
			/*bool inserted = 0;
				if (A_Normal_Motorcycles.isEmpty())
				{
					A_Normal_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < A_Normal_Motorcycles.getCount(); i++)
				{
					pMot = A_Normal_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < SpeedN)
					{
						A_Normal_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					A_Normal_Motorcycles.Insert(A_Normal_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		for (int i = 0; i < FN; i++)
		{
			InputFile>> SpeedFrozen;
			Motorcycle* N = new Motorcycle(SpeedFrozen, i + 1, TYPE_FROZ, A_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, A_Frozen_Motorcycles);
			/*bool inserted = 0;
			if (A_Frozen_Motorcycles.isEmpty())
				{
					A_Frozen_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < A_Frozen_Motorcycles.getCount(); i++)
				{
					pMot = A_Frozen_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < SpeedFrozen)
					{
						A_Frozen_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					A_Frozen_Motorcycles.Insert(A_Frozen_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		for (int i = 0; i < VN; i++)
		{
			InputFile>> SpeedF;
			Motorcycle* N = new Motorcycle(SpeedF, i + 1, TYPE_VIP, A_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, A_Fast_Motorcycles);
			/*bool inserted = 0;
			if (A_Fast_Motorcycles.isEmpty())
				{
					A_Fast_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < A_Fast_Motorcycles.getCount(); i++)
				{
					pMot = A_Fast_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < SpeedF)
					{
						A_Fast_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					A_Fast_Motorcycles.Insert(A_Fast_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		// Region B Motorcycles
		InputFile >> NN >> FN >> VN;
		MotorBN = NN;
		MotorBF = FN;
		MotorBV = VN;
		NormalB = NN;
		FrozenB = FN;
		VIPB = VN;
		for (int i = 0; i < NN; i++)
		{
			InputFile>> SpeedN;
			Motorcycle* N = new Motorcycle(SpeedN, i + 1, TYPE_NRM, B_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, B_Normal_Motorcycles);
				/*bool inserted = 0;
				if (B_Normal_Motorcycles.isEmpty())
				{
					B_Normal_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < B_Normal_Motorcycles.getCount(); i++)
				{
					pMot = B_Normal_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < N->getSpeed())
					{
						B_Normal_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					B_Normal_Motorcycles.Insert(B_Normal_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		for (int i = 0; i < FN; i++)
		{
			InputFile>> SpeedFrozen;
			Motorcycle* N = new Motorcycle(SpeedFrozen, i + 1, TYPE_FROZ, B_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, B_Frozen_Motorcycles);
			/*bool inserted = 0;
			if (B_Frozen_Motorcycles.isEmpty())
				{
					B_Frozen_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < B_Frozen_Motorcycles.getCount(); i++)
				{
					pMot = B_Frozen_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < N->getSpeed())
					{
						B_Frozen_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					B_Frozen_Motorcycles.Insert(B_Frozen_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		for (int i = 0; i < VN; i++)
		{
			InputFile>> SpeedF;
			Motorcycle* N = new Motorcycle(SpeedF, i + 1, TYPE_VIP, B_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, B_Fast_Motorcycles);
			/*bool inserted = 0;
			if (B_Fast_Motorcycles.isEmpty())
				{
					B_Fast_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < B_Fast_Motorcycles.getCount(); i++)
				{
					pMot = B_Fast_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < N->getSpeed())
					{
						B_Fast_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					B_Fast_Motorcycles.Insert(B_Fast_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		// Region C Motorcycles
		InputFile >> NN >> FN >> VN;
		MotorCN = NN;
		MotorCF = FN;
		MotorCV = VN;
		NormalC = NN;
		FrozenC = FN;
		VIPC = VN;
		for (int i = 0; i < NN; i++)
		{
			InputFile>> SpeedN;
			Motorcycle* N = new Motorcycle(SpeedN, i + 1, TYPE_NRM, C_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, C_Normal_Motorcycles);
				/*bool inserted = 0;
				if (C_Normal_Motorcycles.isEmpty())
				{
					C_Normal_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < C_Normal_Motorcycles.getCount(); i++)
				{
					pMot = C_Normal_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < N->getSpeed())
					{
						C_Normal_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					C_Normal_Motorcycles.Insert(C_Normal_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		for (int i = 0; i < FN; i++)
		{
			InputFile>> SpeedFrozen;
			Motorcycle* N = new Motorcycle(SpeedFrozen, i + 1, TYPE_FROZ, C_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, C_Frozen_Motorcycles);
			/*bool inserted = 0;
			if (C_Frozen_Motorcycles.isEmpty())
				{
					C_Frozen_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < C_Frozen_Motorcycles.getCount(); i++)
				{
					pMot = C_Frozen_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < N->getSpeed())
					{
						C_Frozen_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					C_Frozen_Motorcycles.Insert(C_Frozen_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		for (int i = 0; i < VN; i++)
		{
			InputFile>> SpeedF;
			Motorcycle* N = new Motorcycle(SpeedF, i + 1, TYPE_VIP, C_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, C_Fast_Motorcycles);
			/*bool inserted = 0;
			if (C_Fast_Motorcycles.isEmpty())
				{
					C_Fast_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < C_Fast_Motorcycles.getCount(); i++)
				{
					pMot = C_Fast_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < N->getSpeed())
					{
						C_Fast_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					C_Fast_Motorcycles.Insert(C_Fast_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		// Region D Motorcycles
		InputFile >> NN >> FN >> VN;
		MotorDN = NN;
		MotorDF = FN;
		MotorDV = VN;
		NormalD = NN;
		FrozenD = FN;
		VIPD = VN;
		for (int i = 0; i < NN; i++)
		{
			InputFile>> SpeedN;
			Motorcycle* N = new Motorcycle(SpeedN, i + 1, TYPE_NRM, D_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, D_Normal_Motorcycles);
				/*bool inserted = 0;
				if (D_Normal_Motorcycles.isEmpty())
				{
					D_Normal_Motorcycles.InsertBeg(N);
				}
				else
				{
					for (int i = 0; i < D_Normal_Motorcycles.getCount(); i++)
					{
						pMot = D_Normal_Motorcycles.getMotorcycle(i);
						if (pMot->getSpeed() < N->getSpeed())
						{
							D_Normal_Motorcycles.Insert(i,N);
							inserted = 1;
						}
					}
					if (inserted == 0)
					{
						D_Normal_Motorcycles.Insert(D_Normal_Motorcycles.getCount() - 1,N);
					}
				}*/
		}
		for (int i = 0; i < FN; i++)
		{
				InputFile>> SpeedFrozen;
				Motorcycle* N = new Motorcycle(SpeedFrozen, i + 1, TYPE_FROZ, D_REG);
				Motorcycle* pMot;
				InsertMotorcycle(N, D_Frozen_Motorcycles);
				/*bool inserted = 0;
				if (D_Frozen_Motorcycles.isEmpty())
					{
						D_Frozen_Motorcycles.InsertBeg(N);
					}
				else
				{
					for (int i = 0; i < D_Frozen_Motorcycles.getCount(); i++)
					{
						pMot = D_Frozen_Motorcycles.getMotorcycle(i);
						if (pMot->getSpeed() < N->getSpeed())
						{
							D_Frozen_Motorcycles.Insert(i,N);
							inserted = 1;
						}
					}
					if (inserted == 0)
					{
						D_Frozen_Motorcycles.Insert(D_Frozen_Motorcycles.getCount() - 1,N);
					}
				}*/
		}
		for (int i = 0; i < VN; i++)
		{
			InputFile>> SpeedF;
			Motorcycle* N = new Motorcycle(SpeedF, i + 1, TYPE_VIP, D_REG);
			Motorcycle* pMot;
			InsertMotorcycle(N, D_Fast_Motorcycles);
			/*bool inserted = 0;
			if (D_Fast_Motorcycles.isEmpty())
				{
					D_Fast_Motorcycles.InsertBeg(N);
				}
			else
			{
				for (int i = 0; i < D_Fast_Motorcycles.getCount(); i++)
				{
					pMot = D_Fast_Motorcycles.getMotorcycle(i);
					if (pMot->getSpeed() < N->getSpeed())
					{
						D_Fast_Motorcycles.Insert(i,N);
						inserted = 1;
					}
				}
				if (inserted == 0)
				{
					D_Fast_Motorcycles.Insert(D_Fast_Motorcycles.getCount() - 1,N);
				}
			}*/
		}
		////// Events
		char x;
		InputFile >> AutoPromotion;
		InputFile >> EventN;
		for (int i = 0; i < EventN; i++)
		{
			InputFile >> x;
			if (x == 'R')
			{
				REGION r;
				ORD_TYPE o;
				int TS, ID, DST;
				double MON;
				char TYP, REG;
				InputFile >> TS >> TYP >> ID >> DST >> MON >> REG;
				if (TYP == 'V')
				{
					o = TYPE_VIP;
				}
				else if (TYP == 'F')
				{
					o = TYPE_FROZ;
				}
				else if (TYP == 'N')
				{
					o = TYPE_NRM;
				}


				if (REG == 'A')
				{
					r = A_REG;
				}
				else if (REG == 'B')
				{
					r = B_REG;
				}
				else if (REG == 'C')
				{
					r = C_REG;
				}
				else if (REG == 'D')
				{
					r = D_REG;
				}
				pEv = new ArrivalEvent(TS, ID, o, r, MON, DST);
				AddEvent(pEv);

			}
			else if (x == 'X')
			{
				int TSX, IDX;
				InputFile >> TSX >> IDX;
				pEv = new CancelEvent(TSX, IDX);
				AddEvent(pEv);
			}
			else if (x == 'P')
			{
				int TSP, IDP, ExMON;
				InputFile >> TSP >> IDP >> ExMON;
				pEv = new PromoteEvent(TSP, IDP, ExMON);
				AddEvent(pEv);
			}
		}
	}
	else
	{
		pGUI->PrintMessage("Error: Could not open the file!");
		pGUI->waitForClick();
	}
}

void Restaurant::Output()
{
	string filename;
	pGUI->PrintMessage("Enter file name");
	filename = pGUI->GetString() + ".txt";
	OutputFile.open(filename);
	Order* pOrd;
	if (OutputFile.is_open())
	{
		double SumWait = 0, SumServe = 0;
		OutputFile << "FT" << "   " << "ID" << "   " << "AT" << "   " << "WT" << "   " << "ST" << endl;
		while (!AServList.isEmpty())
		{
			AServList.Remove(pOrd);
			SumWait += pOrd->getwaittime();
			SumServe += pOrd->getServTime();
			OutputFile << pOrd->getFT() << "   " << pOrd->GetID() << "   " << pOrd->getArrTime() << "   " << pOrd->getwaittime() << "   " << pOrd->getServTime()<<endl;
		}
		OutputFile << "Region A:\n";
		OutputFile << "        " << "Orders:" << " " << (ServAF + ServAN + ServAV) << " " << "[Norm:" << ServAN << ", Froz:" << ServAF << ", VIP:" << ServAV <<"]"<< endl;
		OutputFile << "        " << "MotorC:" << (MotorAN + MotorAF + MotorAV) << " " << "[Norm:" << MotorAN << ", Froz:" << MotorAF << ", VIP:" << MotorAV <<"]"<< endl;
		OutputFile << "        " << "Avg Wait= " << SumWait/ (ServAF + ServAN + ServAV )<<" " << ", Avg Serv= " << SumServe / (ServAF + ServAN + ServAV) << endl;
		OutputFile << "FT" << "   " << "ID" << "   " << "AT" << "   " << "WT" << "   " << "ST" << endl;


		SumWait = 0; SumServe = 0;
		while (!BServList.isEmpty())
		{
			BServList.Remove(pOrd);
			SumWait += pOrd->getwaittime();
			SumServe += pOrd->getServTime();
			OutputFile << pOrd->getFT() << "   " << pOrd->GetID() << "   " << pOrd->getArrTime() << "   " << pOrd->getwaittime() << "   " << pOrd->getServTime()<<endl;
		}
		OutputFile << "Region B:\n";
		OutputFile << "        " << "Orders:" << " " << (ServBF + ServBN + ServBV) << " " << "[Norm:" << ServBN << ", Froz:" << ServBF << ", VIP:" << ServBV <<"]"<< endl;
		OutputFile << "        " << "MotorC:" << (MotorBN + MotorBF + MotorBV) << " " << "[Norm:" << MotorBN << ", Froz:" << MotorBF << ", VIP:" << MotorBV <<"]"<< endl;
		OutputFile << "        " << "Avg Wait= " << SumWait / (ServBF + ServBN + ServBV) <<" " << ", Avg Serv= " << SumServe / (ServBF + ServBN + ServBV) << endl;
		OutputFile << "FT" << "   " << "ID" << "   " << "AT" << "   " << "WT" << "   " << "ST" << endl;

		SumWait = 0; SumServe = 0;
		while (!CServList.isEmpty())
		{
			CServList.Remove(pOrd);
			SumWait += pOrd->getwaittime();
			SumServe += pOrd->getServTime();
			OutputFile << pOrd->getFT() << "   " << pOrd->GetID() << "   " << pOrd->getArrTime() << "   " << pOrd->getwaittime() << "   " << pOrd->getServTime() << endl;
		}
		OutputFile << "Region C:\n";
		OutputFile << "        " << "Orders:" << " " << (ServCF + ServCN + ServCV) << " " << "[Norm:" << ServCN << ", Froz:" << ServCF << ", VIP:" << ServCV << "]" << endl;
		OutputFile << "        " << "MotorC:" << (MotorCN + MotorCF + MotorCV) << " " << "[Norm:" << MotorCN << ", Froz:" << MotorCF << ", VIP:" << MotorCV <<" ]"<< endl;
		OutputFile << "        " << "Avg Wait= " << SumWait / (ServCF + ServCN + ServCV) << " " << ", Avg Serv= " << SumServe / (ServCF + ServCN + ServCV) <<endl;
		OutputFile << "FT" << "   " << "ID" << "   " << "AT" << "   " << "WT" << "   " << "ST" << endl;

		SumWait = 0; SumServe = 0;
		while (!DServList.isEmpty())
		{
			DServList.Remove(pOrd);
			SumWait += pOrd->getwaittime();
			SumServe += pOrd->getServTime();
			OutputFile << pOrd->getFT() << "   " << pOrd->GetID() << "   " << pOrd->getArrTime() << "   " << pOrd->getwaittime() << "   " << pOrd->getServTime() << endl;;
		}
		OutputFile << "Region D:\n";
		OutputFile << "        " << "Orders:" << " " << (ServDF + ServDN + ServDV) << " " << "[Norm:" << ServDN << ", Froz:" << ServDF << ", VIP:" << ServDV <<"]"<< endl;
		OutputFile << "        " << "MotorC:" << (MotorDN + MotorDF + MotorDV) << " " << "[Norm:" << MotorDN << ", Froz:" << MotorDF << ", VIP:" << MotorDV <<"]"<< endl;
		OutputFile << "        " << "Avg Wait= " << SumWait / (ServDF + ServDN + ServDV) <<" " << ", Avg Serv= " << SumServe / (ServDF + ServDN + ServDV) <<endl;
	}
}

GUI* Restaurant::getGUI()
{
	return pGUI;
}

void Restaurant::AddOrder(Order*order)
{
	if (order->GetRegion() == A_REG)
	{
		if (order->GetType() == TYPE_VIP)
		{
			Insert(order, A_VIP);
			ActiveAV++;
			
		}
		else if (order->GetType() == TYPE_FROZ)
		{
			A_Frozen.enqueue(order);
			ActiveAF++;
		}
		else if (order->GetType() == TYPE_NRM)
		{
			A_Normal.Insert(order);
			ActiveAN++;  
		}
	}
	else if (order->GetRegion() == B_REG)
	{
		if (order->GetType() == TYPE_VIP)
		{
			Insert(order, B_VIP);
			ActiveBV++;
			
		}
		else if (order->GetType() == TYPE_FROZ)
		{
			B_Frozen.enqueue(order);
			ActiveBF++;
		}
		else if (order->GetType() == TYPE_NRM)
		{
			B_Normal.Insert(order);
			ActiveBN++;
		}
	}
	else if (order->GetRegion() == C_REG)
	{
		if (order->GetType() == TYPE_VIP)
		{
			Insert(order, C_VIP);
			ActiveCV++;
			
		}
		else if (order->GetType() == TYPE_FROZ)
		{
			C_Frozen.enqueue(order);
			ActiveCF++;
		}
		else if (order->GetType() == TYPE_NRM)
		{
			C_Normal.Insert(order);
			ActiveCN++;
		}
	}
	else if (order->GetRegion() == D_REG)
	{
		if (order->GetType() == TYPE_VIP)
		{
			Insert(order, D_VIP);
			ActiveDV++;
			
		}
		else if (order->GetType() == TYPE_FROZ)
		{
			D_Frozen.enqueue(order);
			ActiveDF++;
		}
		else if (order->GetType() == TYPE_NRM)
		{
			D_Normal.Insert(order);
			ActiveDN++;
		}
	}
}

void Restaurant::Autopromotion()
{
	Node<Order*>* TempA = A_Normal.getHead();
	Node<Order*>* TempB = B_Normal.getHead();
	Node<Order*>* TempC = C_Normal.getHead();
	Node<Order*>* TempD = D_Normal.getHead();
	while (TempA)
	{
		int ARR = TempA->getItem()->getArrTime();
		if ((CurrentTimeStep - ARR) == AutoPromotion)
		{
			Order* V = new Order(TempA->getItem()->GetID(), TYPE_VIP, A_REG, TempA->getItem()->getMoney(), TempA->getItem()->getArrTime(), TempA->getItem()->GetDistance());
			AddOrder(V);
			pGUI->AddOrderForDrawing(V);
			//Insert(V, A_VIP);
			//ActiveAV++;
			Order* temp = TempA->getItem();
			TempA = TempA->getNext();
			pGUI->RemoveOrder(temp->GetID());
			A_Normal.Delete(temp->GetID());
			ActiveAN--;
			continue;
		}
		TempA = TempA->getNext();
	}
	while (TempB)
	{
		int BRR = TempB->getItem()->getArrTime();
		if ((CurrentTimeStep - BRR) == AutoPromotion)
		{
			Order* V = new Order(TempB->getItem()->GetID(), TYPE_VIP, B_REG, TempB->getItem()->getMoney(), TempB->getItem()->getArrTime(), TempB->getItem()->GetDistance());
			AddOrder(V);
			pGUI->AddOrderForDrawing(V);
			//Insert(V, A_VIP);
			//ActiveBV++;
			Order* temp = TempB->getItem();
			TempB = TempB->getNext();
			pGUI->RemoveOrder(temp->GetID());
			B_Normal.Delete(temp->GetID());
			ActiveBN--;
			continue;
		}
		TempB = TempB->getNext();
	}
	while (TempC)
	{
		int CRR = TempC->getItem()->getArrTime();
		if ((CurrentTimeStep - CRR) == AutoPromotion)
		{
			Order* V = new Order(TempC->getItem()->GetID(), TYPE_VIP, C_REG, TempC->getItem()->getMoney(), TempC->getItem()->getArrTime(), TempC->getItem()->GetDistance());
			AddOrder(V);
			pGUI->AddOrderForDrawing(V);
			//Insert(V, A_VIP);
			//ActiveCV++;
			Order* temp = TempC->getItem();
			TempC = TempC->getNext();
			pGUI->RemoveOrder(temp->GetID());
			C_Normal.Delete(temp->GetID());
			ActiveCN--;
			continue;
		}
		TempC = TempC->getNext();
	}
	while (TempD)
	{
		int DRR = TempD->getItem()->getArrTime();
		if ((CurrentTimeStep - DRR) == AutoPromotion)
		{
			Order* V = new Order(TempD->getItem()->GetID(), TYPE_VIP, D_REG, TempD->getItem()->getMoney(), TempD->getItem()->getArrTime(), TempD->getItem()->GetDistance());
			AddOrder(V);
			pGUI->AddOrderForDrawing(V);
			//Insert(V, A_VIP);
			//ActiveDV++;
			Order* temp = TempD->getItem();
			TempD = TempD->getNext();
			pGUI->RemoveOrder(temp->GetID());
			D_Normal.Delete(temp->GetID());
			ActiveDN--;
			continue;
		}
		TempD = TempD->getNext();
	}
}

void Restaurant::promotion(int id,int mon)
{   Order* p=A_Normal.Search(id);
	if(p!=NULL)
	{
		p->setmoney(mon);
		p->setordertype(TYPE_VIP);
		if (A_VIP.isEmpty())
		{
			A_VIP.InsertBeg(p);
			ActiveAV++;
			ActiveAN--;
		}
		else
		{
			Order* pOrd;
			bool inserted = 0;
			for (int i = 0; i < A_VIP.getCount(); i++)
			{
				pOrd = A_VIP.getOrder(i);
				if (pOrd->GetPriority() > p->GetPriority())
				{
					A_VIP.Insert(i, p);
					ActiveAV++;
					ActiveAN--;
					inserted = 1;
				}
			}
			if (inserted == 0)
			{
				A_VIP.Insert(A_VIP.getCount() - 1, p);
				ActiveAV++;
				ActiveAN--;
			}
		}	
		return;
	}

	p=B_Normal.Search(id);
	if(p!=NULL)
	{ 
		p->setmoney(mon);
		p->setordertype(TYPE_VIP);
		if (B_VIP.isEmpty())
		{
			B_VIP.InsertBeg(p);
			ActiveBV++;
			ActiveBN--;
		}
		else
		{
			Order* pOrd;
			bool inserted = 0;
			for (int i = 0; i < B_VIP.getCount(); i++)
			{
				pOrd = B_VIP.getOrder(i);
				if (pOrd->GetPriority() > p->GetPriority())
				{
					B_VIP.Insert(i, p);
					ActiveBV++;
					ActiveBN--;
					inserted = 1;
				}
			}
			if (inserted == 0)
			{
				B_VIP.Insert(B_VIP.getCount() - 1, p);
				ActiveBV++;
				ActiveBN--;
			}
		}   
		return;
    }
	p=C_Normal.Search(id);
	if(p!=NULL)
	{
		p->setmoney(mon);
		p->setordertype(TYPE_VIP);
		if (C_VIP.isEmpty())
		{
			C_VIP.InsertBeg(p);
			ActiveCV++;
			ActiveCN--;
		}
		else
		{
			Order* pOrd;
			bool inserted = 0;
			for (int i = 0; i < C_VIP.getCount(); i++)
			{
				pOrd = C_VIP.getOrder(i);
				if (pOrd->GetPriority() > p->GetPriority())
				{
					C_VIP.Insert(i, p);
					ActiveCV++;
					ActiveCN--;
					inserted = 1;
				}
			}
			if (inserted == 0)
			{
				C_VIP.Insert(C_VIP.getCount() - 1, p);
				ActiveCV++;
				ActiveCN--;
			}
		}	 
		return;
	}
	p=D_Normal.Search(id);
	if(p!=NULL)
	{
		p->setmoney(mon);
		p->setordertype(TYPE_VIP);
		if (D_VIP.isEmpty())
		{
			D_VIP.InsertBeg(p);
			ActiveDV++;
			ActiveDN--;
		}
		else
		{
			Order* pOrd;
			bool inserted = 0;
			for (int i = 0; i < D_VIP.getCount(); i++)
			{
				pOrd = D_VIP.getOrder(i);
				if (pOrd->GetPriority() > p->GetPriority())
				{
					D_VIP.Insert(i, p);
					ActiveDV++;
					ActiveDN--;
					inserted = 1;
				}
			}
			if (inserted == 0)
			{
				D_VIP.Insert(D_VIP.getCount() - 1, p);
				ActiveDV++;
				ActiveDN--;
			}
		}
		return;
	}
  
}

bool Restaurant::Assigning(Order*pOrd,Motorcycle* &pMoto)
{
	//bool serv = 0;
	if (pOrd->GetType() == TYPE_VIP)
	{
		if (pOrd->GetRegion() == A_REG)
		{
			for (int i = 0; i < A_Fast_Motorcycles.getCount(); i++)
			{
				if (A_Fast_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					A_Fast_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					VIPA--;
					pMoto = A_Fast_Motorcycles.getMotorcycle(i);
					return true;
				}
			}

			for (int i = 0; i < A_Normal_Motorcycles.getCount(); i++)
			{
				if (A_Normal_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					A_Normal_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					NormalA--;
					pMoto = A_Normal_Motorcycles.getMotorcycle(i);
					return true;
				}
			}

			for (int i = 0; i < A_Frozen_Motorcycles.getCount(); i++)
			{
				if (A_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					A_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					FrozenA--;
					pMoto = A_Frozen_Motorcycles.getMotorcycle(i);
					return true;
				}
			}
			return false;
		}
		else if (pOrd->GetRegion() == B_REG)
		{
			for (int i = 0; i < B_Fast_Motorcycles.getCount(); i++)
			{
				if (B_Fast_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					B_Fast_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					VIPB--;
					pMoto = B_Fast_Motorcycles.getMotorcycle(i);
					return true;
				}
			}

			for (int i = 0; i < B_Normal_Motorcycles.getCount(); i++)
			{
				if (B_Normal_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					B_Normal_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					NormalB--;
					pMoto = B_Normal_Motorcycles.getMotorcycle(i);
					return true;
				}
			}

			for (int i = 0; i < B_Frozen_Motorcycles.getCount(); i++)
			{
				if (B_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					B_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					pMoto = B_Frozen_Motorcycles.getMotorcycle(i);
					FrozenB--;
					return true;
				}
			}
			return false;
		}
		else if (pOrd->GetRegion() == C_REG)
		{
			for (int i = 0; i < C_Fast_Motorcycles.getCount(); i++)
			{
				if (C_Fast_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					C_Fast_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					VIPC--;
					pMoto = C_Fast_Motorcycles.getMotorcycle(i);
					return true;
				}
			}

			for (int i = 0; i < C_Normal_Motorcycles.getCount(); i++)
			{
				if (C_Normal_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					C_Normal_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					NormalC--;
					pMoto = C_Normal_Motorcycles.getMotorcycle(i);
					return true;
				}
			}

			for (int i = 0; i < C_Frozen_Motorcycles.getCount(); i++)
			{
				if (C_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					C_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					FrozenC--;
					pMoto = C_Frozen_Motorcycles.getMotorcycle(i);
					return true;
				}
			}
			return false;
		}
		else if (pOrd->GetRegion() == D_REG)
		{
			for (int i = 0; i < D_Fast_Motorcycles.getCount(); i++)
			{
				if (D_Fast_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					D_Fast_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					VIPD--;
					pMoto = D_Fast_Motorcycles.getMotorcycle(i);
					return true;
				}
			}

			for (int i = 0; i < D_Normal_Motorcycles.getCount(); i++)
			{
				if (D_Normal_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					D_Normal_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					NormalD--;
					pMoto = D_Normal_Motorcycles.getMotorcycle(i);
					return true;
				}
			}

			for (int i = 0; i < D_Frozen_Motorcycles.getCount(); i++)
			{
				if (D_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					D_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);

					FrozenD--;
					pMoto = D_Frozen_Motorcycles.getMotorcycle(i);
					return true;
				}
			}
			return false;
		}
	}
	else if (pOrd->GetType() == TYPE_FROZ)
	{
		if (pOrd->GetRegion() == A_REG)
		{
			for (int i = 0; i < A_Frozen_Motorcycles.getCount(); i++)
			{
				if (A_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					A_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					FrozenA--;
					pMoto = A_Frozen_Motorcycles.getMotorcycle(i);
					return true;
				}
			}
		}
		else if (pOrd->GetRegion() == B_REG)
		{
			for (int i = 0; i < B_Frozen_Motorcycles.getCount(); i++)
			{
				if (B_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					B_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					FrozenB--;
					pMoto = B_Frozen_Motorcycles.getMotorcycle(i);
					return true;
				}
			}
		}
		else if (pOrd->GetRegion() == C_REG)
		{
			for (int i = 0; i < C_Frozen_Motorcycles.getCount(); i++)
			{
				if (C_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					C_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					FrozenC--;
					pMoto = C_Frozen_Motorcycles.getMotorcycle(i);
					return true;
				}
			}
		}
		else if (pOrd->GetRegion() == D_REG)
		{
			for (int i = 0; i < D_Frozen_Motorcycles.getCount(); i++)
			{
				if (D_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
				{
					D_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
					FrozenD--;
					pMoto = D_Frozen_Motorcycles.getMotorcycle(i);
					return true;
				}
			}
		}
		return false;
	}
	else if (pOrd->GetType() == TYPE_NRM)
		{
			if (pOrd->GetRegion() == A_REG)
			{
				for (int i = 0; i < A_Normal_Motorcycles.getCount(); i++)
				{
					if (A_Normal_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						A_Normal_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						NormalA--;
						pMoto=A_Normal_Motorcycles.getMotorcycle(i);
						return true;
					}
				}
				for (int i = 0; i < A_Fast_Motorcycles.getCount(); i++)
				{
					if (A_Fast_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						A_Fast_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						VIPA--;
						pMoto=A_Fast_Motorcycles.getMotorcycle(i);
						return true;
					}
				}
				return false;
				/*for (int i = 0; i < A_Frozen_Motorcycles.getCount(); i++)
				{
					if (A_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						A_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						FrozenA--;
						pMoto = A_Frozen_Motorcycles.getMotorcycle(i);
						return true;
					}
				}*/

			}
			else if (pOrd->GetRegion() == B_REG)
			{
				for (int i = 0; i < B_Normal_Motorcycles.getCount(); i++)
				{
					if (B_Normal_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						B_Normal_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						NormalB--;
						pMoto=B_Normal_Motorcycles.getMotorcycle(i);
						return true;
					}
				}
				for (int i = 0; i < B_Fast_Motorcycles.getCount(); i++)
				{
					if (B_Fast_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						B_Fast_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						VIPB--;
						pMoto=B_Fast_Motorcycles.getMotorcycle(i);
						return true;
					}
				}
				return false;
				/*for (int i = 0; i < B_Frozen_Motorcycles.getCount(); i++)
				{
					if (B_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						B_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						FrozenB--;
						pMoto = B_Frozen_Motorcycles.getMotorcycle(i);
						return true;
					}
				}*/
			}
			else if (pOrd->GetRegion() == C_REG)
			{
				for (int i = 0; i < C_Normal_Motorcycles.getCount(); i++)
				{
					if (C_Normal_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						C_Normal_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						NormalC--;
						pMoto=D_Frozen_Motorcycles.getMotorcycle(i);
						return true;
					}
				}
				for (int i = 0; i < C_Fast_Motorcycles.getCount(); i++)
				{
					if (C_Fast_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						C_Fast_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						VIPC--;
						pMoto=C_Fast_Motorcycles.getMotorcycle(i);
						return true;
					}
				}
				return false;
				/*for (int i = 0; i < C_Frozen_Motorcycles.getCount(); i++)
				{
					if (C_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						C_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						FrozenC--;
						pMoto = C_Frozen_Motorcycles.getMotorcycle(i);
						return true;
					}
				}*/
			}
			else if (pOrd->GetRegion() == D_REG)
			{
				for (int i = 0; i < D_Normal_Motorcycles.getCount(); i++)
				{
					if (D_Normal_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						D_Normal_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						NormalD--;
						pMoto=D_Normal_Motorcycles.getMotorcycle(i);
						return true;
					}
				}
				for (int i = 0; i < D_Fast_Motorcycles.getCount(); i++)
				{
					if (D_Fast_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						D_Fast_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						VIPD--;
						pMoto=D_Fast_Motorcycles.getMotorcycle(i);
						return true;
					}
				}
				/*for (int i = 0; i < D_Frozen_Motorcycles.getCount(); i++)
				{
					if (D_Frozen_Motorcycles.getMotorcycle(i)->getStatus() == IDLE)
					{
						D_Frozen_Motorcycles.getMotorcycle(i)->setStatus(SERV);
						FrozenD--;
						pMoto = D_Frozen_Motorcycles.getMotorcycle(i);
						return true;
					}
				}*/
				return false;
			}
		}
}


Restaurant::~Restaurant()
{
	delete pGUI;
}










