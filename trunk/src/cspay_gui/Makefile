CXXFLAGS = -Wall `wx-config --cxxflags`
LDFLAGS = `wx-config --libs`
SRC = cspay_gui.cpp
BIN = cspay_gui
# RD: default rule is usually all
all: cspay_gui

#
# RD: we shoud ususally print compile/linking messages
# if one really wants silent operations, he/she should use make's -s flag
#
$(BIN):
	$(CXX) $(SRC) -o $@ $(LDFLAGS) $(CXXFLAGS)

.PHONY: clean

clean: 
	-rm -f $(BIN)
