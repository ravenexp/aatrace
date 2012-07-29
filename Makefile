CC = gcc
#CC = clang --analyze
CCLD = $(CC)
CFLAGS = -std=c89 -O3 -g -Wall -Wextra -Wc++-compat

all: testdiff testconv testrender

%.o: %.c *.h Makefile
	$(CC) $(CFLAGS) -c $<

testdiff: testdiff.o pnm.o util.o diff.o
	$(CCLD) -o $@ $^

testconv: testconv.o pnm.o util.o match.o search.o convert.o
	$(CCLD) -o $@ $^

testrender: testrender.o pnm.o util.o render.o
	$(CCLD) -o $@ $^

clean:
	rm -f testdiff testconv testrender *.o
