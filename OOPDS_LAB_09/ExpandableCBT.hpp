#ifndef EXPANDABLE_CBT_H
#define EXPANDABLE_CBT_H

#include "Event.hpp"

template<typename E>
class CBTN // CompleteBinTreeNode
{
public:
	CBTN() {
		priority = -1;
		entry = E();
	} // default constructor
	CBTN(int pri, E e)
	{
		priority = pri;
		entry = e;
	}
	int getPriority() { return priority; }
	E* getPtrEntry() { return &entry; }
	E getEntry() { return entry; }
	void setPriority(int pri) { priority = pri; }
	void setEntry(E e) { entry = e; }
private:
	int priority;
	E entry;
};

template<typename E>
class CompleteBinaryTree
{
public:
	CompleteBinaryTree()
	{
		this->priority = 0;
		this->capacity = 1;
		pCBT = new CBTN<E>[capacity + 1];
		//pCBT = (CBTN<E> *)malloc(sizeof(CBTN<E>)* (capacity + 1));
	}
	CompleteBinaryTree(int capa)
	{
		this->priority = 0;
		this->capacity = capa;
		pCBT = new CBTN<E>[capacity + 1];
		//pCBT = (CBTN<E> *)malloc(sizeof(CBTN<E>)* (capacity + 1));
	}
	void initCBinT(int capa)
	{
		this->priority = 0;
		this->capacity = capa;
		pCBT = new CBTN<E>[capacity + 1];
		//pCBT = (CBTN<E> *)malloc(sizeof(CBTN<E>)* (capacity + 1));
	}
	CBTN<E>* getRootNodePtr() { return &pCBT[1]; }
	void doubleCapacity()
	{ 
		CBTN<E> *tmp = new CBTN<E>[capacity*2 + 1];
		//CBTN<E> *tmp = (CBTN<E> *)malloc(sizeof(CBTN<E>)* (capacity*2 + 1));
		for (int i = 0; i < capacity; i++)
			tmp[i] = pCBT[i];
		delete[] pCBT;
		pCBT = tmp;
		this->capacity *= 2;
	}
	int getCapacity() { return capacity; }
	void insertAt(int pos, E& e)
	{ 
		pCBT[pos].setEntry(e);
		pCBT[pos].setPriority(e.getEventPri());
	}
	void removeNode(int pos)
	{
		pCBT[pos].setEntry(E());
		pCBT[pos].setPriority(-1);
	}
	void print(int num_entry)
	{ 
		cout << endl;
		for (int i = 1; i < 1 + num_entry; i++)
		{
			cout << pCBT[i].getEntry() << " ";
			if (i % 5 == 0)
				cout << endl;
		}
	}
protected:
	int priority;
	int capacity;
	CBTN<E>* pCBT; // pointer to dynamic array of CompleteBinaryTreeNode
};

#endif