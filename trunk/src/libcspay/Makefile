CPPFLAGS = -I../common
CFLAGS = -Wall `xml2-config --cflags`
LDFLAGS = `xml2-config --libs` -L. -L../libspreadconv -L../iniparser-2.17
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
