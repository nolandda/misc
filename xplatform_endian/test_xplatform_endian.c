#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "xplatform_endian.h"

static void print_results(char* name, uint32_t errs)
{
  printf("\n");
  if(errs) {
    printf("%s: FAIL - %" PRId32 " total errors\n", name, errs);
  }
  else {
    printf("%s: ALL TESTS PASS\n", name);
  }
  return;
}

static uint32_t test_uint64()
{
  const size_t num_fuzz = 5000;
  uint64_t* ctrl = calloc(num_fuzz, sizeof(uint64_t));
  uint64_t* test = calloc(num_fuzz, sizeof(uint64_t));

  uint8_t* buf = calloc(sizeof(uint64_t), sizeof(uint8_t));
  size_t sz = sizeof(uint64_t);
  size_t i;

  uint32_t errs = 0;
  /* Unit tests */
  const uint64_t t1 = 0xffffffffffffffff;
  const uint64_t t2 = 0x0;
  uint64_t r1;
  uint64_t r2;
  sz = u64_to_be_buf(buf, t1);
  r1 = be_buf_to_u64(buf, sz);
  if( r1 != t1 ) {
    printf("FAIL: UT1\n");
    errs += 1;
  }

  sz = u64_to_be_buf(buf, t2);
  r2 = be_buf_to_u64(buf, sz);
  if( r2 != t2 ) {
    printf("FAIL: UT2\n");
    errs += 1;
  }
  
  for(i=0; i<num_fuzz; i++)
  {
    ctrl[i] = rand()*rand();
    sz = u64_to_be_buf(buf, ctrl[i]);
    test[i] = be_buf_to_u64(buf, sz);
    if( ctrl[i] != test[i] ) {
      printf("FAIL [%u] : 0x%" PRIx64 " != 0x%" PRIx64 "\n", (unsigned int)i+1, ctrl[i], test[i]);
      errs += 1;
    }
    else {
      printf(".");
      if(i && i%40==0) {
	printf("\n");
      }
    }
  }
  print_results("test_uint64", errs);
  printf("\n-----------------------------\n");
  free(buf); buf = 0;
  free(ctrl); ctrl = 0;
  free(test); test = 0;
  return errs;
}


static uint32_t test_int64()
{
  const size_t num_fuzz = 5000;
  int64_t* ctrl = calloc(num_fuzz, sizeof(int64_t));
  int64_t* test = calloc(num_fuzz, sizeof(int64_t));

  uint8_t* buf = calloc(sizeof(int64_t), sizeof(uint8_t));
  size_t sz = sizeof(int64_t);
  size_t i;

  uint32_t errs = 0;
  /* Unit tests */
  const int64_t t1 = 0x7fffffffffffffff;
  const int64_t t2 = 0x0;
  const int64_t t3 = -t1 - 1;

  int64_t r1;
  int64_t r2;
  int64_t r3;

  sz = i64_to_be_buf(buf, t1);
  r1 = be_buf_to_i64(buf, sz);
  if( r1 != t1 ) {
    printf("FAIL: UT1 %" PRId64 " != %" PRId64 "\n", r1, t1);
    errs += 1;
  }

  sz = i64_to_be_buf(buf, t2);
  r2 = be_buf_to_i64(buf, sz);
  if( r2 != t2 ) {
    printf("FAIL: UT2\n");
    errs += 1;
  }

  sz = i64_to_be_buf(buf, t3);
  r3 = be_buf_to_i64(buf, sz);
  if( r3 != t3 ) {
    printf("FAIL: UT3\n");
    errs += 1;
  }
  
  for(i=0; i<num_fuzz; i++)
  {
    ctrl[i] = rand()*rand();
    sz = i64_to_be_buf(buf, ctrl[i]);
    test[i] = be_buf_to_i64(buf, sz);
    if( ctrl[i] != test[i] ) {
      printf("FAIL [%u] : 0x%" PRId64 " != 0x%" PRId64 "\n", (unsigned int)i+1, ctrl[i], test[i]);
      errs += 1;
    }
    else {
      printf(".");
      if(i && i%40==0) {
    	printf("\n");
      }
    }
  }
  print_results("test_int64", errs);
  printf("\n-----------------------------\n");
  free(buf); buf = 0;
  free(ctrl); ctrl = 0;
  free(test); test = 0;
  return errs;
}

