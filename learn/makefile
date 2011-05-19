CFLAGS=-g -I/usr/local/include
LDFLAGS=-lgtest -L/usr/local/lib
BINS=gdb-test gtest-foo

all:$(BINS)
	
gdb-test:gdb-test.o foo.o

gtest-foo:gtest-foo.o

clean:
	rm -rf $(BINS)
	rm -rf gdb-test.dSYM
	rm *.o
	rm *~
