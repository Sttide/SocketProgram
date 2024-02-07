#makefile
CC = gcc
CFLAGS = -g -Wall
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGETS = $(patsubst %.c, %, $(SRCS))

#gpt
EXECUTABLES = $(SRCS:.c=)

all: $(EXECUTABLES)


#all: $(SRCS)
#	@echo $(SRCS)
#	@echo $(TARGETS)
#	$(CC)  $(CFLAGS) -o $(TARGETS) $(SRCS)

%: %.c
	$(CC)  $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)