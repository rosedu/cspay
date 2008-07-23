CPPFLAGS = -ggdb -I../libspreadconv
CFLAGS = -Wall `mysql_config --cflags`
LDFLAGS = 
LDLIBS = `mysql_config --libs` -lpopt

# SRC = header_footer.c cspay.c db_op.c sheet.c load_cfg.c
SRC = main.c
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
