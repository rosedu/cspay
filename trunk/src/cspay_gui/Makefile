#
# CL: Ciudat, chestia de mai jos functioneaza bine
#
CXXFLAGS = -Wall `wx-config --cxxflags`
LDFLAGS = `wx-config --libs`

SRC = cspay_gui.cpp opt_gui.cpp
BIN = cspay_gui
OBJ = $(SRC:.cpp=.o)

all: $(BIN)

$(BIN): $(OBJ)

.PHONY: clean
clean: 
	-rm -f $(BIN)
	-rm -f $(OBJ)
