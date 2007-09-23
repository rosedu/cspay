CPPFLAGS = -I../common -I../libspreadconv -I../iniparser-2.17/src
CFLAGS = -Wall `xml2-config --cflags`
LDFLAGS = `xml2-config --libs` -L. -L../libspreadconv -L../iniparser-2.17
LDLIBS = -lspreadconv -liniparser

SRC = load_cfg.c cspay.c
OBJ = $(SRC:.c=.o)
BIN = libcspay.a
all: $(BIN)

$(BIN): $(OBJ)
	ar rc $(BIN) $(OBJ)
	ranlib $(BIN)
.PHONY: clean
clean: 
	-rm -rf $(OBJ)
	-rm -f *~
	-rm -f $(BIN)
