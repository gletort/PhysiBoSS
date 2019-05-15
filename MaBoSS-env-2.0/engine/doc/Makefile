
STYLE = native
#STYLE = db2latex
HTML_XSL_STYLESHEET = /usr/share/xml/docbook/stylesheet/nwalsh/html/docbook.xsl
#HTML_XSL_STYLESHEET = /usr/share/xml/docbook/stylesheet/nwalsh/html/chunk.xsl

all: maboss-refcard.pdf

%.pdf:	%.xml
	/usr/bin/dblatex -T $(STYLE) -p params.xsl -o $@ $<
	#/usr/bin/dblatex -p $(DOCBOOK_COMMON_DIR)/params.xsl -o $@ $<

%.html: %.xml
	xsltproc --nonet --xinclude --output $@ $(HTML_XSL_STYLESHEET) $<

