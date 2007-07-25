CFLAGS = -Wall `xml2-config --cflags`
LDFLAGS = `xml2-config --libs` -L.
LDLIBS = -lspreadconv -liniparser

SRC = getopt.c cspay.c
BIN = cspay
OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)

.PHONY: clean
clean: 
	-rm -f $(BIN)
	-rm -f $(OBJ)
	-rm -f *~
