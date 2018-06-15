CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

ASSEMBLE_SOURCES = assemble.c $(wildcard assemble_src/*.c)
ASSEMBLE_OBJECTS = assemble.o $(ASSEMBLE_SOURCES:.c=.o)

EMULATE_SOURCES = emulate.c $(wildcard emulate_src/*.c)
EMULATE_OBJECTS = emulate.o $(EMULATE_SOURCES:.c=.o)

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: $(ASSEMBLE_OBJECTS)

emulate: $(EMULATE_OBJECTS)

clean:
	rm -f $(ASSEMBLE_OBJECTS)
	rm -f $(EMULATE_OBJECTS)
	rm -f assemble
	rm -f emulate
	rm -f out
	rm -f extension

test:
	make
	@echo "Make sure arm11_1718_testsuite is cloned in the same directory as arm11_TEAMNUMBER"
	ruby ../../arm11_1718_testsuite/testserver.rb .

ext:
	$(CC) $(CFLAGS) extension.c -g -lpng -o extension
