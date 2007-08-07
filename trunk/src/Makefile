SUBDIRS = iniparser-2.17 libspreadconv libcspay cspay cspay_gui

all:
	for dir in ${SUBDIRS}; do (cd $$dir; make); done
.PHONY: clean
clean:
	for dir in ${SUBDIRS}; do (cd $$dir; make clean); done
