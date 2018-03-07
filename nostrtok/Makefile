
CC = gcc
LDFLAGS = -g
INCLUDES = 

# -ftrapv to make sure a fault is generated if we use any
# dirty integer tricks relying on overflow.
CFLAGS = -Wall -Wextra -ftrapv -g -c

SRCS_TOK = tokenize.c test_tokenize.c 
OBJS_TOK = $(SRCS_TOK:%.c=%.o)

.PHONY: all

all: runtest_tokenize

runtest_tokenize: $(OBJS_TOK)
	$(CC) $(LDFLAGS) -o runtest_tokenize $(OBJS_TOK)


# Inference Rule
%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

.PHONY: tags
tags:
	ctags -e *.c *.h

clean:
	rm -f runtest_tokenize *.o 
