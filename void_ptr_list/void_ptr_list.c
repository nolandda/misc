/** 
 * @author Dan Noland <nolandda@nolandda.org>
 */

#include <stdlib.h>
#include <assert.h>
#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>
#include "void_ptr_list.h"

// Pointer mask for a futile attempt to keep users of the list from
// messing with list internals when they are handed an iterator.
// TODO : I should make the implemnentation totally hidden behind the API
//const uintptr_t PTR_MASK = (uintptr_t)0x7a7a7a7a7a7a7a7aLL;
const uintptr_t PTR_MASK = (uintptr_t)0x0LL;

static uint64_t local_lcrng_state = 0xdeadbeef;

static void local_lcrng_seed( uint32_t a_seed ) {
  local_lcrng_state = a_seed;
}

static uint32_t local_lcrng_rand( void ) {
  // Linear Congruential Pseudo-Random Number Generator.
  // Coeficents chosen for maximal period per Applied Cryptography
  // by B. Schneier (page 370)
  const uint64_t A = 4096;
  const uint64_t B = 150889;
  const uint64_t MODULUS = 714025;
  uint32_t next_state = (uint32_t)((A*local_lcrng_state + B) % MODULUS);
  local_lcrng_state = next_state;
  return(next_state);
}

vpl_context_t* vpl_context_create( void ) { 
  vpl_context_t* result = calloc( 1, sizeof(vpl_context_t) );
  result->copy_mode = CM_PTRCOPY;
  return(result);
}

vpl_context_t* vpl_context_duplicate( const vpl_context_t* ctx ) {
  vpl_context_t* result = calloc( 1, sizeof(vpl_context_t) );
  result->compare = ctx->compare;
  result->assign = ctx->assign;
  result->duplicate = ctx->duplicate;
  result->deallocate = ctx->deallocate;
  result->tostring = ctx->tostring;
  result->copy_mode = ctx->copy_mode;
  return(result);
}

uint32_t vpl_context_is_valid( const vpl_context_t* ctx ) {
  uint32_t result = 0;
  if( ctx ) {
    if( ctx->compare ) {
      if( ctx->assign ) {
	if( ctx->duplicate ) {
	  if( ctx->deallocate ) {
	    if(( ctx->copy_mode == CM_DEEPCOPY ) 
	       || (ctx->copy_mode == CM_PTRCOPY)) {
	      result = 1;
	    }
	  }
	}
      }
    }
  }
  return result;
}

void vpl_context_free( vpl_context_t* ctx ) {
  // Useless right now, but maybe smarter in the future
  free(ctx);
  return;
}

uint32_t vpl_create( vpl_t** a_list, const vpl_context_t* ctx ) {
  uint32_t result = VPL_OK;
  if( *a_list == NULL ) {
    *a_list = calloc( 1, sizeof(vpl_t));
  }
  (*a_list)->head = NULL;
  (*a_list)->tail = NULL;
  (*a_list)->count = 0;
  (*a_list)->ctx = vpl_context_duplicate(ctx);  
  return result;
}

void vpl_free( vpl_t* a_list, uint32_t full_delete) {
  vpl_node_t* cur = a_list->head;
  vpl_node_t* nxt = NULL;
  while( cur != NULL ) {
    nxt = cur->next;
    /* deallocate the data */
    if( full_delete || (a_list->ctx->copy_mode == CM_DEEPCOPY)) {
      a_list->ctx->deallocate( cur->mystery_data );
    }
    /* Rearrange the deck chairs on the titanic */
    cur->mystery_data = 0;
    cur->prev = 0;
    cur->next = 0;
    /* Now free the node */
    free( cur );
    cur = nxt;
  }
  a_list->head = 0;
  a_list->tail = 0;
  vpl_context_free(a_list->ctx);
  a_list->ctx = 0;
  free(a_list);
  return;
}

