CPPFLAGS = -I../common
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
test: $(BIN)
	gcc test.c -lcspay  $(LDFLAGS) $(CFLAGS) $(LDLIBS) -g -ggdb -o cspay_test
.PHONY: clean
clean: 
	-rm -f $(OBJ)
	-rm -f *~
	-rm -f $(BIN)
