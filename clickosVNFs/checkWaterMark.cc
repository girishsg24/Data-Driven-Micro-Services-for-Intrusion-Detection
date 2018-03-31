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

int removeWaterMark(char* pktPtr, int& pktLength,int& position, char& waterMark)
{
    //char* currPtr = pktPtr;
    printf("start: %x",*pktPtr);
    for (int i=0;i<pktLength-1;i++)
    {
        if (i<position)
        {
          pktPtr++;  
        }
        else
        {
            if(i==position)
            {
                printf("\nSent WaterMark: %x Found Mark: %x Position:%d",waterMark, *pktPtr, position);
                if(waterMark != *pktPtr) return 0;
            }
            *pktPtr=*(pktPtr+1);
            pktPtr++;
        }
    }
    pktLength--;
    printf("\nTaking WaterMark: %x at position: %d",waterMark,position);
    return 1;
}

int decode(char* pktPtr,int& pktLen, char* waterMark)
{
    int pos1 = 3;
    int pos2 = 5;
    int pos3 = 7;
    int pos4 = 9;
    int embededMark = removeWaterMark(pktPtr,pktLen,pos4,waterMark[3]);
    if(!embededMark)
    {
       printf("\nWaterMark mismatch at position: %d\n",pos4);
       return 0;
    }
    embededMark = removeWaterMark(pktPtr,pktLen,pos3,waterMark[2]);
    if(!embededMark)
    {
       printf("\nWaterMark mismatch at position: %d\n",pos3);
       return 0;
    }
    embededMark = removeWaterMark(pktPtr,pktLen,pos2,waterMark[1]);
    if(!embededMark)
    {
       printf("\nWaterMark mismatch at position: %d\n",pos2);
       return 0;
    }
    embededMark = removeWaterMark(pktPtr,pktLen,pos1,waterMark[0]);
    if(!embededMark)
    {
       printf("\nWaterMark mismatch at position: %d\n", pos1);
       return 0;
    }
    printf("\nWaterMark Matched, Removing waterMark...\n");
    return 1;
}

Packet* CheckWaterMark::simple_action(Packet *p)
{
  //int len = p->length();
  //WritablePacket *q = p->put(0);
  WritablePacket *q = p->uniqueify();
  int len = q->length();
 // char* test = (char*) q;
  //for(int i=0;i<4;i++,test++)
  //printf("\n%x ",*test);
  //printf("\n");

  char requiredWaterMark[4] = {0x73,0x6a,0x73,0x75};
  if(len < 4)
    goto drop;


  if (!decode((char*)(q->data()),len,requiredWaterMark))
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
