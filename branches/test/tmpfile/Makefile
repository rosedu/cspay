# Vlad: sper ca am respectat conventiile

CFLAGS=-Wall -g

all:tmptest.o
	$(CC) $^ -o tmptest

*.o:
	$(CC) $(CFLAGS) -c $@

.PHONY: clean
clean:
	rm -f *.o tmptest
