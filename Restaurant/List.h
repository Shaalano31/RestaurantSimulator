#pragma once
#include "Generic_DS/Node.h"

template <typename T>
class List
{
private:
	int itemcount;
	Node<T>*Head;
public:
	List();
	void Insert(const T& data);
	bool remove(T& anEntry);
	bool Delete(int id);
	bool isEmpty();
	Node<T>* getHead();
	T GetItem();
	T Search(int id);
	void sethead(Node<T>*head);
	void decrementcount();
	void InsertBeg(const T& data);
	~List();
};

template <typename T>
void List<T>::InsertBeg(const T& newEntry)
{
	Node<T>* temp = new Node<T>(newEntry);
	temp->setNext(Head);
	Head = temp;
	itemcount++;
}

template<typename T>
Node<T>* List<T>::getHead()
{
	return Head;
}

//template<typename T>
//T List<T>::GetItem()
//{
//	return Head->getItem();
//}


template<typename T>
List<T>::List()
{
	Head = nullptr;
	itemcount = 0;
}

template<typename T>
void List<T>::Insert(const T& data)
{
	Node<T>*p = new Node<T>;
	p->setItem(data);
	if (Head == nullptr)
	{
		Head = p;
		itemcount++;
	}
	else
	{
		Node<T>* curr = Head;
		while (curr->getNext())
		{
			curr = curr->getNext();
		}
		curr->setNext(p);
		itemcount++;
	}
}

template<typename T>
bool List<T>::isEmpty()
{
	if (Head == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
bool List<T>::Delete(int id)
{
	Node<T> *temp = Head;
	if (isEmpty())
		return false;

	if (*(Head->getItem()) == id)
	{
		Head = Head->getNext();
		delete temp;
		itemcount--;
		return true;
	}

	temp = temp->getNext();
	Node <T> *prev = Head;
	while (temp)
	{
		if (*(temp->getItem()) == id)
		{
			prev->setNext(temp->getNext());
			delete temp;
			itemcount--;
			return true;
		}
		else
		{
			temp = temp->getNext();
			prev = prev->getNext();
		}
	}
}

template <typename T>
bool List<T> ::remove(T& anEntry)
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
template <typename T>
 T List<T>::Search(int id)
 {  if(Head==NULL){ return NULL;}
	 Node<T>*ptr=Head;
 if(*(ptr->getItem())==id)
 {  
	 Head=Head->getNext();
	 ptr->setNext(NULL);
	 itemcount--;
	 return ptr->getItem();
 }
     ptr = ptr->getNext();
	 Node <T> *prev = Head;
	 while (ptr)
	 {
		 if (*(ptr->getItem()) == id)
		 {
			 prev->setNext(ptr->getNext());
			 itemcount--;
			 ptr->setNext(NULL);
			 return ptr->getItem();
		 }
		 else 
		 {
			 ptr = ptr->getNext();
			 prev = prev->getNext();
		 }
	 }
	 return NULL;
 }

 template <typename T>
 void List<T>::sethead(Node<T>*head)
 { Head=head;
  itemcount--;
 }
 template <typename T>
 void List<T>::decrementcount()
 { itemcount--;}
 template<typename T>
T List<T>::GetItem() 
{
	return Head->getItem();
}
template <typename T>
List<T>::~List()
{
}