uint32_t vpl_push_back( vpl_t* a_list, void* a_data ) {
  uint32_t result = VPL_OK;
  vpl_node_t* new_node = NULL;
  new_node = vpl_helper_node_create( a_list, a_data );
  
  vpl_helper_insert( a_list, a_list->tail, 
		     new_node, NULL); 
  return result;
}

const void* vpl_peek_back( vpl_t* a_list ) {
  void* result = NULL;
  if(a_list->tail) {
    result = a_list->tail->mystery_data;
  }
  return result;
}

void* vpl_pop_back( vpl_t* a_list ) {
  void* result = NULL;
  if(a_list->tail) {
    if(a_list->ctx->copy_mode == CM_DEEPCOPY) {
      result = a_list->ctx->duplicate( a_list->tail->mystery_data );
    }
    else if(a_list->ctx->copy_mode == CM_PTRCOPY) { 
      result = a_list->tail->mystery_data;
    }
    else { 
      // Can't happen
      result = NULL;
    }

    vpl_node_t* victim = a_list->tail;   
    vpl_helper_remove( a_list, 
		       victim->prev, 	
		       victim, 
		       victim->next /*always NULL*/);
    a_list->count -= 1;
  }  
  return result;
}

void vpl_sort( vpl_t* a_list ) {
  int changes = 1;
  vpl_node_t* cur = NULL;
  vpl_node_t* tmp = NULL;
  // TODO: Bubble sort... could be faster as quicksort.
  while( changes ) {
    changes = 0;
    cur = a_list->head;
    while(cur && cur->next) {
      if(a_list->ctx->compare( cur->mystery_data, 
			       cur->next->mystery_data ) > 0) {
	// Do swap
	tmp = cur->next;
	vpl_helper_node_swap( a_list, 
			      cur, tmp );
	// point the iterator back at the new "cur"
	cur = tmp;
	// We changed the list.
	changes = 1;
      }
      cur = cur->next;
    }    
  }
  return;
}

int32_t vpl_compare( const vpl_t* lst_left, const vpl_t* lst_right ) {
  int32_t result = -1;
  vpl_node_t* cur_l = NULL;
  vpl_node_t* cur_r = NULL;
  int32_t flag = 0;
  assert( (int) (lst_left && lst_right));
  if( lst_left->count == lst_right->count ) {
    if( lst_left->ctx->compare == lst_right->ctx->compare ) {
      cur_l = lst_left->head;
      cur_r = lst_right->head;
      while( cur_l && cur_r ) {
	if( lst_left->ctx->compare( cur_l->mystery_data, 
				    cur_r->mystery_data ) ) {
	  flag = 1;
	  break;
	}
	cur_l = cur_l->next;
	cur_r = cur_r->next;
      }
      if( !flag ) {
	result = 0;
      }
    }
  }
  return result;
}

uint32_t vpl_copy( vpl_t* dst_list, vpl_t* src_list ) { 
  vpl_node_t* cur = NULL;
  void* tmp = NULL;
  uint32_t result = 0;
  assert((int) (dst_list && src_list ));
  if( dst_list->ctx->duplicate == src_list->ctx->duplicate ) {
    cur = src_list->head;
    while( cur ) {
      tmp = cur->mystery_data; 
      vpl_push_back(dst_list, tmp);
      result += 1;
      cur = cur->next;
    }
  }
  return result;
}


vpl_iter_t* vpl_find( const vpl_t* a_list, const void* a_data ) { 
  vpl_iter_t* result = NULL;
  vpl_node_t* cur = NULL;
  assert(a_list);
  cur = a_list->head;
  while( cur ) {
    if( a_list->ctx->compare( cur->mystery_data, a_data ) == 0 ) {
      // Found it.
      result = calloc(1, sizeof(vpl_iter_t));
      result->direction = DIR_FORWARD;
      result->opaque_token_alpha = (uintptr_t)(a_list) ^ PTR_MASK;
      result->opaque_token_beta = (uintptr_t)(cur) ^ PTR_MASK;      
      break;
    }
    cur = cur->next;
  }
  return result;
}

