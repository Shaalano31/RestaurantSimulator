#pragma once
#include "Generic_DS/Node.h"
#include "GUI/GUI.h"
#include "../Restaurant/Rest/Motorcycle.h"
template <typename T>
class SortedList
{
private:
	int itemcount;
	Node<T>*Head;
	GUI* pGUI;
public:
	SortedList();
	Order* getOrder(int index);
	void Insert(int pos, const T& newEntry);
	void InsertBeg(const T& newEntry);
	bool Remove(T& anEntry);
	bool isEmpty();
	int getCount();
	Motorcycle* getMotorcycle(int index);
};

template <typename T>
void SortedList<T>::InsertBeg(const T& newEntry)
{
	Node<T>* temp = new Node<T>(newEntry);
	temp->setNext(Head);
	Head = temp;
	itemcount++;
}


template <typename T>
int SortedList<T>::getCount()
{
	return itemcount;
}


template <typename T>
Order* SortedList<T>::getOrder(int index)
{
	Node<T> * temp = Head;
	for (int i = 0; i < index; i++)
	{
		temp = temp->getNext();
	}
	Order* pOrd;
	pOrd = temp->getItem();
	//pGUI->PrintMessage("Getting order");
	//pGUI->waitForClick();
	return pOrd;
}
template <typename T>
Motorcycle* SortedList<T>::getMotorcycle(int index)
{
	Node<T> * temp = Head;
	for (int i = 0; i < index; i++)
	{
		temp = temp->getNext();
	}
	Motorcycle* pOrd;
	pOrd = temp->getItem();
	//pGUI->PrintMessage("Getting order");
	//pGUI->waitForClick();
	return pOrd;
}

template<typename T>
SortedList<T>::SortedList()
{
	Head = nullptr;
	itemcount = 0;
}

template<typename T>
void SortedList<T>::Insert(int pos, const T& newEntry)
{
	if (pos == -1)
	{
		InsertBeg(newEntry);
		return;
	}
	Node<T>* temp = new Node<T>(newEntry);
	Node<T>* P = Head;
	for (int i = 0; i < pos; i++)
		P = P->getNext();


	Node<T>* temp2 = P->getNext();
	P->setNext(temp);
	temp->setNext(temp2);
	itemcount++;
}

template<typename T>
bool SortedList<T>::Remove(T& anEntry)
{
	if (isEmpty())
		return false;

	Node<T> *ptr = Head;
	if (Head->getNext() == nullptr)
	{
		anEntry = Head->getItem();
		delete Head;
		Head = nullptr;
		itemcount--;
		return true;
	}
	anEntry = Head->getItem();
	Head = Head->getNext();

	delete ptr;
	itemcount--;
	return true;
}

template<typename T>
bool SortedList<T>::isEmpty()
{
	if (Head == NULL)
		return true;
	return false;
}
