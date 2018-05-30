CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: global.o shift.o Branch.o Data_Processing.o Multiply.o Single_Data_Transfer.o decode.o execute.o emulate.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
