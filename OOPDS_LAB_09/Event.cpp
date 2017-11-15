#include "Event.hpp"

ostream & operator<<(ostream & fout, const Event & e)
{
	cout << "Ev(pri: " << setw(2) << e.event_pri << ", src: " << setw(2) << e.event_gen_addr << ", id:" << setw(3) << e.event_no << ")";
	return fout;
}

Event::Event()
{
	this->event_no = -1;
	this->event_gen_addr = -1;
	this->event_handler_addr = -1;
	this->event_pri = -1;
	this->eventStatus = UNDEFINED;
}

Event::Event(int event_id, int event_pri, int srcAddr)
{
	this->event_no = event_id;
	this->event_gen_addr = srcAddr;
	this->event_handler_addr = -1;
	this->event_pri = event_pri;
	this->eventStatus = GENERATED;
}

void Event::printEvent()
{
	cout << "Ev(pri: " << setw(2) << event_pri << ", src: " << setw(2) << event_gen_addr << ", id:" << setw(3) << event_no << ")";
}
