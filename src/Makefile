CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: global.o decode.o execute.o 

global.o: global.h

decode.o: decode.h global.o 

execute.o: execute.h global.o Data_Processing.o Multiply.o Single_Data_Transfer.o Branch.o

Data_Processing.o: Data_Processing.h global.o 

Multiply.o: Multiply.h global.o 

Single_Data_Transfer.o: Single_Data_Transfer.h global.o 

Branch.o: Branch.h global.o 

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