void vpl_remove( vpl_t* a_list, vpl_iter_t* a_iter ) {
  vpl_t* lptr = (vpl_t*)( a_iter->opaque_token_alpha ^ PTR_MASK );
  vpl_node_t* nptr = (vpl_node_t*)( a_iter->opaque_token_beta ^ PTR_MASK );
  if(a_list == lptr) {
    if( nptr ) {
      vpl_helper_remove( a_list, nptr->prev, 
			 nptr, nptr->next );      
      a_list->count -= 1;
    }
  }
  else { 
    assert(0);
  }
  return;
}

void vpl_shuffle( vpl_t* a_list, uint32_t a_seed ) {
  vpl_node_t* cur = a_list->head;
  void** tmp_ary = calloc( a_list->count,  sizeof(void*) );
  void* tmp_val = NULL;
  uint32_t i = 0;
  uint32_t rn = 0;
  local_lcrng_seed( a_seed );
  // Fill a fixed size array with random access
  while( cur != NULL ) {
    tmp_ary[i] = cur->mystery_data;    
    i += 1;
    cur = cur->next; 
  }

  //for(i=0; i < a_list->count; i++) {
  //  printf("tmp_ary[%d] = %p\n", i, tmp_ary[i]);
  //}

  // Fisher-Yates Shuffle fixed array
  uint32_t len = (uint32_t)(a_list->count-1);
  for(i=0; i < len; i++) {
    rn = i + ((local_lcrng_rand()) % (len - i + 1));
    tmp_val = tmp_ary[rn];
    tmp_ary[rn] = tmp_ary[i];
    tmp_ary[i] = tmp_val;    
  }
 
  //for(i=0; i < a_list->count; i++) {
  //  printf("tmp_ary[%d] = %p\n", i, tmp_ary[i]);
  //}
 
  // repopulate list with shuffeled data
  cur = a_list->head;
  i = 0;
  while( cur != NULL ) {
    cur->mystery_data = tmp_ary[i];    
    i += 1;
    cur = cur->next;  
  }
  
  // clean up tmp array
  free( tmp_ary ); tmp_ary = 0;
  return;
}

void vpl_print( const vpl_t* a_list ) { // TODO: creates stdio dep. Keep?
  vpl_node_t* cur = a_list->head;
  int i = 0;
  printf(":::::::::::::::::::::::::::::::::\n");
  if( a_list ) {
    printf(":: List at %p contains %"PRIu64" items\n", a_list, a_list->count);
    printf("::    head %p ... %p tail\n", a_list->head, a_list->tail);
    printf(":::::::::::::::::::::::::::::::::\n");

    if( a_list->ctx ) {
      printf("::    Context at %p\n", a_list->ctx);
      printf("::      Func compare %p\n", a_list->ctx->compare);
      printf("::      Func assign %p\n", a_list->ctx->assign);
      printf("::      Func duplicate %p\n", a_list->ctx->duplicate);
      printf("::      Func deallocate %p\n", a_list->ctx->deallocate);
      printf("::      Func tostring %p\n", a_list->ctx->tostring);
      if( a_list->ctx->copy_mode == CM_DEEPCOPY ) {
	printf("::      Copy mode CM_DEEPCOPY\n");
      }
      else if( a_list->ctx->copy_mode == CM_PTRCOPY ) {
	printf("::      Copy mode CM_PTRCOPY\n");
      }
      else { 
	printf("::      INVALID copy mode.\n");
      }
    }
    else { 
      printf("::    Context is NULL\n");
    }
    printf(":::::::::::::::::::::::::::::::::\n");
    while( cur != NULL ) {
      printf("  : Item %d at %p", i, (void*) cur);
      if( a_list->ctx->tostring ) {
	char* tstr = a_list->ctx->tostring(cur->mystery_data);
	printf("%s", tstr);
	free(tstr);
      }
      else {
	printf("  : UNPRINTABLE\n");
      }
      i += 1;
      cur = cur->next; 
    }
  }
  else {
    printf(" NULL List\n");
  }
  printf(":::::::::::::::::::::::::::::::::\n");

  return;
}

