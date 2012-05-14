CC = gcc
#CC = clang --analyze
CCLD = $(CC)
CFLAGS = -std=c89 -O3 -g -Wall -Wextra -Wc++-compat

all: testconv testdiff

%.o: %.c *.h Makefile
	$(CC) $(CFLAGS) -c $<

testdiff: testdiff.o pnm.o util.o diff.o
	$(CCLD) -o $@ $^

testconv: testconv.o pnm.o util.o diff.o match.o render.o
	$(CCLD) -o $@ $^

clean:
	rm -f testdiff testconv *.o
