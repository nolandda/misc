/** 
 * @author Dan Noland <nolandda@nolandda.org>
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>
#include "tokenize.h"

void do_token_trim( tokset_t* set );

// API Functions 
size_t create_tokens( tokset_t* ctx, const char* str, 
		      const char* sep, uint8_t trim ) {
  size_t i=0, j=0;
  size_t len = 0;
  size_t tokcount = 0;
  char* tstr = NULL;
  char** tokens = NULL;
  char* prev = NULL;

  if( str && sep ) {
    tokcount = 1;
    tstr = strdup(str);
    // Make a first pass marking seperators with '\0' and counting tokens.
    while(tstr[i]) {
      j = 0;
      while(sep[j]) {
	if(tstr[i] == sep[j]) {
	  // found a seperator
	  tokcount += 1;
	  tstr[i] = '\0';
	}
	j += 1;
      }
      i += 1;
    }
    // we accidentially compute strlen along the way. Minor
    // optimization here to avoid a call. 
    len = i;

    // Allocate the tokens
    tokens = calloc(tokcount, sizeof(char*));

    // Second pass to assign token pointers
    prev = tstr;
    j = 0;
    for(i=0; i<len; i+=1) {
      if( tstr[i] == '\0' ) {
	tokens[j] = prev;
	prev = tstr+i+1;
	j += 1;
      }
    }
    // one more left over after the loop (we never see the final '\0')
    tokens[j] = prev;
  }
  ctx->victimstr = tstr;
  ctx->numtok = tokcount;
  ctx->tokens = tokens;
  ctx->curidx = 0;  
  // Trim out null tokens if the user requested
  if( trim ) {
    do_token_trim( ctx );
  }

  return ctx->numtok;
}

const char* get_token( tokset_t* ctx, size_t idx ) {
  size_t tidx = idx;
  if(idx < ctx->numtok) {
    ctx->curidx = idx;
    return ctx->tokens[tidx];
  }
  else {
    return NULL;
  }
}

const char* get_next_token( tokset_t* ctx ) {
  size_t tidx = ctx->curidx;
  if(ctx->curidx < ctx->numtok) {
    ctx->curidx += 1;
    return ctx->tokens[tidx];
  }
  else {
    return NULL;
  }
}

size_t get_num_tokens( const tokset_t* ctx ) {
  return ctx->numtok;
}

void reset_token_counter( tokset_t* ctx ) {
  ctx->curidx = 0;
  return;
}

void free_tokens( tokset_t* ctx ) {
  if( ctx ) {
    free(ctx->tokens); ctx->tokens = NULL;
    free(ctx->victimstr); ctx->victimstr = NULL;
    ctx->numtok = 0;
    ctx->curidx = 0;
  }
  return;
}


void print_tokens( const tokset_t* ctx ) { // TODO: creates stdio dep. Keep?
  size_t i = 0;
  int tok_ok = 0;
  int vs_ok = 0;
  char* cur = NULL;
  printf(":::::::::::::::::::::::::::::::::\n");
  if( ctx ) {
    printf("::    Context at %p\n", ctx);
    printf("::      Num Tokens %zu\n", ctx->numtok);
    if( ctx->tokens ) {
      tok_ok = 1;
      printf("::      Tokens ptr at %p\n", ctx->tokens);
    }
    else {
      printf("::      Tokens are NULL\n");
    }
    if( ctx->victimstr ) {
      vs_ok = 1;
      printf("::      Victim string at %p\n", ctx->victimstr);
    }
    else {
      printf("::      Victim string is NULL\n");
    }
    if( tok_ok && vs_ok ) {
      for(i=0; i<ctx->numtok; i+=1) {
	cur = ctx->tokens[i];
	printf("::        Token[%zu] = %p = [%s]\n", i, cur, cur);
      }
      cur = ctx->tokens[ctx->curidx];
      printf("::      Current Token is %zu = %p = [%s]\n", 
	     ctx->curidx, cur, cur);

    }
  }
  else { 
    printf("::    Context is NULL\n");
  }
  printf(":::::::::::::::::::::::::::::::::\n");
  return;
}


void do_token_trim( tokset_t* set ) {
  size_t i;
  size_t count = 0;
  char** newtoks = calloc(set->numtok, sizeof(char*));
  for(i=0; i<set->numtok; i+=1) {
    if(set->tokens[i][0] != '\0') {
      // Found an non-empty token
      newtoks[count] = set->tokens[i];
      count+=1;
    }
  }

  free(set->tokens);
  set->tokens = newtoks;
  set->numtok = count;
  return;
}
