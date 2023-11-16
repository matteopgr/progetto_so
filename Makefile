CC=gcc
CCOPTS= --std=gnu99 -Wall

OBJS=bitmap.o pool_allocator.o

LIBS=lib.a

HEADERS=bitmap.h pool_allocator.h

BINS=bitmap_test pool_allocator_test

all:	$(LIBS) $(BINS)

%.o:	%.c $(HEADERS)
		$(CC) $(CCOPTS) -c -o $@ $<

lib.a:	$(OBJS)
	ar -rcs $@ $^
	$(RM) $(OBJS)

bitmap_test:	bitmap_test.o $(LIBS)
				$(CC) $(CCOPTS) -o $@ $^

pool_allocator_test:	pool_allocator_test.o $(LIBS)
						$(CC) $(CCOPTS) -o $@ $^

clean:
	rm -rf *.o *~ $(BINS) $(LIBS)