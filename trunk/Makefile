#
# publish pages
#

EXEC_SCRIPTS_FOLDER = /home/cspay/web-exec-scripts
SVN_REPO_BASE_URL = file:///home/cspay/svn/trunk
WEB_PUBLISH_FOLDER = /home/cspay/public_html/2009

.PHONY: all publish publish_web publish_scipts clean

all: publish

publish: publish_web publish_scripts
	-chown -R cspay:projects $(WEB_PUBLISH_FOLDER)

publish_web: clean_web
	-svn export --force $(SVN_REPO_BASE_URL)/webui $(WEB_PUBLISH_FOLDER)

publish_scripts: clean_scripts
	-svn export --force $(SVN_REPO_BASE_URL)/import $(EXEC_SCRIPTS_FOLDER)
	-svn export --force $(SVN_REPO_BASE_URL)/export $(EXEC_SCRIPTS_FOLDER)

clean: clean_web clean_scripts

clean_web:
	-rm -fr $(WEB_PUBLISH_FOLDER)/*

clean_scripts:
	-rm -fr $(EXEC_SCRIPTS_FOLDER)/*