////////////////////////////////
// Iterator Functions
////////////////////////////////

vpl_iter_t* vpl_fwd_iter( const vpl_t* lst ) {
  vpl_iter_t* result = NULL;
  if(lst) { 
    result = calloc(1, sizeof(vpl_iter_t));
    result->direction = DIR_FORWARD;
    if(!lst->head) {
      result->past = PAST_EMPTY;
    }
    result->opaque_token_alpha = (uintptr_t)(lst) ^ PTR_MASK;
    result->opaque_token_beta = (uintptr_t)(lst->head) ^ PTR_MASK;
  }
  return result;
}

vpl_iter_t* vpl_bkwd_iter( const vpl_t* lst ) {
  vpl_iter_t* result = NULL;
  if( lst ) {
    result = calloc(1, sizeof(vpl_iter_t));
    result->direction = DIR_BACKWARD;
    if(!lst->tail) {
      result->past = PAST_EMPTY;
    }
    result->opaque_token_alpha = (uintptr_t)(lst) ^ PTR_MASK;
    result->opaque_token_beta = (uintptr_t)(lst->tail) ^ PTR_MASK;  
  }
  return result;
}

uint32_t vpl_is_end( const vpl_iter_t* it ) {
  uint32_t result = 0;
  vpl_node_t* ptr = (vpl_node_t*)( it->opaque_token_beta ^ PTR_MASK );
  if( !ptr ) {
    result = 1;
  }
  return result;
}

vpl_iter_t* vpl_iter_next(vpl_iter_t* it) {
  vpl_iter_t* result = it;
  vpl_t* lptr = (vpl_t*)( it->opaque_token_alpha ^ PTR_MASK );  
  vpl_node_t* nptr = (vpl_node_t*)( it->opaque_token_beta ^ PTR_MASK );  
  if( it->direction == DIR_FORWARD ) {
    if( nptr == NULL ) {
      // Special cases for fwd iterators
      if((it->past == PAST_TAIL) || (it->past == PAST_EMPTY)) {
	result = it;
      }
      else if(it->past == PAST_HEAD) {
	result->opaque_token_beta = (uintptr_t)(lptr->head) ^ PTR_MASK;  	
      }
      else { 
	// Can't happen;
	assert(0);
	result = NULL;
      }
    }
    else { 
      // Typical case for fwd iterators
      result->opaque_token_beta = (uintptr_t)(nptr->next) ^ PTR_MASK;  	      
      if( nptr->next == NULL ) {
	result->past = PAST_TAIL;
      }
    }
  }
  else if( it->direction == DIR_BACKWARD ) {
    if( nptr == NULL ) {
      // Special cases for backward iterators
      if((it->past == PAST_HEAD) || (it->past == PAST_EMPTY)) {
	result = it;
      }
      else if(it->past == PAST_TAIL) {
	result->opaque_token_beta = (uintptr_t)(lptr->tail) ^ PTR_MASK;  	
      }
      else { 
	// Can't happen;
	assert(0);
	result = NULL;
      }

    }
    else { 
      // Typical case for backward iterators
      result->opaque_token_beta = (uintptr_t)(nptr->prev) ^ PTR_MASK;  	      
      if( nptr->prev == NULL ) {
	result->past = PAST_HEAD;
      }
    }
  }
  else { 
    // Can't happen;
    assert(0);
    result = NULL;
  }
  
  return result;
}

vpl_iter_t* vpl_iter_prev(vpl_iter_t* it) {
  // Just temporarially reverse the direction of the iterator and call
  // next will all its special cases already complete. Restore
  // direction afterward
  vpl_iter_t* result = NULL;
  if( it->direction == DIR_BACKWARD ) {
    it->direction = DIR_FORWARD;
    result = vpl_iter_next( it );
    it->direction = DIR_BACKWARD;
  } 
  else if( it->direction == DIR_FORWARD ) {
    it->direction = DIR_BACKWARD;
    result = vpl_iter_next( it );
    it->direction = DIR_FORWARD;    
  }
  else {
    // Can't happen;
    assert(0);
    result = NULL;
  }
  return result;
}

