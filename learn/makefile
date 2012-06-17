CC=clang
LD=clang
CFLAGS=-g -I/usr/local/include -I/opt/local/include
CPPFLAGS=-g -I/usr/local/include  -I/opt/local/include
LDFLAGS=-lgtest -L/usr/local/lib -L/opt/local/lib 
TEST_FLAG = -L/opt/local/lib -lgtest -stdlib=libstdc++ -lstdc++ 
BINS=gdb-test gtest-foo gl/test_gl

all:$(BINS)

gdb-test:gdb-test.o foo.o

gtest-foo:gtest-foo.o

align-test:align-test.o

gl/test_gl:gl/test_gl.cc
	$(CC) $< -lsdl -framework OpenGL -o $@

gl/polygon:gl/polygon.cc
	$(CC) $< -lsdl -framework OpenGL -framework GLUT -o $@	

gl/shape:gl/shape.cc
	clang $< -lsdl -framework OpenGL -framework GLUT -o $@	

gl/texture:gl/texture.c
	clang $< -lsdl -framework OpenGL -framework GLUT -o $@	

gl/light:gl/light.c
	clang $< -lsdl -framework OpenGL -framework GLUT -o $@	

npl/edit_distance:npl/edit_distance.c
	clang -D __MAIN__ $< -o $@

npl/test_edit_distance:npl/test_edit_distance.cc npl/edit_distance.c
	$(CC) $(CFLAGS) $^ $(TEST_FLAG) -o $@ 

clean:
	rm -rf $(BINS)
	rm -rf gdb-test.dSYM
	rm *.o
	rm *~


# for flymake
CHK_SOURCES	= lassert.c
.PHONY: check-syntax 
check-syntax: 
	$(CC) $(CFLAGS) -Wall -fsyntax-only $(CHK_SOURCES)	

