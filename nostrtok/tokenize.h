/**
 * @file   tokenize.h
 * @author Dan Noland <nolandda@nolandda.org>
 * @date   Wed Mar  7 17:44:52 2018
 * 
 * @brief  Header for tokenizer 
 * 
 * 
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

// Use this as the sep value to break on whitespace
extern const char* ASCII_WHITESPACE_CHARS;

// API Functions 
size_t create_tokens( tokset_t* set, const char* str, 
		      const char* sep, uint8_t trim );
size_t create_ws_delimited_tokens( tokset_t* set, const char* str );
const char* get_token( tokset_t* set, size_t idx );
const char* get_next_token( tokset_t* set );

size_t get_num_tokens( const tokset_t* set );
void reset_token_counter( tokset_t* set );

void free_tokens( tokset_t* set );

void print_tokens( const tokset_t* set );

#endif // __TOKENIZE_H__
