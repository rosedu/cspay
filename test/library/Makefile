CFLAGS = -Wall

SRC = main.c util.c
BIN = libtest.a
OBJ = main.o

all: $(BIN)

main.o:
	gcc -c  main.c util.c

$(BIN): $(OBJ)
	ar rc $(BIN) $(OBJ)

.PHONY: clean

clean:
	rm -v $(BIN)
	rm -v $(OBJ)
