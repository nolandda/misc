/** 
 * @author Dan Noland <nolandda@nolandda.org>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "void_ptr_list.h"

int str_compare(const void* left, const void* right );
void str_assign(void* dst, const void* src );
void* str_duplicate( const void* data_to_dup );
void str_deallocate( void* ptr);

char* random_string(uint32_t min, uint32_t max);
void test_setup_helper( vpl_t** lpp, vpl_context_t** cpp );

uint32_t runcase__sort_empty_list(const char* TNAME);
uint32_t runcase__sort_one_item(const char* TNAME);
uint32_t runcase__sort_two_items(const char* TNAME);
uint32_t runcase__copy_and_compare(const char* TNAME);
uint32_t runcase__iter_fuzz(const char* TNAME);
uint32_t runcase__push_pop_fuzz(const char* TNAME);

int str_compare(const void* left, const void* right ) {
  int result = strcmp((const char*)left, (const char*)right);
  return(result);
}

void str_assign(void* dst, const void* src ) {
  strcpy((char*)dst, (const char*)src);
  return;
}

void* str_duplicate( const void* data_to_dup ) {
  void* result = strdup( (const char*)data_to_dup );
  return result;
}

void str_deallocate( void* ptr) {
  free(ptr);
  return; 
}  

char* random_string(uint32_t min, uint32_t max) {
  const char* printable = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";//"!@#$%^&*()_+-[]{}:,.<>?";
  size_t plen = strlen(printable);
  uint32_t range = max - min;
  uint32_t i = 0;
  uint32_t res_len = min + ((uint32_t)rand()) % range;
  char* res = calloc( res_len+1, sizeof(char) );  
  for(i=0; i<res_len; i++) {
    res[i] = printable[ ((uint32_t)rand())%plen ];
  }
  return(res);
}

void test_setup_helper( vpl_t** lpp, vpl_context_t** cpp ) {
  vpl_context_t* ctx = NULL; 
  *cpp = vpl_context_create( );
  ctx = *cpp;
  ctx->compare = str_compare;
  ctx->assign = str_assign;
  ctx->duplicate = str_duplicate;
  ctx->deallocate = str_deallocate;
  vpl_create( lpp, ctx );
  return;
}


uint32_t runcase__sort_empty_list(const char* TNAME) {
  ////////////////////////
  ///// Common setup /////
  ////////////////////////
  vpl_context_t* ctx = NULL;
  vpl_t* lst = NULL;
  vpl_iter_t* it = NULL;
  //char* str = NULL;
  char* temp_str = NULL;
  int i = 0;
  uint32_t result = 1;
  test_setup_helper( &lst, &ctx );
  printf("----- Begin Case %s -----\n", TNAME);  fflush(NULL);

  ////////////////////
  ///// The case /////
  ////////////////////  
  vpl_sort( lst );
  for( it = vpl_fwd_iter(lst); !vpl_is_end(it); it = vpl_iter_next(it) ) {
    temp_str = (char*)vpl_iter_get_data( it );
    printf("%s[%d] = %s\n", TNAME, i, temp_str);
    i += 1;
  }  

  ///////////////////////////
  ///// Common teardown /////
  ///////////////////////////
  vpl_iter_free( it ); it = NULL;
  vpl_free( lst, 1); lst = 0;
  vpl_context_free(ctx); ctx = 0;
  printf("----- End Case %s -----\n", TNAME);  fflush(NULL);
  return result;
}

uint32_t runcase__sort_one_item(const char* TNAME) {
  ////////////////////////
  ///// Common setup /////
  ////////////////////////
  vpl_context_t* ctx = NULL;
  vpl_t* lst = NULL;
  vpl_iter_t* it = NULL;
  char* str = NULL;
  char* temp_str = NULL;
  int i = 0;
  uint32_t result = 1;
  test_setup_helper( &lst, &ctx );
  printf("----- Begin Case %s -----\n", TNAME);  fflush(NULL);

  ////////////////////
  ///// The case /////
  ////////////////////  
  str = random_string(10, 50);
  vpl_push_back( lst, str );  
  vpl_sort( lst );
  for( it = vpl_fwd_iter(lst); !vpl_is_end(it); it = vpl_iter_next(it) ) {
    temp_str = (char*)vpl_iter_get_data( it );
    printf("%s[%d] = %s\n", TNAME, i, temp_str);
    i += 1;
  }  

  if( strcmp( temp_str, str ) != 0 ) {
    printf("Error %s != %s\n", temp_str, str );
    result = 0;
  }

  ///////////////////////////
  ///// Common teardown /////
  ///////////////////////////
  vpl_iter_free( it ); it = 0;
  vpl_free( lst, 1); lst = 0;
  vpl_context_free(ctx); ctx = 0;
  printf("----- End Case %s -----\n", TNAME);  fflush(NULL);
  return result;
}


uint32_t runcase__sort_two_items(const char* TNAME) {
  ////////////////////////
  ///// Common setup /////
  ////////////////////////
  vpl_context_t* ctx = NULL;
  vpl_t* lst = NULL;
  vpl_iter_t* it = NULL;
  char* str = NULL;
  char* temp_str = NULL;
  int i = 0;
  uint32_t result = 1;
  test_setup_helper( &lst, &ctx );
  printf("----- Begin Case %s -----\n", TNAME);  fflush(NULL);

  ////////////////////
  ///// The case /////
  ////////////////////  
  const char* str_ary[] = {"aaaaaaaaaaaaaaaaa", "zzzzzzzzzzzzzzzzzzzz"};
  str = strdup(str_ary[1]);
  vpl_push_back( lst, str );  
  str = strdup(str_ary[0]);
  vpl_push_back( lst, str );  


  vpl_sort( lst );
  for( it = vpl_fwd_iter(lst); !vpl_is_end(it); it = vpl_iter_next(it) ) {
    temp_str = (char*)vpl_iter_get_data( it );
    printf("%s[%d] = %s\n", TNAME, i, temp_str);
    if( strcmp( temp_str, str_ary[i] ) != 0 ) {
      printf("Error %s != %s\n", temp_str, str_ary[i] );
      result = 0;
      break;
    }
    i += 1;
  }  

  ///////////////////////////
  ///// Common teardown /////
  ///////////////////////////
  vpl_iter_free( it ); it = 0;
  vpl_free( lst, 1); lst = 0;
  vpl_context_free(ctx); ctx = 0;
  printf("----- End Case %s -----\n", TNAME);  fflush(NULL);
  return result;
}


uint32_t runcase__copy_and_compare(const char* TNAME) {
  ////////////////////////
  ///// Common setup /////
  ////////////////////////
  vpl_context_t* ctx_o = NULL;
  vpl_context_t* ctx = NULL;
  vpl_context_t* ctx_d = NULL;
  vpl_t* lst_orig = NULL;
  vpl_t* lst = NULL;
  vpl_t* lst_dup = NULL;
  char* str = NULL;
  //char* temp_str = NULL;
  int i = 0;
  uint32_t result = 1;
  test_setup_helper( &lst_orig, &ctx_o );
  lst_orig->ctx->copy_mode = CM_DEEPCOPY;
  test_setup_helper( &lst, &ctx );
  test_setup_helper( &lst_dup, &ctx_d );
  lst_dup->ctx->copy_mode = CM_DEEPCOPY;
  printf("----- Begin Case %s -----\n", TNAME);  fflush(NULL);

  ////////////////////
  ///// The case /////
  ////////////////////  
  int count = 10 + rand()%100; 
  for(i=0; i<count; i++) {
    str = random_string(10, 50);
    //temp_str = strdup( str );
    vpl_push_back( lst_orig, str );
    vpl_push_back( lst, str );    
  }

  if( vpl_compare( lst, lst_orig ) != 0 ) {
    printf("Orignial compare mismatch\n");
    result = 0;
    return result;
  }

  vpl_sort( lst );
  vpl_copy( lst_dup, lst );
  vpl_sort( lst_orig );

  if( vpl_compare( lst_dup, lst_orig ) != 0 ) {
    printf("Final compare mismatch\n");
    result = 0;
    return result;
  }

  ///////////////////////////
  ///// Common teardown /////
  ///////////////////////////
  vpl_free( lst_orig, 1); lst_orig = 0;
  vpl_free( lst, 1); lst = 0;
  vpl_free( lst_dup, 1); lst_dup = 0;
  vpl_context_free(ctx_o); ctx_o = 0;
  vpl_context_free(ctx); ctx = 0;
  vpl_context_free(ctx_d); ctx_d = 0;
  printf("----- End Case %s -----\n", TNAME);  fflush(NULL);
  return result;
}



uint32_t runcase__push_pop_fuzz(const char* TNAME) {
  ////////////////////////
  ///// Common setup /////
  ////////////////////////
  vpl_context_t* ctx = NULL;
  vpl_t* lst = NULL;
  vpl_iter_t* it = NULL;
  char* str = NULL;
  char* temp_str = NULL;
  int i = 0;
  uint32_t result = 1;
  test_setup_helper( &lst, &ctx );
  printf("----- Begin Case %s -----\n", TNAME);  fflush(NULL);

  ////////////////////
  ///// The case /////
  ////////////////////  
  char* str_ary[100] = {0};
  char* recovered_ary[100] = {0};
  for(i=0; i<100; i++) {
    str_ary[i] = random_string(10, 50);
  }
  int in_count = 0;
  int out_count = 0;
  while(in_count < 100 ) {
    if(rand()%2 == 0) {
      // Tails remove an item
      str = (char*)vpl_pop_back( lst );
      if( str ) {
	recovered_ary[ out_count ] = str;
	out_count += 1;
      }
    }
    else {
      // Heads add an item
      vpl_push_back( lst, str_ary[in_count] );
      in_count += 1;
    }    
  }

  // pop any stragglers
  while( (str = (char*)vpl_pop_back( lst )) ) {
    recovered_ary[ out_count ] = str;
    out_count += 1;
  }

  if(in_count != out_count) {
    printf("Error: length mismatch\n");
    result = 0;
    return result;
  }

  // sort the references & the recovered
  int changes = 1;
  while(changes) {
    changes = 0;
    for(i=0; i<100-1; i+=1) {
      if( strcmp(str_ary[i], str_ary[i+1]) > 0 ) {
	temp_str = str_ary[i];
	str_ary[i] = str_ary[i+1];
	str_ary[i+1] = temp_str;
	changes = 1;
      } 

      if( strcmp(recovered_ary[i], recovered_ary[i+1]) > 0 ) {
	temp_str = recovered_ary[i];
	recovered_ary[i] = recovered_ary[i+1];
	recovered_ary[i+1] = temp_str;
	changes = 1;
      } 
    }
  }

  
  // final comparison
  for(i=0; i<100; i+=1) {
    if( strcmp( recovered_ary[i], str_ary[i] ) != 0 ) {
      printf("Error %s != %s\n", recovered_ary[i], str_ary[i] );
      result = 0;
      break;
    }
  }  

  // cleanup test data
  for(i=0; i<100; i+=1) {
    free(str_ary[i]);
    str_ary[i] = 0;
    recovered_ary[i] = 0;
  }

  ///////////////////////////
  ///// Common teardown /////
  ///////////////////////////
  vpl_iter_free( it ); it = 0;
  vpl_free( lst, 1); lst = 0;
  vpl_context_free(ctx); ctx = 0;
  printf("----- End Case %s -----\n", TNAME);  fflush(NULL);
  return result;
}



uint32_t runcase__iter_fuzz(const char* TNAME) {
  ////////////////////////
  ///// Common setup /////
  ////////////////////////
  vpl_context_t* ctx = NULL;
  vpl_t* lst = NULL;
  vpl_iter_t* it = NULL;
  char* str = NULL;
  char* temp_str = NULL;
  int i = 0;
  uint32_t result = 1;
  test_setup_helper( &lst, &ctx );
  printf("----- Begin Case %s -----\n", TNAME);  fflush(NULL);

  ////////////////////
  ///// The case /////
  ////////////////////  
  char* str_ary[100] = {0};
  char* cur_list[100] = {0};
  char* recovered_ary[100] = {0};
  for(i=0; i<100; i++) {
    str_ary[i] = random_string(10, 50);
  }
  int in_count = 0;
  int cur_count = 0;
  int out_count = 0;
  int idx = 0;
  while(in_count < 100 ) {
    if(rand()%2 == 0) {
      // Tails remove an item
      if( cur_count > 0 ) {
	idx = rand()%cur_count;
	str = cur_list[ idx ];
	it = vpl_find( lst, str );
	if( it == NULL ) { 
	  result = 0;
	  printf("Error: Failed to find\n");
	  break;
	}
	recovered_ary[ out_count ] = (char*)vpl_iter_get_data( it );
	out_count += 1;
	if(cur_count > 1) {
	  cur_list[idx] = cur_list[cur_count-1];
	}
	cur_count -= 1;
	vpl_remove( lst, it );
	vpl_iter_free( it ); it = 0;
      }
    }
    else {
      // Heads add an item
      vpl_push_back( lst, str_ary[in_count] );
      cur_list[cur_count] = str_ary[in_count];
      cur_count += 1;
      in_count += 1;
    }    
  }
  
  it = NULL;

  // pop any stragglers
  while( cur_count ) {
    idx = rand()%cur_count;
    str = cur_list[ idx ];
    it = vpl_find( lst, str );
    if( it == NULL ) { 
      result = 0;
      printf("Error: Failed to find\n");
      break;
    }
    recovered_ary[ out_count ] = vpl_iter_get_data( it );
    out_count += 1;
    if(cur_count > 1) {
      cur_list[idx] = cur_list[cur_count-1];
    }
    cur_count -= 1;
    vpl_remove( lst, it );
    vpl_iter_free( it ); it = NULL;
  }

  if(in_count != out_count) {
    printf("Error: length mismatch in=[%d] out=[%d]\n", in_count, out_count);
    result = 0;
    return result;
  }

  // sort the references & the recovered
  int changes = 1;
  while(changes) {
    changes = 0;
    for(i=0; i<100-1; i+=1) {
      if( strcmp(str_ary[i], str_ary[i+1]) > 0 ) {
	temp_str = str_ary[i];
	str_ary[i] = str_ary[i+1];
	str_ary[i+1] = temp_str;
	changes = 1;
      } 

      if( strcmp(recovered_ary[i], recovered_ary[i+1]) > 0 ) {
	temp_str = recovered_ary[i];
	recovered_ary[i] = recovered_ary[i+1];
	recovered_ary[i+1] = temp_str;
	changes = 1;
      } 
    }
  }
  
  // final comparison
  for(i=0; i<100; i+=1) {
    if( strcmp( recovered_ary[i], str_ary[i] ) != 0 ) {
      printf("Error %s != %s\n", recovered_ary[i], str_ary[i] );
      result = 0;
      break;
    }
  }  

  // cleanup test data
  for(i=0; i<100; i+=1) {
    free(str_ary[i]);
    str_ary[i] = 0;
    recovered_ary[i] = 0;
  }

  ///////////////////////////
  ///// Common teardown /////
  ///////////////////////////
  vpl_iter_free( it ); it = 0;
  vpl_free( lst, 1); lst = 0;
  vpl_context_free(ctx); ctx = 0;
  printf("----- End Case %s -----\n", TNAME);  fflush(NULL);
  return result;
}





int main(int argc, char *argv[])
{
  int i = 0;
  vpl_context_t* sctx = NULL;
  vpl_t* sl = NULL;
  argc = argc; argv = argv; /* shut up warnings */
  printf("--------- BEGIN LIST_TEST ----------\n");  fflush(NULL);
  sctx = vpl_context_create( );
  sctx->compare = str_compare;
  sctx->assign = str_assign;
  sctx->duplicate = str_duplicate;
  sctx->deallocate = str_deallocate;
  vpl_create( &sl, sctx );
  char* str = NULL;
  for(i=0; i<50; i++) {
    str = random_string(10, 50);
    printf("str[%d] = %s\n", i, str);
    vpl_push_back( sl, str );
    //free(str);
  }

  vpl_iter_t* it;
  char* temp_str = NULL;
  i = 0;
  for( it = vpl_fwd_iter(sl); !vpl_is_end( it ); it = vpl_iter_next(it) ) {
    temp_str = (char*)vpl_iter_get_data( it );
    printf("LIST[%d] = %s\n", i, temp_str);
    i += 1;
  }
  vpl_iter_free( it ); it = 0;
  i = 0;
  vpl_sort( sl );
  for( it = vpl_fwd_iter(sl); !vpl_is_end( it ); it = vpl_iter_next(it) ) {
    temp_str = (char*)vpl_iter_get_data( it );
    printf("SORT_LIST[%d] = %s\n", i, temp_str);
    i += 1;
  }
  vpl_iter_free( it ); it = 0;
  vpl_free( sl, 1); sl = 0;

  uint32_t rr=1; 
  rr = runcase__push_pop_fuzz("push_pop_fuzz") && rr;  
  rr = runcase__iter_fuzz("iter_fuzz") && rr;  
  rr = runcase__copy_and_compare("copy_and_compare") && rr;

  printf("--- Edge Cases ---\n");  
  // Case - sort one item
  rr = runcase__sort_empty_list("sort_empty_list") && rr; 
  rr = runcase__sort_one_item("sort_one_item") && rr; 
  rr = runcase__sort_two_items("sort_two_items") && rr;

  vpl_context_free(sctx); sctx = 0;
  printf("--------- END LIST_TEST ----------\n");  
  if( rr ) {
    printf("ALL TESTS PASS\n");
  }
  else { 
    printf("*** FAILURE ***\n");
  }
  //for(i=0; i<45; i++) {
  //  str = random_string(8, 50);
  //  printf("%d:%s\n", i, str);
  //}

  return 0;
}
