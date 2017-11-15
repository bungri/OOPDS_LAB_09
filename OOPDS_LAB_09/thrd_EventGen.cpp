#include "Thread.h"

DWORD WINAPI Thread_EventGenerator(LPVOID pParam)
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
	int myAddr;

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
			if (event_gen_count >= targetEventGen)
			{
				if (*pThrdMon->pFlagThreadTerminate == TERMINATE)
					break;
				else {
					Sleep(500);
					continue;
				}
			}
			event_no = event_gen_count;
			event_priority = targetEventGen - (event_gen_count + 1);
			Event event(event_no, event_priority, myAddr);

			pPriQ_Event->insert(event);
			//Sleep(10);

			EnterCriticalSection(pThrdParam->pCS_thrd_mon);
			pThrdMon->eventGenerated[pThrdMon->totalEventGenerated] = event;
			event_gen_count++;
			pThrdMon->numEventGenerated++;
			pThrdMon->totalEventGenerated++;
			LeaveCriticalSection(pThrdParam->pCS_thrd_mon);

			//Sleep(100 + rand() % 300);
			Sleep(10);
		}
	return 0;
}