/////////////////////////
static uint32_t test_uint32()
{
  const size_t num_fuzz = 5000;
  uint32_t* ctrl = calloc(num_fuzz, sizeof(uint32_t));
  uint32_t* test = calloc(num_fuzz, sizeof(uint32_t));

  uint8_t* buf = calloc(sizeof(uint32_t), sizeof(uint8_t));
  size_t sz = sizeof(uint32_t);
  size_t i;

  uint32_t errs = 0;
  /* Unit tests */
  const uint32_t t1 = 0xffffffff;
  const uint32_t t2 = 0x0;
  uint32_t r1;
  uint32_t r2;
  sz = u32_to_be_buf(buf, t1);
  r1 = be_buf_to_u32(buf, sz);
  if( r1 != t1 ) {
    printf("FAIL: UT1\n");
    errs += 1;
  }

  sz = u32_to_be_buf(buf, t2);
  r2 = be_buf_to_u32(buf, sz);
  if( r2 != t2 ) {
    printf("FAIL: UT2\n");
    errs += 1;
  }
  
  for(i=0; i<num_fuzz; i++)
  {
    ctrl[i] = rand()*rand();
    sz = u32_to_be_buf(buf, ctrl[i]);
    test[i] = be_buf_to_u32(buf, sz);
    if( ctrl[i] != test[i] ) {
      printf("FAIL [%u] : 0x%" PRIx32 " != 0x%" PRIx32 "\n", (unsigned int)i+1, ctrl[i], test[i]);
      errs += 1;
    }
    else {
      printf(".");
      if(i && i%40==0) {
	printf("\n");
      }
    }
  }
  print_results("test_uint32", errs);
  printf("\n-----------------------------\n");
  free(buf); buf = 0;
  free(ctrl); ctrl = 0;
  free(test); test = 0;
  return errs;
}


static uint32_t test_int32()
{
  const size_t num_fuzz = 5000;
  int32_t* ctrl = calloc(num_fuzz, sizeof(int32_t));
  int32_t* test = calloc(num_fuzz, sizeof(int32_t));

  uint8_t* buf = calloc(sizeof(int32_t), sizeof(uint8_t));
  size_t sz = sizeof(int32_t);
  size_t i;

  uint32_t errs = 0;
  /* Unit tests */
  const int32_t t1 = 0x7fffffff;
  const int32_t t2 = 0x0;
  const int32_t t3 = -t1 - 1;

  int32_t r1;
  int32_t r2;
  int32_t r3;

  sz = i32_to_be_buf(buf, t1);
  r1 = be_buf_to_i32(buf, sz);
  if( r1 != t1 ) {
    printf("FAIL: UT1 %" PRId32" != %" PRId32 "\n", r1, t1);
    errs += 1;
  }

  sz = i32_to_be_buf(buf, t2);
  r2 = be_buf_to_i32(buf, sz);
  if( r2 != t2 ) {
    printf("FAIL: UT2\n");
    errs += 1;
  }

  sz = i32_to_be_buf(buf, t3);
  r3 = be_buf_to_i32(buf, sz);
  if( r3 != t3 ) {
    printf("FAIL: UT3\n");
    errs += 1;
  }
  
  for(i=0; i<num_fuzz; i++)
  {
    ctrl[i] = rand()*rand();
    sz = i32_to_be_buf(buf, ctrl[i]);
    test[i] = be_buf_to_i32(buf, sz);
    if( ctrl[i] != test[i] ) {
      printf("FAIL [%u] : %" PRId32 " != %" PRId32 "\n", (unsigned int)i+1, ctrl[i], test[i]);
      errs += 1;
    }
    else {
      printf(".");
      if(i && i%40==0) {
    	printf("\n");
      }
    }
  }
  print_results("test_int32", errs);
  printf("\n-----------------------------\n");
  free(buf); buf = 0;
  free(ctrl); ctrl = 0;
  free(test); test = 0;
  return errs;
}


