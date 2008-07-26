CPPFLAGS = -ggdb -I../libspreadconv -I../common
CFLAGS = -Wall `mysql_config --cflags`
LDFLAGS = -L../libspreadconv
LDLIBS = `mysql_config --libs` -lpopt -lspreadconv

SRC = main.c cspay.c load_cfg.c header_footer.c sheet.c
OBJ = $(SRC:.c=.o)
BIN = cspay_bin
all: $(BIN)

$(BIN): $(OBJ)
	gcc $(OBJ) $(LDFLAGS) $(LDLIBS) -o $(BIN)
doc:
	doxygen
.PHONY: clean
clean: 
	-rm -rf $(OBJ)
	-rm -f *~
	-rm -f $(BIN)
	-rm -rf doc
clena: clean
