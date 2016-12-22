
#include "xplatform_endian.h"

size_t u8_to_be_buf(uint8_t* buf, uint8_t num) 
{
  buf[0] = num;
  return sizeof(uint8_t);
}

size_t i8_to_be_buf(uint8_t* buf, int8_t num)
{
  uint8_t tmp;
  uint8_t sign = 0x00;
  if( num < 0 )
  {
    sign = 0x80;
    if(num == INT8_MIN) {
      num = 0;
    }
    else {
      num = num * -1;
    }
  }
  tmp = (uint8_t)num;
  u8_to_be_buf(buf, tmp);
  buf[0] = buf[0] | sign;
  return sizeof(int8_t);
}

size_t u16_to_be_buf(uint8_t* buf, uint16_t num)
{
  buf[0] = ((num & 0xff00) >> 8);
  buf[1] = (num & 0xff);
  return sizeof(uint16_t);
}

size_t i16_to_be_buf(uint8_t* buf, int16_t num)
{
  uint16_t tmp;
  uint8_t sign = 0x00;
  if( num < 0 )
  {
    sign = 0x80;
    if(num == INT16_MIN) {
      num = 0;
    }
    else {
      num = num * -1;
    }
  }
  tmp = (uint16_t)num;
  u16_to_be_buf(buf, tmp);
  buf[0] = buf[0] | sign;
  return sizeof(int16_t);
}

size_t u32_to_be_buf(uint8_t* buf, uint32_t num)
{
  buf[0] = ((num & 0xff000000) >> 24);
  buf[1] = ((num & 0xff0000) >> 16);
  buf[2] = ((num & 0xff00) >> 8);
  buf[3] = (num & 0xff);
  return sizeof(uint32_t);
}

size_t i32_to_be_buf(uint8_t* buf, int32_t num)
{
  uint32_t tmp;
  uint8_t sign = 0x00;
  if( num < 0 )
  {
    sign = 0x80;
    if(num == INT32_MIN) {
      num = 0;
    }
    else {
      num = num * -1;
    }
  }
  tmp = (uint32_t)num;
  u32_to_be_buf(buf, tmp);
  buf[0] = buf[0] | sign;
  return sizeof(int32_t);
}


size_t u64_to_be_buf(uint8_t* buf, uint64_t num)
{
  uint64_t mask = 0xff00000000000000;
  unsigned int sval = 7*8;
  unsigned int i;
  for(i=0; i<8; i+=1)
  {    
    buf[i] = (num & mask) >> sval;
    mask = mask >> 8;
    sval -= 8;
  }
  return sizeof(uint64_t);
}

size_t i64_to_be_buf(uint8_t* buf, int64_t num)
{
  uint64_t tmp;
  uint8_t sign = 0x00;
  if( num < 0 )
  {
    sign = 0x80;
    if(num == INT64_MIN) {
      num = 0;
    }
    else {
      num = num * -1;
    }
  }
  tmp = (uint64_t)num;
  u64_to_be_buf(buf, tmp);
  buf[0] = buf[0] | sign;
  return sizeof(int64_t);
}

uint8_t be_buf_to_u8(const uint8_t* buf, size_t len)
{
  uint8_t result = 0;
  if( len >= 1 ) 
  {
    result = buf[0];
  }
  return result;
}

int8_t be_buf_to_i8(const uint8_t* buf, size_t len)
{
  int8_t sign = 1;
  uint8_t tmp = 0;
  int8_t result = 0;
  uint8_t* pbuf = (uint8_t*)buf; 
  uint8_t old = pbuf[0];
  if( pbuf[0] & 0x80 ) {
    pbuf[0] = pbuf[0] ^ 0x80;
    sign = -1;
  }
  tmp = be_buf_to_u8(pbuf,len);
  if((!tmp) && (sign<0)) {
    // Special case INT8_MIN
    result = -(0x7f) - 1;
  }
  else {
    result = sign * ((int8_t)tmp);
  }
  pbuf[0] = old;
  return result;
}

uint16_t be_buf_to_u16(const uint8_t* buf, size_t len)
{
  uint16_t result = 0;
  if( len >= 2 ) 
  {
    result = buf[0];
    result = result << 8;
    result = result | buf[1];
  }
  return result;
}

int16_t be_buf_to_i16(const uint8_t* buf, size_t len)
{
  int16_t sign = 1;
  uint16_t tmp = 0;
  int16_t result = 0;
  uint8_t* pbuf = (uint8_t*)buf; 
  uint8_t old = pbuf[0];
  if( pbuf[0] & 0x80 ) {
    pbuf[0] = pbuf[0] ^ 0x80;
    sign = -1;
  }
  tmp = be_buf_to_u16(pbuf,len);
  if((!tmp) && (sign<0)) {
    // Special case INT16_MIN (-32,786)
    result = -(0x7fff) - 1;
  }
  else {
    result = sign * ((int16_t)tmp);
  }
  pbuf[0] = old;
  return result;
}

uint32_t be_buf_to_u32(const uint8_t* buf, size_t len)
{
  uint32_t result = 0;
  if( len >= 4 )
  {
    result = buf[0];
    result = result << 8;
    result = result | buf[1];
    result = result << 8;
    result = result | buf[2];
    result = result << 8;
    result = result | buf[3];
  }
  return result;
}

int32_t be_buf_to_i32(const uint8_t* buf, size_t len)
{
  int32_t sign = 1;
  uint32_t tmp = 0;
  int32_t result = 0;
  uint8_t* pbuf = (uint8_t*)buf; 
  uint8_t old = pbuf[0];
  if( pbuf[0] & 0x80 ) {
    pbuf[0] = pbuf[0] ^ 0x80;
    sign = -1;
  }
  tmp = be_buf_to_u32(pbuf,len);
  if((!tmp) && (sign<0)) {
    // Special case INT32_MIN 
    result = -(0x7fffffff) - 1;
  }
  else {
    result = sign * ((int32_t)tmp);
  }
  pbuf[0] = old;
  return result;
}

uint64_t be_buf_to_u64(const uint8_t* buf, size_t len)
{
  uint64_t result = 0;
  if(len >= 8 ) 
  {
    result = buf[0];
    result = result << 8;
    result = result | buf[1];
    result = result << 8;
    result = result | buf[2];
    result = result << 8;
    result = result | buf[3];
    result = result << 8;
    result = result | buf[4];
    result = result << 8;
    result = result | buf[5];
    result = result << 8;
    result = result | buf[6];
    result = result << 8;
    result = result | buf[7];
  }
  return result;
}

int64_t be_buf_to_i64(const uint8_t* buf, size_t len)
{
  int64_t sign = 1;
  uint64_t tmp = 0;
  int64_t result = 0;
  uint8_t* pbuf = (uint8_t*)buf; 
  uint8_t old = pbuf[0];
  if( pbuf[0] & 0x80 ) {
    pbuf[0] = pbuf[0] ^ 0x80;
    sign = -1;
  }
  tmp = be_buf_to_u64(pbuf,len);
  if((!tmp) && (sign<0)) {
    // Special case INT64_MIN
    result = -(0x7fffffffffffffff) - 1;
  }
  else {
    result = sign * ((int64_t)tmp);
  }
  pbuf[0] = old;
  return result;
}
