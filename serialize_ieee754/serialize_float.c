#include <stdint.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "../xplatform_endian/xplatform_endian.h"
#include "serialize_float.h"

void FlattenDouble(int64_t* mantissa, int16_t* exponent, double number)
{
  int exp;
  int64_t mant;
  const int64_t MAXVAL = 0x7FFFFFFFFFFFFFFF;
  double fractional_mant;
  fractional_mant = frexp(number, &exp);
  // The truly paranoid could test if exp fits in 16-bits here,
  // but IEEE 754 says it is 11-bits long so should be safe.
  // TODO: NOT A NUMBER
  //if( isnan( fractional_mant ) )
  mant = MAXVAL * fractional_mant;

  *mantissa = mant;
  *exponent = (int16_t)exp;
  return;
}

double UnflattenDouble(double* number, int64_t mantissa, int16_t exponent)
{
  const int64_t MAXVAL = 0x7FFFFFFFFFFFFFFF;
  double fractional_mant = ((double)mantissa) / MAXVAL;
  // TODO Not a number
  double result = ldexp(fractional_mant, (int)exponent); 
  if( number )
  {
    *number = result;
  }
  return result;
}

void FlattenFloat(int32_t* mantissa, int8_t* exponent, float number)
{
  int exp;
  int32_t mant;
  const int32_t MAXVAL = 0x7FFFFFFF;
  float fractional_mant;
  fractional_mant = frexpf(number, &exp);
  // TODO: NOT A NUMBER
  //if( isnan( fractional_mant ) )
  mant = MAXVAL * fractional_mant;

  *mantissa = mant;
  // IEEE 754 exponents can be [-126,127] while int8_t
  // contains [-128, 127]
  *exponent = (int8_t)exp;
  return;
}

float UnflattenFloat(float* number, int32_t mantissa, int8_t exponent)
{
  const int32_t MAXVAL = 0x7FFFFFFF;
  float fractional_mant = ((float)mantissa) / MAXVAL;
  // TODO: Not a number
  float result = ldexpf(fractional_mant, (int)exponent);  
  if( number ) 
  {
    *number = result;
  }
  return result;
}

/* static void print_buf(const uint8_t* buf, size_t len) */
/* { */
/*   size_t i = 0; */
/*   for(i=0; i<len; i+=1) */
/*   { */
/*     printf("%02x", buf[i]); */
/*   } */
/*   return; */
/* }  */

size_t SerializeDouble(uint8_t* buf, double number)
{
  int64_t mant;
  int16_t exp;
  size_t result = 0;
  FlattenDouble(&mant, &exp, number);
  //printf("ser_in: %g = 0x%llx, 0x%x\n", number, (uint64_t)mant, exp);
  result = i64_to_be_buf(buf, mant);
  result += i16_to_be_buf(buf+result, exp);  
  //printf("ser_out (%d): ", result);
  //print_buf(buf, result);
  //printf("\n");
  return result;
}

double DeserializeDouble(const uint8_t* buf, size_t len)
{
  int64_t mant;
  int16_t exp;
  double result = 0.0;
  mant = be_buf_to_i64(buf, len);
  exp = be_buf_to_i16(buf+sizeof(int64_t), len-sizeof(int64_t));
  result = UnflattenDouble(NULL, mant, exp);
  return result;
}

size_t SerializeFloat(uint8_t* buf, float number)
{
  int32_t mant;
  int8_t exp;
  size_t result = 0;
  FlattenFloat(&mant, &exp, number);
  //printf("ser_in: %g = 0x%llx, 0x%x\n", number, (uint64_t)mant, exp);
  result = i32_to_be_buf(buf, mant);
  buf[result] = exp;
  result += sizeof(uint8_t);
  //printf("ser_out (%d): ", result);
  //print_buf(buf, result);
  //printf("\n");
  return result;
}

float DeserializeFloat(const uint8_t* buf, size_t len)
{
  int32_t mant;
  int8_t exp;
  float result = 0.0;
  mant = be_buf_to_i32(buf, len);
  exp = buf[sizeof(uint32_t)];
  result = UnflattenFloat(NULL, mant, exp);
  return result;
}

