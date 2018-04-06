#include <click/config.h>
#include "setWaterMark.hh"
#include <stdint.h>
#include <stddef.h> 
typedef uint16_t u_int16_t; 
typedef size_t size_type;

CLICK_DECLS
/*
std::string string_to_hex(const std::string& input,size_t& len)
{
    static const char* const lut = "0123456789ABCDEF";

    std::string output;
    output.reserve(3 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
        output.push_back(' ');
    }
    return output;
}
*/
void insertWaterMark(char* pktPtr, int pktLength,u_int16_t position,char waterMark)
{
    char* lastExistingByte = pktPtr+pktLength-1;
    char* spaceByte = pktPtr + pktLength;
    printf("\nwatermark: %x", waterMark);
    printf("\nposition: %x",(int)position);
    printf("\npacketlength: %i",(int)pktLength);
    //start with pktlength because of 1 byte increase
    for (int i=pktLength;i>=0;i--)
    {
        if (i == position)
        {
            *spaceByte = waterMark;
            pktLength++;
            return;
        }
        else
        {
            *spaceByte = *lastExistingByte;
        }
        lastExistingByte--;
        spaceByte--;
    }
    pktLength++;
}

void encode(char* pktPtr, int pktLen, char* waterMark)
{
    char waterMark1 = waterMark[0];
    char waterMark2 = waterMark[1];
    char waterMark3 = waterMark[2];
    char waterMark4 = waterMark[3];
    u_int16_t pos1 = (55);
    u_int16_t pos2 = (60);
    u_int16_t pos3 = (65);
    u_int16_t pos4 = (70);
    
    insertWaterMark(pktPtr,pktLen,pos1,waterMark1);
    insertWaterMark(pktPtr,pktLen+1,pos2,waterMark2);
    insertWaterMark(pktPtr,pktLen+2,pos3,waterMark3);
    insertWaterMark(pktPtr,pktLen+3,pos4,waterMark4);
}

SetWaterMark::SetWaterMark()
{
}

Packet *
SetWaterMark::simple_action(Packet *p)
{
  int len = p->length();
  char waterMark[5] = {0x73,0x6a,0x73,0x75};
 printf("\noriginalLen: %d",len);
  WritablePacket *q = p->put(4);
  encode((char*)q->data(),len, waterMark);
 printf("\nlength: %d\n",q->length());
  return(q);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetWaterMark)
ELEMENT_MT_SAFE(SetWaterMark)
