#ifndef CLICK_SETWATERMARK_HH
#define CLICK_SETWATERMARK_HH
#include <click/element.hh>
#include <sys/time.h> 
#include <stdlib.h>
#include <time.h>
CLICK_DECLS

class SetWaterMark : public Element { public:

  SetWaterMark();

  const char *class_name() const	{ return "SetWaterMark"; }
  const char *port_count() const	{ return PORTS_1_1; }

  Packet *simple_action(Packet *);

};

CLICK_ENDDECLS
#endif
