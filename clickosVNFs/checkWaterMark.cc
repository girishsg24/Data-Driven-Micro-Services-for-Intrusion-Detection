/*
 * 
 * Robert Morris
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include "checkWaterMark.hh"
CLICK_DECLS

CheckWaterMark::CheckWaterMark()
{
  _drops = 0;
}

Packet *
CheckWaterMark::simple_action(Packet *p)
{
  unsigned int waterMark;

  int len = p->length();
  if(len < 4)
    goto drop;
  waterMark=0xfffcfcff;
  unsigned int pWaterMark;
  memcpy(&pWaterMark, p->data() + len - 4, 4);
  if(pWaterMark != waterMark)
    goto drop;

  p->take(4);
  return p;

 drop:
  click_chatter("WATERMARK failed, len %d",
              p->length());
  p->kill();
  _drops++;
  return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CheckWaterMark)
ELEMENT_MT_SAFE(CheckWaterMark)
