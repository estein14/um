#
# Makefile
# Written By: Emmett Stein (estein14), Noah Wright (nwrigh05)
# With command "make", produces binary executable um
#
CC = gcc

IFLAGS  = -I/comp/40/build/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -Wall -Wextra -Werror -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64
LDLIBS  = -lbitpack -l40locality -lcii40 -lcii -lm -lum-dis

EXECS   = um

um: um.o memory.o run.o functions.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	
writetests: umlabwrite.o umlab.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	
# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS)  *.o
