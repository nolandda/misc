#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "serialize_float.h"

double rand_dbl_bounded(double M, double N)
{
  int choice = rand()%2;
  double v = 0.0;
  if( choice ) 
  {
    // Rationals
    v = ((double)rand() / (double)RAND_MAX); 
    return M + v*(N-M);  
  }
  else 
  {
    // Mostly Irationals
    double v = sin( (double)rand() );
    return M + v*(N-M);
  }
}


float rand_flt_bounded(float M, float N)
{
  int choice = rand()%2;
  float v = 0.0;
  if( choice ) 
  {
    // Rationals
    v = ((float)rand() / (float)RAND_MAX); 
    return M + v*(N-M);  
  }
  else 
  {
    // Mostly irrationals
    float v = sinf( (float)rand() );
    return M + v*(N-M);
  }
}

uint32_t double_fuzz_range(double low, double high, size_t count) {
  const size_t RIGHT_SZ = sizeof(int64_t) + sizeof(int16_t);
  double* dbls_ctrl = calloc(count, sizeof(double));
  double* dbls_test = calloc(count, sizeof(double));
  double diff = 0.0;
  uint8_t* buf = calloc(RIGHT_SZ, sizeof(uint8_t));
  size_t sz = 0;
  size_t i;
  uint32_t errs = 0;

  if( low < high ) {
    for(i=0; i<count; i++)
    {
      dbls_ctrl[i] = rand_dbl_bounded(low, high);
      sz = SerializeDouble(buf, dbls_ctrl[i]);
      if( sz == RIGHT_SZ)
      {
	dbls_test[i] = DeserializeDouble(buf, sz);
	diff = abs(dbls_ctrl[i] - dbls_test[i]);
	if( diff != 0.0 ) {
	  // Yes. Exact comparison of doubles. This is not a drill!
	  printf("dbl_small[%u] : %g-%g = %g\n", (unsigned int)i+1, dbls_ctrl[i], dbls_test[i], diff);
	  errs += 1;
	}
      }
      else {
	printf("Err serialize size[%zd]: %zd != %zd", i, sz, RIGHT_SZ);
	errs += 1;
      }
    }
  }
  else {
    printf("Bad Range low %g >= high %g\n", low, high);
    errs += 1;
  }
  free(buf); buf = 0;
  free(dbls_ctrl); dbls_ctrl = 0;
  free(dbls_test); dbls_test = 0;
  return errs;
}


uint32_t float_fuzz_range(float low, float high, size_t count) {
  const size_t RIGHT_SZ = sizeof(int32_t) + sizeof(int8_t);
  float* flts_ctrl = calloc(count, sizeof(float));
  float* flts_test = calloc(count, sizeof(float));
  float diff = 0.0;
  uint8_t* buf = calloc(RIGHT_SZ, sizeof(uint8_t));
  size_t sz = 0;
  size_t i;
  uint32_t errs = 0;
  if( low < high ) {
    for(i=0; i<count; i++)
    {
      flts_ctrl[i] = rand_flt_bounded(low, high);
      sz = SerializeFloat(buf, flts_ctrl[i]);
      if( sz == RIGHT_SZ)
      {
	flts_test[i] = DeserializeFloat(buf, sz);
	diff = abs(flts_ctrl[i] - flts_test[i]);
	if( diff != 0.0 ) {
	  // Yes. Exact comparison of floats. This is not a drill!
	  printf("flt_small[%u] : %f-%f = %f\n", (unsigned int)i+1, flts_ctrl[i], flts_test[i], diff);
	  errs += 1;
	}
      }
      else {
	printf("Err serialize size[%zd]: %zd != %zd", i, sz, RIGHT_SZ);
	errs += 1;
      }
    }
  }
  else {
    printf("Bad Range low %f >= high %f\n", low, high);
    errs += 1;
  }
  free(buf); buf = 0;
  free(flts_ctrl); flts_ctrl = 0;
  free(flts_test); flts_test = 0;
  return errs;
}

uint32_t test_doubles()
{
  size_t num_dbls = 5000;
  uint32_t errs = 0;

  errs += double_fuzz_range(-1.0, 1.0, num_dbls);
  errs += double_fuzz_range(-5000000000.0, 5000000000.0, num_dbls);
  
  return errs;
}


uint32_t test_floats()
{
  size_t num_flts = 5000;
  uint32_t errs = 0;

  errs += float_fuzz_range(-1.0, 1.0, num_flts);
  errs += float_fuzz_range(-5000000000.0, 5000000000.0, num_flts);
  
  return errs;
}



int main(int argc, char** argv)
{
  int result = 0;
  argc = argc; argv=argv;

  result += test_doubles();
  result += test_floats();
  if(result == 0) {
    printf("All Tests Pass\n");
  }
  else {
    printf("FAIL: %d errors\n", result);
  }

  return result;
}
