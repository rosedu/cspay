# RD: try to use as many of GNU make's built-in rules as possible
CXXFLAGS = -Wall -g `wx-config --cxxflags`
LDFLAGS = -L. -lcspay `wx-config --libs`

# RD: default rule is usually all
all: main_static

#
# RD: we shoud ususally print compile/linking messages
# if one really wants silent operations, he/she should use make's -s flag
#
main_static: main_wx.o libcspay.a
	$(CXX) $< -o $@ $(LDFLAGS) 
	@echo "main_static este gata"

libcspay.a: cspay.o
	$(AR) rcs $@ $^

.PHONY: clean

clean: 
	-rm -f *.o *~ main libcspay.a
	-rm -f lib*
	-rm -f main_static main_dinamic
