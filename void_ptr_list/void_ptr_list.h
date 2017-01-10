/** 
 * @author Dan Noland <nolandda@nolandda.org>
 */

#include <stdint.h>
#include <stdio.h>

#ifndef __VOID_PTR_LIST_H__
#define __VOID_PTR_LIST_H__ 1

/* Error defines */
#define VPL_OK 0

/* Copy Modes */
#define CM_DEEPCOPY 0x12345678
#define CM_PTRCOPY 0x87654321

typedef struct vpl_context_type {
  int (*compare)(const void* left, const void* right );
  void (*assign)(void* dst, const void* src );
  void* (*duplicate)( const void* data_to_dup );
  void (*deallocate)( void* ptr);  
  char* (*tostring)( void* ptr);  
  uint32_t copy_mode;
} vpl_context_t;

typedef struct void_ptr_list_node_type {
  void* mystery_data;
  struct void_ptr_list_node_type* next;
  struct void_ptr_list_node_type* prev;
} vpl_node_t;

typedef struct void_ptr_list_type {
  vpl_node_t* head;
  vpl_node_t* tail;
  uint64_t count;
  vpl_context_t* ctx;
} vpl_t;

#define DIR_FORWARD 0xABCD
#define DIR_BACKWARD 0xDCBA
#define PAST_HEAD 0x11
#define PAST_TAIL 0x22
#define PAST_EMPTY 0x33
#define NOT_PAST 0x0

typedef struct void_pointer_list_iterator_type {
  uint32_t direction;
  uint32_t past;
  uintptr_t opaque_token_alpha;
  uintptr_t opaque_token_beta;  
} vpl_iter_t;

// API Functions 
uint32_t vpl_create( vpl_t** a_list, const vpl_context_t* ctx );
void vpl_free( vpl_t* a_list, uint32_t full_delete);
uint32_t vpl_push_back( vpl_t* a_list, void* a_data );
const void* vpl_peek_back( vpl_t* a_list );
void* vpl_pop_back( vpl_t* a_list );
void vpl_sort( vpl_t* a_list );
uint32_t vpl_copy( vpl_t* dst_list, vpl_t* src_list );
int32_t vpl_compare( const vpl_t* dst_list, const vpl_t* src_list );
vpl_iter_t* vpl_find( const vpl_t* a_list, const void* a_data );
void vpl_remove( vpl_t* a_list, vpl_iter_t* a_iter );
void vpl_shuffle( vpl_t* a_list, uint32_t a_seed );
void vpl_print( const vpl_t* a_list );

vpl_context_t* vpl_context_create( void );
vpl_context_t* vpl_context_duplicate( const vpl_context_t* ctx );
uint32_t vpl_context_is_valid( const vpl_context_t* ctx );
void vpl_context_free( vpl_context_t* ctx );

vpl_iter_t* vpl_fwd_iter( const vpl_t* lst ); 
vpl_iter_t* vpl_bkwd_iter( const vpl_t* lst ); 
uint32_t vpl_is_end( const vpl_iter_t* it ); 
vpl_iter_t* vpl_iter_next(vpl_iter_t* it); 
vpl_iter_t* vpl_iter_prev(vpl_iter_t* it); 
void* vpl_iter_get_data( const vpl_iter_t* it); 
void vpl_iter_free( vpl_iter_t* it ); 

// NON-API Functions. Don't call these.
uint32_t vpl_helper_insert( vpl_t* a_list, vpl_node_t* before,
			    vpl_node_t* to_insert, vpl_node_t* after );

uint32_t vpl_helper_remove( vpl_t* a_list, vpl_node_t* before, 
			    vpl_node_t* to_remove, vpl_node_t* after );

vpl_node_t* vpl_helper_node_create( vpl_t* lst, void* new_data );

void vpl_helper_node_swap( vpl_t* lst, 
			   vpl_node_t* left, vpl_node_t* right );
#endif
