# libspreadconv makefile
# If anyone has a better way to do this, shoot me right up ;-)

CFLAGS = -Wall

SRC = spreadconv.c
OBJ = $(SRC:.c=.o)

all: libspreadconv.a

libspreadconv.a: $(OBJ)
	ar rc libspreadconv.a $(OBJ)
	ranlib libspreadconv.a

test: test.o col_test.o
	$(CC) test.o -L. -lspreadconv -o test
	$(CC) col_test.o -L. -lspreadconv -o col_test

.PHONY: clean clean_test

clean_test:
	rm -rf test.o col_test.o test col_test

clean:
	rm -rf $(OBJ)
	rm -rf libspreadconv.a
