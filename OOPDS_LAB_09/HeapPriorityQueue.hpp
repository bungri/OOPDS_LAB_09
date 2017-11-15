#ifndef HEAP_PRIORITY_QUEUE
#define HEAP_PRIOIRTY_QUEUE

#include <Windows.h>
#include "ExpandableCBT.hpp"

template<typename E>
class HeapPriorityQueue : CompleteBinaryTree <E> {
public:
	HeapPriorityQueue() : CompleteBinaryTree <E> () // default constructor
	{
		this->num_entry = 0;
		InitializeCriticalSection(&cs_heapPriQ);
	}
	HeapPriorityQueue(int capacity) : CompleteBinaryTree <E>(capacity) // default constructor
	{
		this->num_entry = 0;
		InitializeCriticalSection(&cs_heapPriQ);
	}
	~HeapPriorityQueue() // default destructor
	{
		delete[] this->pCBT;
		this->pCBT = NULL;
	}
	int size() const { return num_entry; }
	bool empty() const { return (num_entry == 0); }
	CompleteBinaryTree<E>* getCBinTree() { return &CBT; }
	bool hasLeftChild(int pos) { return ((pos * 2) <= num_entry); }
	bool hasRightChild(int pos) { return ((pos * 2 + 1) <= num_entry); }

	E* getMinPtr()
	{
		return &pCBT[num_entry + 1];
	}

	void insert(E& e)
	{
		EnterCriticalSection(&cs_heapPriQ);

		if ((++num_entry) >= capacity)
		{
			doubleCapacity();
		}
		

		insertAt(num_entry, e);
		int pos = num_entry;
		int pos_parent;

		/* up-heap bubbling */
		while (&pCBT[pos] != getRootNodePtr())
		{
			pos_parent = pos / 2;
			if (pCBT[pos].getPriority() >= pCBT[pos_parent].getPriority())
			{
				break; // if the priority of the new Event is lower than its parent's priority, just stop up-heap bubbling
			}
			else
			{
				CBTN<E> CBT_Event_tmp = pCBT[pos];
				pCBT[pos] = pCBT[pos_parent];
				pCBT[pos_parent] = CBT_Event_tmp;

				pos = pos_parent;
			}
		}
		LeaveCriticalSection(&cs_heapPriQ);
	}

	E removeMin()
	{
		EnterCriticalSection(&cs_heapPriQ);

		if (num_entry == 0)
		{
			LeaveCriticalSection(&cs_heapPriQ);
			return E();
		}
		else
		{
			CBTN<E> CBT_Event_tmp = pCBT[1];
			pCBT[1] = pCBT[num_entry];
			pCBT[num_entry] = CBT_Event_tmp;

			removeNode(num_entry--);

			int pos = 1;

			/* Down-heap bubbling */
			while (hasLeftChild(pos))
			{
				int pos_child = pos * 2;
				if (hasRightChild(pos))
					if (pCBT[pos_child].getPriority() > pCBT[pos_child+1].getPriority())
						pos_child++;
				if (pCBT[pos_child].getPriority() < pCBT[pos].getPriority())
				{
					CBTN<E> CBT_Tmp = pCBT[pos];
					pCBT[pos] = pCBT[pos_child];
					pCBT[pos_child] = CBT_Tmp;
				}
				else
				{
					break;
				}
				pos = pos_child;
			}

			LeaveCriticalSection(&cs_heapPriQ);
			return CBT_Event_tmp.getEntry();
		}
	}
	void print()
	{
		__super::print(num_entry);
	}
private:
	void swap(E* src, E* dst)
	{
		E tmp = src;
		src = dst;
		dst = tmp;
	}
	int num_entry;
	CRITICAL_SECTION cs_heapPriQ;
};

#endif