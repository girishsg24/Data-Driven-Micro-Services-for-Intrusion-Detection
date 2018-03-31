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

u_char removeWaterMark(u_char* pktPtr, int& pktLength,u_int16_t position)
{
    u_char* currPtr = pktPtr;
    char waterMark;
    for (int i=0;i<pktLength-1;i++)
    {
        if (i<position)
        {
          currPtr++;  
        }
        else
        {
            if(i==position)
            {
                waterMark = *currPtr;
            }
            *currPtr=*(currPtr+1);
            currPtr++;
        }
    }
    pktLength--;
    return waterMark;
}

int decode(u_char* pktPtr,int& pktLen, char* waterMark)
{
    u_int16_t pos1 = 3;
    u_int16_t pos2 = 5;
    u_int16_t pos3 = 7;
    u_int16_t pos4 = 9;
    string pktHex = "";
    if(removeWaterMark(pktPtr,pktLen,pos4)!=waterMark[3]) return 0;
    if(removeWaterMark(pktPtr,pktLen,pos3)!=waterMark[2]) return 0;
    if(removeWaterMark(pktPtr,pktLen,pos2)!=waterMark[1]) return 0;
    if(removeWaterMark(pktPtr,pktLen,pos1)!=waterMark[0]) return 0;
    return 1;
}

Packet* CheckWaterMark::simple_action(Packet *p)
{
  if(len < 4)
    goto drop;
  int len = p->length();
  WritablePacket *q = p->put(0);
  char requiredWaterMark[4] = {0x73,0x6a,0x73,0x75};
  if (!decode(q,len,requiredWaterMark))
    goto drop;
  q->take(4);
  return q;  
 drop:
  click_chatter("WATERMARK failed, len %d", p->length());
  p->kill();
  q->kill();
  _drops++;
  return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CheckWaterMark)
ELEMENT_MT_SAFE(CheckWaterMark)
