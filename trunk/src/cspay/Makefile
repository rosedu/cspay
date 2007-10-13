# CL: nu se poate si mai simplu de atat?
#
CPPFLAGS = -I../common -I../libspreadconv -I../iniparser-2.17/src -I../libcspay
CFLAGS = -Wall `xml2-config --cflags`
LDFLAGS = -L. -L../libcspay `xml2-config --libs` -L../libspreadconv -L../iniparser-2.17
LDLIBS = -lcspay -lspreadconv -liniparser

SRC = main.c
BIN = cspay
OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	gcc $(OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(BIN)
.PHONY: clean
clean: 
	-rm -f $(BIN)
	-rm -f $(OBJ)
	-rm -f *~
