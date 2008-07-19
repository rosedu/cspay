#
# publish pages
#

.PHONY: all publish clean

all: publish

publish: clean
	svn export file:///home/projects/cspay/svn/branches/edi/cspay \
		/home/projects/cspay/public_html/iw2/cspay

clean:
	-rm -fr /home/projects/cspay/public_html/iw2/cspay/
