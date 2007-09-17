CPPFLAGS = -I../
CFLAGS = -Wall -O0 -g
LDFLAGS = -L../
LDLIBS = -lspreadconv

align: align.o

trans: trans.o

generic: generic.o

span: span.o

.PHONY: clean

clean:
	rm -f *.o *~ trans align generic span
