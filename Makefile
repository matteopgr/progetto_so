CC=gcc
CCOPTS= --std=gnu99 -Wall

OBJS=bitmap.o buddy_allocator.o allocator.o linked_list.o pool_allocator.o

LIBS=lib.a

HEADERS=bitmap.h buddy_allocator.h allocator.h linked_list.h pool_allocator.h

BINS=bitmap_test buddy_allocator_test allocator_test pool_allocator_test

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

buddy_allocator_test:	buddy_allocator_test.o $(LIBS) 
						$(CC) $(CCOPTS) -o $@ $^ -lm

allocator_test:	allocator_test.o $(LIBS) 
				$(CC) $(CCOPTS) -o $@ $^ -lm

clean:
	rm -rf *.o *~ $(BINS) $(LIBS)