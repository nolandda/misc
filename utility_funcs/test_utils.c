#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>

extern int safe_atoi(int* intval, const char* str);
extern int run_command(char* const * cmd);
extern int run_command_and_pipe_input(char* const * cmd, 
				      const char* inputstr);

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

static uint32_t test_safe_atoi()
{
  uint32_t errs = 0;
  int rv = 0;
  int intret = 0;
  int testval = 0;
  char testvalstr[50] = {0};

  // First negative test cases
  rv = safe_atoi(NULL, NULL);
  if( rv == 0 ) {
    printf("Error: False Positive %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }

  rv = safe_atoi(&intret, NULL);
  if( rv == 0 ) {
    printf("Error: False Positive %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }

  rv = safe_atoi(NULL, "11");
  if( rv == 0 ) {
    printf("Error: False Positive %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }

  rv = safe_atoi(&intret, "cat");
  if( rv == 0 ) {
    printf("Error: False Positive %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }

  rv = safe_atoi(&intret, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
  if( rv == 0 ) {
    printf("Error: False Positive %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }

  //////////////////////////////////////
  // Now positive testcases
  //////////////////////////////////////

  testval = 0; // Zero
  sprintf(testvalstr, "%d", testval);  
  rv = safe_atoi(&intret, testvalstr);
  if( rv != 0 ) {
    printf("Error: False Negative %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }
  else {
    if(intret != testval) {
      printf("Error: Mismatch %s:%d\n", __FILE__, __LINE__);
      errs += 1;
    }
  }

  testval = INT_MIN; // Minimum int val
  sprintf(testvalstr, "%d", testval);  
  rv = safe_atoi(&intret, testvalstr);
  if( rv != 0 ) {
    printf("Error: False Negative %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }
  else {
    if(intret != testval) {
      printf("Error: Mismatch %s:%d\n", __FILE__, __LINE__);
      errs += 1;
    }
  }


  testval = INT_MAX; // Maximum int val
  sprintf(testvalstr, "%d", testval);  
  rv = safe_atoi(&intret, testvalstr);
  if( rv != 0 ) {
    printf("Error: False Negative %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }
  else {
    if(intret != testval) {
      printf("Error: Mismatch %s:%d\n", __FILE__, __LINE__);
      errs += 1;
    }
  }  

  // Check that it is immune to whitespace
  testval = 1234567; // some value
  sprintf(testvalstr, "  \t %d \t\n", testval);  
  rv = safe_atoi(&intret, testvalstr);
  if( rv != 0 ) {
    printf("Error: False Negative %s:%d\n", __FILE__, __LINE__);
    errs += 1;
  }
  else {
    if(intret != testval) {
      printf("Error: Mismatch %s:%d\n", __FILE__, __LINE__);
      errs += 1;
    }
  }  

  return errs;
}


static uint32_t test_run_command()
{
  uint32_t errs = 0;
  int rv = 0;
  int intret = 0;
  int testval = 0;

  // Negative test cases
  char* const * cmd0 = NULL; 
  rv = run_command(cmd0);

  char* const cmd1[] = {0};
  rv = run_command(cmd1);

  char* const cmd2[] = {"/bin/this_is_not_a_valid_executable", 0};
  rv = run_command(cmd2);

  // Positive test cases
  char* const cmd3[] = {"/bin/ls", "-al", 0};
  rv = run_command(cmd3);

  return errs;
}

static uint32_t test_run_command_and_pipe_input()
{
  uint32_t errs = 0;
  int rv = 0;
  //int intret = 0;
  //int testval = 0;
  
  char* const cmd3[] = {"/usr/bin/sort", "--reverse", 0};
  const char* indata = "aaa\nbbbb\nccccc\ndddddd\n";
  rv = run_command_and_pipe_input(cmd3, indata);

  return errs;
}





int main(int argc, char** argv)
{
  int result = 0;
  argc = argc; argv=argv;

  result = test_safe_atoi();


  printf("\n-----------------------------");
  print_results("TOTAL_TESTSUITE:", result);
  printf("-----------------------------\n");

  return result;
}
