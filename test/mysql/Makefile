CFLAGS = -Wall `mysql_config --cflags` -ggdb
LDFLAGS = `mysql_config --libs`

SRC = main.c
BIN = main
OBJ = main.o

all: $(BIN)

.PHONY: clean

clean:
	rm -v $(BIN)
	rm -v $(OBJ)
