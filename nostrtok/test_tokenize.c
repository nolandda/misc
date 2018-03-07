#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>

#include "tokenize.h"

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

uint32_t check_token_test_results(size_t rv, tokset_t* set, 
				  const char** good, size_t count,
				  const char* F, int L) {
  uint32_t errs = 0;
  size_t i = 0;
  if( rv == 0 ) {
    printf("Error: False Negative %s:%d\n", F, L);
    errs += 1;
  }
  else {
    if( rv != count ) {
      printf("Error: Bad token count %zu != %zu at %s:%d\n", rv, count, F, L);
      errs += 1;
    }
    for(i=0; good[i]!=NULL; i+=1) {
      const char* cur = get_next_token(set);
      if( strcmp(cur, good[i]) != 0) {
	printf("Error: Token mismatch [%s] != [%s] at %s:%d\n", cur, good[i], F, L);
      }
    }
      
  }
  return errs;
}

static uint32_t test_tokenize()
{
  uint32_t errs = 0;
  size_t rv = 0;
  tokset_t set = {0};

  // First negative test cases
  rv = create_tokens( &set, NULL, NULL, 0 );
  if( rv != 0 ) {
    printf("Error: False Positive %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }

  //////////////////////////////////////
  // Now positive testcases
  //////////////////////////////////////

  // Testcase one
  {
    const char* CASE_STR = "The rain in spain falls mainly in the plain.";
    const char* CASE_SEP = " ";
    const char* KNOWN_GOOD[] = {"The", "rain", "in", "spain", "falls", "mainly", "in", "the", "plain.", 0}; 
    const size_t TCOUNT = sizeof(KNOWN_GOOD)/sizeof(char*) - 1;

    rv = create_tokens( &set, CASE_STR, CASE_SEP, 0 );
    //print_tokens( &set );
    errs += check_token_test_results(rv, &set, KNOWN_GOOD, TCOUNT, __FILE__, __LINE__);
    free_tokens( &set );
  }


  // Testcase two - Trim with initial null tokens
  {
    const char* CASE_STR = "      The  rain in  spain falls     mainly in the                  plain.";
    const char* CASE_SEP = " ";
    const char* KNOWN_GOOD[] = {"The", "rain", "in", "spain", "falls", "mainly", "in", "the", "plain.", 0}; 
    const size_t TCOUNT = sizeof(KNOWN_GOOD)/sizeof(char*) - 1;

    rv = create_tokens( &set, CASE_STR, CASE_SEP, 1 );
    //print_tokens( &set );
    errs += check_token_test_results(rv, &set, KNOWN_GOOD, TCOUNT, __FILE__, __LINE__);
    free_tokens( &set );
  }


  // Testcase three - Trim with trailing null tokens
  {
    const char* CASE_STR = "The  rain in  spain falls     mainly in zzz                  plain.    ";
    const char* CASE_SEP = " ";
    const char* KNOWN_GOOD[] = {"The", "rain", "in", "spain", "falls", "mainly", "in", "zzz", "plain.", 0}; 
    const size_t TCOUNT = sizeof(KNOWN_GOOD)/sizeof(char*) - 1;

    rv = create_tokens( &set, CASE_STR, CASE_SEP, 1 );
    //print_tokens( &set );
    errs += check_token_test_results(rv, &set, KNOWN_GOOD, TCOUNT, __FILE__, __LINE__);
    free_tokens( &set );
  }

  // Testcase Four - Delimitors do not appear
  {
    const char* CASE_STR = "The rain in spain falls mainly in the plain.";
    const char* CASE_SEP = "!$|";
    const char* KNOWN_GOOD[] = {"The rain in spain falls mainly in the plain.", 0};
    const size_t TCOUNT = sizeof(KNOWN_GOOD)/sizeof(char*) - 1;

    rv = create_tokens( &set, CASE_STR, CASE_SEP, 1 );
    //print_tokens( &set );
    errs += check_token_test_results(rv, &set, KNOWN_GOOD, TCOUNT, __FILE__, __LINE__);
    free_tokens( &set );
  }


  // Testcase Five - Empty string, no trim
  {
    const char* CASE_STR = "";
    const char* CASE_SEP = " |,";
    const char* KNOWN_GOOD[] = {"", 0};
    const size_t TCOUNT = sizeof(KNOWN_GOOD)/sizeof(char*) - 1;

    rv = create_tokens( &set, CASE_STR, CASE_SEP, 0 );
    //print_tokens( &set );
    errs += check_token_test_results(rv, &set, KNOWN_GOOD, TCOUNT, __FILE__, __LINE__);
    free_tokens( &set );
  }

  // Testcase six - Empty string + empty sep, no trim
  {
    const char* CASE_STR = "";
    const char* CASE_SEP = "";
    const char* KNOWN_GOOD[] = {"", 0};
    const size_t TCOUNT = sizeof(KNOWN_GOOD)/sizeof(char*) - 1;

    rv = create_tokens( &set, CASE_STR, CASE_SEP, 0 );
    //print_tokens( &set );
    errs += check_token_test_results(rv, &set, KNOWN_GOOD, TCOUNT, __FILE__, __LINE__);
    free_tokens( &set );
  }

  // Testcase seven - multiple seperators
  {
    const char* CASE_STR = "The|rain,in spain falls,mainly|in the,plain.";
    const char* CASE_SEP = " |,";
    const char* KNOWN_GOOD[] = {"The", "rain", "in", "spain", "falls", "mainly", "in", "the", "plain.", 0}; 
    const size_t TCOUNT = sizeof(KNOWN_GOOD)/sizeof(char*) - 1;

    rv = create_tokens( &set, CASE_STR, CASE_SEP, 0 );
    //print_tokens( &set );
    errs += check_token_test_results(rv, &set, KNOWN_GOOD, TCOUNT, __FILE__, __LINE__);
    free_tokens( &set );
  }

  // Testcase eight - untrimmed tokens have the right set of null tokens
  {
    const char* CASE_STR = "  The|rain,in spain falls,mainly||in the,plain.,,";
    const char* CASE_SEP = " |,";
    const char* KNOWN_GOOD[] = {"", "", "The", "rain", "in", "spain", "falls", "mainly", "", "in", "the", "plain.", "", "", 0}; 
    const size_t TCOUNT = sizeof(KNOWN_GOOD)/sizeof(char*) - 1;

    rv = create_tokens( &set, CASE_STR, CASE_SEP, 0 );
    //print_tokens( &set );
    errs += check_token_test_results(rv, &set, KNOWN_GOOD, TCOUNT, __FILE__, __LINE__);
    free_tokens( &set );
  }

  return errs;
}


int main(int argc, char** argv)
{
  int result = 0;
  argc = argc; argv=argv;

  result = test_tokenize();


  printf("\n-----------------------------");
  print_results("TOTAL_TESTSUITE:", result);
  printf("-----------------------------\n");

  return result;
}
