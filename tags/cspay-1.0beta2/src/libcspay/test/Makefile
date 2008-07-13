CPPFLAGS = -Wall -I../
LDFLAGS = `xml2-config --libs` -L../. -L../../libspreadconv -L../../iniparser-2.17/
LDLIBS = -lcspay -liniparser -lspreadconv

cspay: cspay.o

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf cspay
	rm -rf *~
