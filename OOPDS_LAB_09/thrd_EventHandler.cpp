#include "Thread.h"

DWORD WINAPI Thread_EventHandler(LPVOID pParam)
{
	ThreadParam_Event *pThrdParam;
	HeapPriorityQueue<Event> *pPriQ_Event;
	THREAD_FLAG *pFlagThreadTerminate;
	ThreadStatusMonitor *pThrdMon;

	int myRole;
	int maxRound;

	//Event *event;
	int event_no = 0;
	int event_priority = 0;
	int event_gen_count = 0;
	int targetEventGen;
	int myAddr = -1;

	pThrdParam = (ThreadParam_Event *)pParam;
	myRole = pThrdParam->role;
	myAddr = pThrdParam->myAddr;
	pPriQ_Event = pThrdParam->pPriQ_Event;
	pThrdMon = pThrdParam->pThrdMon;
	maxRound = pThrdParam->maxRound;
	targetEventGen = pThrdParam->targetEventGen;
	pFlagThreadTerminate = pThrdParam->pThrdMon->pFlagThreadTerminate;

		for (int round = 0; round < maxRound; round++)
		{
			if (*pThrdMon->pFlagThreadTerminate == TERMINATE)
				break;
			Event event = pPriQ_Event->removeMin();
			if (event.getEventNo() != -1)
			{
				EnterCriticalSection(pThrdParam->pCS_thrd_mon);
				pThrdMon->eventProcessed[pThrdMon->totalEventProcessed] = event;
				pThrdMon->numEventProcessed++;
				pThrdMon->totalEventProcessed++;
				LeaveCriticalSection(pThrdParam->pCS_thrd_mon);
			} // end if
			Sleep(300 + rand() % 500);
		} // end for
	return 0;
}