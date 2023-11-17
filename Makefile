CC=gcc
CCOPTS= --std=gnu99 -Wall

OBJS=bitmap.o

LIBS=lib.a

HEADERS=bitmap.h

BINS=bitmap_test

all:	$(LIBS) $(BINS)

%.o:	%.c $(HEADERS)
		$(CC) $(CCOPTS) -c -o $@ $<

lib.a:	$(OBJS)
	ar -rcs $@ $^
	$(RM) $(OBJS)

bitmap_test:	bitmap_test.o $(LIBS)
				$(CC) $(CCOPTS) -o $@ $^

clean:
	rm -rf *.o *~ $(BINS) $(LIBS)