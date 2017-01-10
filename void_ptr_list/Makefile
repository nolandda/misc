
CC = gcc
LDFLAGS = -g
INCLUDES = 

# -ftrapv to make sure a fault is generated if we use any
# dirty integer tricks relying on overflow.
CFLAGS = -Wall -Wextra -ftrapv -g -c

SRCS_VPL = void_ptr_list.c test_void_ptr_list.c 
OBJS_VPL = $(SRCS_VPL:%.c=%.o)

.PHONY: all

all: runtest_vpl

runtest_vpl: $(OBJS_VPL)
	$(CC) $(LDFLAGS) -o runtest_vpl $(OBJS_VPL)


# Inference Rule
%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@


clean:
	rm -f runtest_vpl *.o 
