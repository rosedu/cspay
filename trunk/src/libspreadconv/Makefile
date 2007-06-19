# libspreadconv makefile
# If anyone has a better way to do this, shoot me right up ;-)

CFLAGS = -Wall

SRC = spreadconv.c
OBJ = $(SRC:.c=.o)

all: libspreadconv.a

libspreadconv.a: $(OBJ)
	ar rc libspreadconv.a $(OBJ)
	ranlib libspreadconv.a

.PHONY: clean

test: test.o
	$(CC) test.o -L. -lspreadconv

clean:
	rm -rf $(OBJ)
	rm -rf libspreadconv.a
