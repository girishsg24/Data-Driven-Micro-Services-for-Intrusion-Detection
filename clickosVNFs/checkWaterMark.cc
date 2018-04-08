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
#include <stdint.h> 
#include <stddef.h> 
typedef uint16_t u_int16_t;
CLICK_DECLS

CheckWaterMark::CheckWaterMark()
{
  _drops = 0;
}

int decode(char* pktPtr, int& pktLength, char* waterMark, int& waterMarkLength, u_int16_t* waterMarkPosition)
{
  int hopNum = 0;
  int waterMarksLeft = waterMarkLength;
  for(int i =0;i<waterMarkLength;i++)
  {
    if(*(pktPtr+waterMarkPosition[i])!=waterMark[i])
    {
      printf("watermark failed at position: %d, looking for %x found: %x",waterMarkPosition[i],waterMark[i],*(pktPtr+waterMarkPosition[i]));
      return 0;
    }
  }
  printf("Water mark found, taking watermarks");  
  pktPtr = pktPtr + (*waterMarkPosition);
  printf("\n%x",*pktPtr);
  for(u_int16_t curPosition = *waterMarkPosition;curPosition<pktLength-waterMarkLength;curPosition++)
  {
    while(waterMarksLeft > 0 && curPosition+hopNum == *waterMarkPosition)
    {  
       hopNum++;
       waterMarksLeft--;
       waterMarkPosition++;
    }
    *pktPtr = *(pktPtr+hopNum);
    pktPtr++;
  }
  return 1;
}
       
Packet* CheckWaterMark::simple_action(Packet *p)
{
  WritablePacket *q = p->uniqueify();
  int len = q->length();
  printf("\nlength:%d\n",len);
  char waterMark[4] = {0x75,0x73,0x6a,0x73};
  int waterMarkLength = 4;
  u_int16_t waterMarkPosition[4]={55,60,65,70};
  if (!decode((char*)(q->data()), len, waterMark, waterMarkLength, waterMarkPosition))
    goto drop;
  q->take(waterMarkLength);
  printf("New packet length: %d",q->length());
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
