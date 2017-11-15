#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <iomanip>

using namespace std;

typedef unsigned int UINT_32;
typedef unsigned short UINT_16;
typedef unsigned char UINT_8;

#define NUM_PRIORITY 100
#define EVENT_PER_LINE 5

enum EventStatus {
	GENERATED,
	ENQUEUED,
	PROCESSED,
	UNDEFINED
};

class Event
{
	friend ostream& operator<<(ostream& fout, const Event& e);
public:
	Event(); // default constructor
	Event(int event_id, int event_pri, int srcAddr); //constructor
	void printEvent();
	void setEventNo(int evt_no) {event_no = evt_no;}; // mark as not-processed
	void setEventPri(int evt_pri) { event_pri = evt_pri; };
	int getEventPri() { return event_pri; }
	int getEventNo() { return event_no; }
	bool operator>(Event& e) { return (event_pri > e.event_pri); }
	bool operator<(Event& e) { return (event_pri < e.event_pri); }
private:
	int event_no;
	int event_gen_addr;
	int event_handler_addr;
	int event_pri; // event_priority
	EventStatus eventStatus;
};

#endif