CFLAGS = -Wall `xml2-config --cflags`
LDFLAGS = `xml2-config --libs` -L.
LDLIBS = -lspreadconv -liniparser

SRC = main.c
BIN = cspay
OBJ = $(SRC:.c=.o)

all: $(BIN)

cspay:
	gcc $(OBJ) -o $(BIN)
	
$(BIN): $(OBJ)

.PHONY: clean
clean: 
	-rm -f $(BIN)
	-rm -f $(OBJ)
	-rm -f *~
