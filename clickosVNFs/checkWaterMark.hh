#ifndef CLICK_CHECKWATERMARK_HH
#define CLICK_CHECKWATERMARK_HH
#include <click/element.hh>
#include <click/atomic.hh>
CLICK_DECLS

class CheckWaterMark : public Element { public:

    CheckWaterMark();

    const char *class_name() const		{ return "CheckWaterMark"; }
    const char *port_count() const		{ return PORTS_1_1; }

    Packet *simple_action(Packet *);

  private:

    atomic_uint32_t _drops;

};

CLICK_ENDDECLS
#endif
