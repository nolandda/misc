
CC = gcc
LDFLAGS = -g
INCLUDES = 

# -ftrapv to make sure a fault is generated if we use any
# dirty integer tricks relying on overflow.
CFLAGS = -Wall -Wextra -ftrapv -g -c

SRCS_XPE = xplatform_endian.c test_xplatform_endian.c 
OBJS_XPE = $(SRCS_XPE:%.c=%.o)

.PHONY: all

all: runtest_xpe

runtest_xpe: $(OBJS_XPE)
	$(CC) $(LDFLAGS) -o runtest_xpe $(OBJS_XPE)


# Inference Rule
%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@


clean:
	rm -f runtest_xpe runtest_fltser *.o 
