/** 
 * @author Dan Noland <nolandda@nolandda.org>
 */

#include <stdint.h>

#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__ 1

// TODO hide the internals of the context type
typedef struct token_set_type {
  char** tokens;
  size_t numtok;
  char* victimstr;
  size_t curidx;
} tokset_t;


// API Functions 
size_t create_tokens( tokset_t* ctx, const char* str, 
		      const char* sep, uint8_t trim );
const char* get_token( tokset_t* ctx, size_t idx );
const char* get_next_token( tokset_t* ctx );

size_t get_num_tokens( const tokset_t* ctx );
void reset_token_counter( tokset_t* ctx );

void free_tokens( tokset_t* ctx );


// NON-API Functions. Don't call these.
void print_tokens( const tokset_t* ctx );

#endif // __TOKENIZE_H__
