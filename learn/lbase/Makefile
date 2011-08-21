CC			= gcc
CFLAGS		= -g -I/usr/local/include
CPPFLAGS	= -g -I/usr/local/include
LDFLAGS		= -lgtest -L/usr/local/lib
BINS 		= lbase.lib
SRCS		= *.c
OBJS		= larith.o lstack.o lexcept.o lassert.o latom.o lmem.o lmemchk.o

all:$(BINS)

lbase.lib:$(OBJS) 

clean:
	rm -rf $(BINS)
	rm *.o

