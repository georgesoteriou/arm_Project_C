CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic 

all:
	$(CC) $(CFLAGS) extension.c -lpng -o extension

clean:
	rm -rf extension
