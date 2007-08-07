#
# CL: Ciudat, chestia de mai jos functioneaza bine
#
CXXFLAGS = -Wall `wx-config --cxxflags` `xml2-config --cflags` -I../libcspay
LDFLAGS = `wx-config --libs` -L../libiniparser-2.17 -L../libspreadconv -L../libcspay `xml2-config --libs`
LDLIBS = -lcspay -lspreadconv -liniparser

SRC = cspay_gui.cpp opt_gui.cpp
BIN = cspay_gui
OBJ = $(SRC:.cpp=.o)

all: $(BIN)

$(BIN): $(OBJ)

.PHONY: clean
clean: 
	-rm -f $(BIN)
	-rm -f $(OBJ)
	-rm -f *~
