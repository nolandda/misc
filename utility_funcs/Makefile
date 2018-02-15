
CC = gcc
LDFLAGS = -g
INCLUDES = 

# -ftrapv to make sure a fault is generated if we use any
# dirty integer tricks relying on overflow.
CFLAGS = -Wall -Wextra -ftrapv -g -c

SRCS_UTILS = safe_atoi.c test_utils.c
OBJS_UTILS = $(SRCS_UTILS:%.c=%.o)

.PHONY: all

all: runtest_utils

runtest_utils: $(OBJS_UTILS)
	$(CC) $(LDFLAGS) -o runtest_utils $(OBJS_UTILS)


# Inference Rule
%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@


clean:
	rm -f runtest_utils *.o 
