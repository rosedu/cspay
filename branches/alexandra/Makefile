#
# publish pages
#

.PHONY: all publish clean

all: publish

publish: commit clean
	svn export file:///home/projects/cspay/svn/branches/alexandra/form/ \
		/home/projects/cspay/public_html/iw2/form

commit:
	cd /home/projects/cspay/checkout && svn add --force . && svn ci -m 'automatic commit'

clean:
	-rm -fr /home/projects/cspay/public_html/iw2/form/
