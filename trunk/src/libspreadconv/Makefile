# libspreadconv makefile
# If anyone has a better way to do this, shoot me right up ;-)
# RD: 21.06.2007 - added 'libspreadconv.a' to 'test' requirements
# 		 - added implicit rule for test/col_test executable
# 		  (http://www.gnu.org/software/make/manual/make.html
# 		  chapter 10.2 - subsection 'Linking a single object file'
#		  or
#		   $ info make "Catalogue of Rules" (with make-doc package))
#		 - added tests rules for creating test and col_test

CFLAGS = -Wall
LDFLAGS = -L.
LDLIBS = -lspreadconv

SRC = spreadconv.c
OBJ = $(SRC:.c=.o)

all: libspreadconv.a

libspreadconv.a: $(OBJ)
	ar rc libspreadconv.a $(OBJ)
	ranlib libspreadconv.a

tests: test col_test

test: test.o libspreadconv.a

col_test: col_test.o libspreadconv.a

.PHONY: clean clean_test

clean_test:
	-rm -rf test.o col_test.o test col_test

clean:
	-rm -rf $(OBJ) *~
	-rm -rf libspreadconv.a
