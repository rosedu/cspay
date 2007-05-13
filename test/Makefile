default:
	@gcc -Wall -c cspay.c -o cspay.o 
	@ar rcs libcspay.a cspay.o
	@g++ -Wall main_wx.cpp -o main_static -L. -lcspay `wx-config --libs` `wx-config --cxxflags`
	@echo "main_static este gata"
clean: 
	rm -f cspay.o main libcspay.a
	rm -f lib*
	rm -f main_static main_dinamic
