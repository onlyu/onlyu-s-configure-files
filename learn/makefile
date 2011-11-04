CC=gcc
CFLAGS=-g -I/usr/local/include
CPPFLAGS=-g -I/usr/local/include
#LDFLAGS=-lgtest -L/usr/local/lib
BINS=gdb-test gtest-foo

all:$(BINS)

gdb-test:gdb-test.o foo.o

gtest-foo:gtest-foo.o

align-test:align-test.o

clean:
	rm -rf $(BINS)
	rm -rf gdb-test.dSYM
	rm *.o
	rm *~


# for flymake
CHK_SOURCES	= lassert.c
.PHONY: check-syntax 
check-syntax: 
	$(CC) -Wall -fsyntax-only $(CHK_SOURCES)

