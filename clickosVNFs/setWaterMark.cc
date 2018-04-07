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
void encode(char* pktPtr, int pktLength,char* waterMark,int& waterMarkLength, u_int16_t* waterMarkPosition)
{
    char* lastExistingByte = pktPtr+pktLength-1;
    char* spaceByte = (pktPtr + pktLength + waterMarkLength) - 1;
    int curPosition = pktLength+waterMarkLength-1;
    for (int j = 0;j<waterMarkLength && curPosition>=0;)
    {
        if (curPosition == *waterMarkPosition)
        {
            *spaceByte = *waterMark;
            waterMarkPosition++;
            waterMark++;
            j++;
        }
        else
        {
            *spaceByte = *lastExistingByte;
            lastExistingByte--;
        }
        spaceByte--;
        curPosition--;
    }
}

SetWaterMark::SetWaterMark()
{
}

Packet* SetWaterMark::simple_action(Packet *p)
{
  int len = p->length();
  char waterMark[5] = {0x73,0x6a,0x73,0x75};
  u_int16_t waterMarkPositions[4] = {70,65,60,55};
  int waterMarkLength = 4;
  struct timeval start, end;
  printf("\nWatermark length: %d",waterMarkLength);
  printf("\nWatermark: %x, %x, %x, %x", waterMark[0], waterMark[1], waterMark[2], waterMark[4]);
  printf("\nWatermark positions: %d, %d, %d, %d", waterMarkPositions[0], waterMarkPositions[1],waterMarkPositions[2],waterMarkPositions[3]);
  printf("\nOriginal packet Len: %d",len);
  WritablePacket *q = p->put(waterMarkLength);
  gettimeofday(&start, NULL);
  encode((char*)q->data(),len,waterMark,waterMarkLength,waterMarkPositions);
  gettimeofday(&end, NULL);
  printf("\nWatermarking Execution time in microseconds %ld", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
  printf("\nNew packet length: %d\n",q->length());
  return(q);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetWaterMark)
ELEMENT_MT_SAFE(SetWaterMark)