void* vpl_iter_get_data( const vpl_iter_t* it) {
  void* result = NULL;
  vpl_node_t* ptr = (vpl_node_t*)( it->opaque_token_beta ^ PTR_MASK );
  if( ptr ) {
    result = ptr->mystery_data;
  }
  return result;
}

void vpl_iter_free( vpl_iter_t* it ) {
  free(it);
  return;
}

uint32_t vpl_helper_insert( vpl_t* lst,
			    vpl_node_t* before, 
			    vpl_node_t* to_insert, 
			    vpl_node_t* after ) {
  uint32_t result = VPL_OK;
  if( (lst->head == NULL) && (lst->tail == NULL)) {
    // First item
    lst->head = lst->tail = to_insert;
  }
  else if( before == NULL ) {
    // Needs to be head
    to_insert->next = lst->head;
    to_insert->prev = NULL;
    lst->head->prev = to_insert;
    lst->head = to_insert;
  }
  else if( after == NULL ) {
    // Needs to be tail
    to_insert->prev = lst->tail;
    to_insert->next = NULL;
    lst->tail->next = to_insert;
    lst->tail = to_insert;
  }
  else {
    // Somewhere in the middle
    before->next = to_insert;
    after->prev = to_insert;
    to_insert->next = after;
    to_insert->prev = before;
  }
  lst->count += 1;
  return result;
}

uint32_t vpl_helper_remove( vpl_t* lst,
			    vpl_node_t* before, 
			    vpl_node_t* to_remove, 
			    vpl_node_t* after ) {
  uint32_t result = VPL_OK;
  if( to_remove == NULL ) {
    // This is OK I guess. Generally free(null) is gauranteed 
    // to be safe so we emulate that behaivor here.
    return( result );
  }

  if( (lst->head == to_remove) && (lst->tail == to_remove)) {
    // Removing last item.
    lst->head = NULL;
    lst->tail = NULL;
  }
  else if( before == NULL ) {
    // removing the head
    lst->head = lst->head->next;
    lst->head->prev = NULL;
  }
  else if( after == NULL ) {
    // removing the tail
    lst->tail = lst->tail->prev;
    lst->tail->next = NULL;
  }
  else {
    // somewhere in the middle
    before->next = after;
    after->prev = before;
  }
  // Common cleanup code
  if( lst->ctx->copy_mode == CM_DEEPCOPY ) {
    lst->ctx->deallocate( to_remove->mystery_data );
    to_remove->mystery_data = 0;
  }
  free(to_remove);
  return result;
}

 
vpl_node_t* vpl_helper_node_create( vpl_t* lst, void* new_data ) {
  vpl_node_t* result = calloc( 1, sizeof(vpl_node_t) );	
  // pointers already null due to calloc
  if( lst->ctx->copy_mode == CM_PTRCOPY ) {
    result->mystery_data = new_data;
  }
  else if(lst->ctx->copy_mode == CM_DEEPCOPY) { 	   
    result->mystery_data = lst->ctx->duplicate( new_data );
  }
  else {
    // Cant' happen
    free(result);
    result = NULL;
  }
  return(result);
} 

void vpl_helper_node_swap( vpl_t* lst, 
			   vpl_node_t* left, vpl_node_t* right ) {
  vpl_node_t* leftleft = left->prev;
  vpl_node_t* rightright = right->next;
  if( lst && left && right ) {
    left->prev = right;
    left->next = rightright;
    right->prev = leftleft;
    right->next = left;
    if( leftleft ) {
      leftleft->next = right;
    }
    if( rightright ) {
      rightright->prev = left;
    }
    if( lst->head == left ) {
      lst->head = right;
    }
    if( lst->tail == right ) {
      lst->tail = left;
    }
  }
  else {
    assert(0);
  }
  return;
}

