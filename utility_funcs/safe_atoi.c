/** 
 * @author Dan Noland <nolandda@nolandda.org>
 */

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

int safe_atoi(int* intvalptr, const char* str) {
  char *endptr = NULL;
  long int candidate = 0;
  int result = -1;
  if( str && intvalptr ) 
  {
    errno = 0;
    // Set zero as the base to parse hex/dec/octal 
    // numbers based on prefix (0x or 0).
    candidate = strtol(str, &endptr, 0);

    // Check everything that can go wrong
    if((endptr != str) && (errno != ERANGE) && 
       (candidate >= INT_MIN) && (candidate <= INT_MAX))
    {
      *intvalptr = (int)candidate;
      result = 0; // Conversion OK
    }
  }  
  return result;
} 


