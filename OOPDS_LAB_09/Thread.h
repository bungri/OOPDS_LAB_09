#ifndef THREAD_H
#define THREAD_H

#include "HeapPriorityQueue.hpp"
#include "SimParams.h"

enum ROLE {
	EVENT_GENERATOR,
	EVENT_HANDLER
};

enum THREAD_FLAG {
	INITIALIZE,
	RUN,
	TERMINATE
};

typedef struct ThreadStatusMonitor
{
	int numEventGenerated;
	int numEventProcessed;
	int totalEventGenerated;
	int totalEventProcessed;
	Event eventGenerated[TOTAL_NUM_EVENTS]; // used for monitoring only
	Event eventProcessed[TOTAL_NUM_EVENTS]; // used for monitoring only
	THREAD_FLAG *pFlagThreadTerminate;
}ThreadStatusMonitor;
typedef struct ThreadParam
{
	CRITICAL_SECTION *pCS_main;
	CRITICAL_SECTION *pCS_thrd_mon;
	HeapPriorityQueue<Event> *pPriQ_Event;
	ROLE role;
	int myAddr;
	int maxRound;
	int targetEventGen;
	ThreadStatusMonitor *pThrdMon;
}ThreadParam_Event;

DWORD WINAPI Thread_EventHandler(LPVOID pParam);
DWORD WINAPI Thread_EventGenerator(LPVOID pParam);

#endif
