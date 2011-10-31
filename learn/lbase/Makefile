CC		= gcc
CFLAGS		= -g -I/usr/local/include
CPPFLAGS	= -g -I/usr/local/include
LDFLAGS		= -lgtest -L/usr/local/lib
BINS 		= lbase.lib
SRCS		= *.c
OBJS		= larith.o lstack.o lexcept.o lassert.o latom.o lmem.o lmemchk.o larena.o llist.o ltable.o lset.o larray.o lseq.o lring.o

all:$(BINS)

lbase.lib:$(OBJS) 

clean:
	rm -rf $(BINS)
	rm *.o

# for flymake
CHK_SOURCES	= lassert.c
.PHONY: check-syntax 
check-syntax: 
	$(CC) -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES)