/////////////////////////
static uint32_t test_uint16()
{
  const size_t num_fuzz = 5000;
  uint16_t* ctrl = calloc(num_fuzz, sizeof(uint16_t));
  uint16_t* test = calloc(num_fuzz, sizeof(uint16_t));

  uint8_t* buf = calloc(sizeof(uint16_t), sizeof(uint8_t));
  size_t sz = sizeof(uint16_t);
  size_t i;

  uint32_t errs = 0;
  /* Unit tests */
  const uint16_t t1 = 0xffff;
  const uint16_t t2 = 0x0;
  uint16_t r1;
  uint16_t r2;
  sz = u16_to_be_buf(buf, t1);
  r1 = be_buf_to_u16(buf, sz);
  if( r1 != t1 ) {
    printf("FAIL: UT1\n");
    errs += 1;
  }

  sz = u16_to_be_buf(buf, t2);
  r2 = be_buf_to_u16(buf, sz);
  if( r2 != t2 ) {
    printf("FAIL: UT2\n");
    errs += 1;
  }
  
  for(i=0; i<num_fuzz; i++)
  {
    ctrl[i] = rand()*rand();
    sz = u16_to_be_buf(buf, ctrl[i]);
    test[i] = be_buf_to_u16(buf, sz);
    if( ctrl[i] != test[i] ) {
      printf("FAIL [%u] : 0x%" PRIx16 " != 0x%" PRIx16 "\n", (unsigned int)i+1, ctrl[i], test[i]);
      errs += 1;
    }
    else {
      printf(".");
      if(i && i%40==0) {
	printf("\n");
      }
    }
  }
  print_results("test_uint16", errs);
  printf("\n-----------------------------\n");
  free(buf); buf = 0;
  free(ctrl); ctrl = 0;
  free(test); test = 0;
  return errs;
}


static uint32_t test_int16()
{
  const size_t num_fuzz = 5000;
  int16_t* ctrl = calloc(num_fuzz, sizeof(int16_t));
  int16_t* test = calloc(num_fuzz, sizeof(int16_t));

  uint8_t* buf = calloc(sizeof(int16_t), sizeof(uint8_t));
  size_t sz = sizeof(int16_t);
  size_t i;

  uint32_t errs = 0;
  /* Unit tests */
  const int16_t t1 = 0x7fff;
  const int16_t t2 = 0x0;
  const int16_t t3 = -t1 - 1;

  int16_t r1;
  int16_t r2;
  int16_t r3;

  sz = i16_to_be_buf(buf, t1);
  r1 = be_buf_to_i16(buf, sz);
  if( r1 != t1 ) {
    printf("FAIL: UT1 %" PRId16 " != %" PRId16 "\n", r1, t1);
    errs += 1;
  }

  sz = i16_to_be_buf(buf, t2);
  r2 = be_buf_to_i16(buf, sz);
  if( r2 != t2 ) {
    printf("FAIL: UT2\n");
    errs += 1;
  }

  sz = i16_to_be_buf(buf, t3);
  r3 = be_buf_to_i16(buf, sz);
  if( r3 != t3 ) {
    printf("FAIL: UT3\n");
    errs += 1;
  }
  
  for(i=0; i<num_fuzz; i++)
  {
    ctrl[i] = rand()*rand();
    sz = i16_to_be_buf(buf, ctrl[i]);
    test[i] = be_buf_to_i16(buf, sz);
    if( ctrl[i] != test[i] ) {
      printf("FAIL [%u] : %" PRId16 " != %" PRId16 "\n", (unsigned int)i+1, ctrl[i], test[i]);
      errs += 1;
    }
    else {
      printf(".");
      if(i && i%40==0) {
    	printf("\n");
      }
    }
  }
  print_results("test_int16", errs);
  printf("\n-----------------------------\n");
  free(buf); buf = 0;
  free(ctrl); ctrl = 0;
  free(test); test = 0;
  return errs;
}



int main(int argc, char** argv)
{
  int result = 0;
  argc = argc; argv=argv;

  result = test_uint64();
  result += test_int64();
  result += test_uint32();
  result += test_int32();
  result += test_uint16();
  result += test_int16();

  printf("\n-----------------------------");
  print_results("TOTAL_TESTSUITE:", result);
  printf("-----------------------------\n");

  return result;
}
