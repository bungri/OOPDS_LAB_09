/* main_EventGen_PriQ_EventHandler.cpp */
#include "Thread.h"
#include "ConsoleDisplay.h"
#include "SimParams.h"

void main()
{
	HeapPriorityQueue<Event> heapPriQ_Event;
	Event *pEvent;
	int myAddr = 0;
	int pkt_event_handler_addr, eventPriority;
	ThreadParam_Event thrdParam_EventGen, thrdParam_EventHndlr;
	HANDLE hThrd_EventGenerator, hThrd_EventHandler;
	CRITICAL_SECTION cs_main;
	CRITICAL_SECTION cs_thrd_mon;
	ThreadStatusMonitor thrdMon;
	HANDLE consHndlr;
	THREAD_FLAG eventThreadFlag = RUN;
	int count, numEventGenerated, numEventProcessed;
	Event eventProcessed[TOTAL_NUM_EVENTS];
	InitializeCriticalSection(&cs_main);
	InitializeCriticalSection(&cs_thrd_mon);
	consHndlr = initConsoleHandler();
	thrdMon.pFlagThreadTerminate = &eventThreadFlag;
	thrdMon.totalEventGenerated = 0;
	thrdMon.totalEventProcessed = 0;
	for (int ev = 0; ev < TOTAL_NUM_EVENTS; ev++)
	{
		thrdMon.eventProcessed[ev].setEventNo(-1); // mark as not-processed
		thrdMon.eventProcessed[ev].setEventPri(-1);
	}
	/* Create and Activate Thread_EventHandler */
	thrdMon.numEventProcessed = 0;
	thrdParam_EventHndlr.role = EVENT_HANDLER;
	thrdParam_EventHndlr.myAddr = 2; // link address
	thrdParam_EventHndlr.pCS_main = &cs_main;
	thrdParam_EventHndlr.pCS_thrd_mon = &cs_thrd_mon;
	thrdParam_EventHndlr.pPriQ_Event = &heapPriQ_Event;
	thrdParam_EventHndlr.maxRound = MAX_ROUND;
	thrdParam_EventHndlr.pThrdMon = &thrdMon;
	hThrd_EventHandler = CreateThread(NULL, 0, Thread_EventHandler, &thrdParam_EventHndlr, 0, NULL);
	EnterCriticalSection(&cs_main);
	printf("Thread_EventGen is created and activated ...\n");
	LeaveCriticalSection(&cs_main);
	/* Create and Activate Thread_EventGen */
	thrdMon.numEventGenerated = 0;
	thrdParam_EventGen.role = EVENT_GENERATOR;
	thrdParam_EventGen.myAddr = 1; // my Address
	thrdParam_EventGen.pCS_main = &cs_main;
	thrdParam_EventGen.pCS_thrd_mon = &cs_thrd_mon;
	thrdParam_EventGen.pPriQ_Event = &heapPriQ_Event;
	thrdParam_EventGen.targetEventGen = NUM_EVENTS_PER_GEN;
	thrdParam_EventGen.maxRound = MAX_ROUND;
	thrdParam_EventGen.pThrdMon = &thrdMon;
	hThrd_EventGenerator = CreateThread(NULL, 0, Thread_EventGenerator, &thrdParam_EventGen, 0, NULL);
	EnterCriticalSection(&cs_main);
	printf("Thread_EventGen is created and activated ...\n");
	LeaveCriticalSection(&cs_main);
	for (int round = 0; round < MAX_ROUND; round++)
	{
		EnterCriticalSection(&cs_main);
		system("cls");
		gotoxy(consHndlr, 0, 0);
		printf("Thread monitoring by main() ::\n");
		printf(" round(%2d): current total_event_gen (%2d), total_event_proc(%2d)\n",
			round, thrdMon.totalEventGenerated, thrdMon.totalEventProcessed);
		printf("*****************************\n");
		numEventGenerated = thrdMon.numEventGenerated;
		printf("Events generated (current total = %2d)\n ", numEventGenerated);
		count = 0;
		for (int ev = 0; ev < numEventGenerated; ev++)
		{
			pEvent = &thrdMon.eventGenerated[ev];
			if (pEvent != NULL)
			{
				pEvent->printEvent();
				if (((ev + 1) % EVENT_PER_LINE) == 0)
					printf("\n ");
			}
		} //end for
		printf("\n");
		printf("\n*****************************\n");
		printf("Events currently in Priority_Queue :: num_elements : %d", heapPriQ_Event.size());
		heapPriQ_Event.print();
		printf("\n*****************************\n");
		numEventProcessed = thrdMon.totalEventProcessed;
		printf("Events processed (current total = %d): \n ", numEventProcessed);
		count = 0;
		for (int ev = 0; ev < numEventProcessed; ev++)
		{
			pEvent = &thrdMon.eventProcessed[ev];
			if (pEvent != NULL)
			{
				pEvent->printEvent();
				if (((ev + 1) % EVENT_PER_LINE) == 0)
					printf("\n ");
			}
		} //end for
		printf("\n");
		if (numEventProcessed >= TOTAL_NUM_EVENTS)
		{
			eventThreadFlag = TERMINATE; // set 1 to terminate threads
			break;
		} //end if
		LeaveCriticalSection(&cs_main);
		Sleep(100);
	} //end for
	DWORD nExitCode = NULL;
	
	WaitForSingleObject(hThrd_EventGenerator, INFINITE);
	EnterCriticalSection(&cs_main);
	printf("Thread_EventGenerator is terminated !!\n");
	LeaveCriticalSection(&cs_main);
	GetExitCodeThread(hThrd_EventGenerator, &nExitCode);
	TerminateThread(hThrd_EventGenerator, nExitCode);
	CloseHandle(hThrd_EventGenerator);
	WaitForSingleObject(hThrd_EventHandler, INFINITE);
	EnterCriticalSection(&cs_main);
	printf("Thread_EventHandler is terminated !!\n");
	LeaveCriticalSection(&cs_main);
	GetExitCodeThread(hThrd_EventHandler, &nExitCode);
	TerminateThread(hThrd_EventHandler, nExitCode);
	CloseHandle(hThrd_EventHandler);
}