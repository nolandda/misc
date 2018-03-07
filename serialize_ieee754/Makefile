
CC = gcc
LDFLAGS = -g
INCLUDES = 

# -ftrapv to make sure a fault is generated if we use any
# dirty integer tricks relying on overflow.
CFLAGS = -Wall -Wextra -ftrapv -g -c

SRCS_FLTSER = ../xplatform_endian/xplatform_endian.c serialize_float.c test_float_serialize.c 
OBJS_FLTSER = $(SRCS_FLTSER:%.c=%.o)

.PHONY: all

all: runtest_fltser

runtest_fltser: $(OBJS_FLTSER)
	$(CC) $(LDFLAGS) -o runtest_fltser $(OBJS_FLTSER) -lm


# Inference Rule
%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

.PHONY: tags
tags:
	ctags -e *.c *.h

clean:
	rm -f runtest_fltser *.o 
