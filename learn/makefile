CC=gcc
CFLAGS=-g -I/usr/local/include
CPPFLAGS=-g -I/usr/local/include
#LDFLAGS=-lgtest -L/usr/local/lib
BINS=gdb-test gtest-foo gl/test_gl

all:$(BINS)

gdb-test:gdb-test.o foo.o

gtest-foo:gtest-foo.o

align-test:align-test.o

gl/test_gl:gl/test_gl.cc
	clang $< -lsdl -framework OpenGL -o $@

gl/polygon:gl/polygon.cc
	clang $< -lsdl -framework OpenGL -framework GLUT -o $@	

gl/shape:gl/shape.cc
	clang $< -lsdl -framework OpenGL -framework GLUT -o $@	

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

