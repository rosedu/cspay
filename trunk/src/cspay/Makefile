CFLAGS = -Wall 
SRC = main.c
BIN = cspay
OBJ = $(SRC:.c=.o)

all: $(BIN)

cspay:
	gcc $(OBJ) -o $(BIN)
	
$(BIN): $(OBJ)

.PHONY: clean
clean: 
	-rm -f $(BIN)
	-rm -f $(OBJ)
	-rm -f *~
