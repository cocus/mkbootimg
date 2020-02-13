CC ?= gcc

RM ?= rm

CFLAGS = -g
LFLAGS =

INCLUDES = -I.

SRCS = mkbootimg.c rsa.c  sha.c

MAIN = mkbootimg


OBJS = $(SRCS:.c=.o)

.PHONY: clean

all: $(MAIN)
	@echo done!

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o $(MAIN)
