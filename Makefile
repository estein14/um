#
# Makefile for the UM Test lab
#
CC = gcc

IFLAGS  = -I/comp/40/build/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -Wall -Wextra -Werror -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64
LDLIBS  = -lbitpack -l40locality -lcii40 -lcii -lm -lum-dis

EXECS   = um

all: um writetests memory

writetests: umlabwrite.o umlab.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	
memory: memory.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

um: um.o memory.o run.o functions.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	
test: test.o memory.o run.o functions.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS)  *.o
