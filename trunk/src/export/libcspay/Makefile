CPPFLAGS = -ggdb -I../libspreadconv -I../common  -D__DEBUG__
CFLAGS = -Wall `mysql_config --cflags`
LDFLAGS = -L../libspreadconv
LDLIBS = `mysql_config --libs` -lpopt -lspreadconv

# SRC = header_footer.c cspay.c db_op.c sheet.c load_cfg.c
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
