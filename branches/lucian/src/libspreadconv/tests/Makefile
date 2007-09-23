CPPFLAGS = -I../
CFLAGS = -Wall -O0 -g
LDFLAGS = -L../
LDLIBS = -lspreadconv

align: align.o

trans: trans.o

generic: generic.o

span: span.o

col_width: col_width.o

row_height: row_height.o

.PHONY: clean

clean:
	rm -f *.o *~ trans align generic span col_width row_height
