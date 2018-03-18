#
# MaBoSS project Makefile wrapper
#
# Institut Curie, France
#
# Eric Viara, Gautier Stoll March 2011
#

MAKEFILE = Makefile.maboss

all:
%:
	@rm -f depend.mk
	@touch depend.mk
	$(MAKE) CXX=$(CC) -f $(MAKEFILE) init
	$(MAKE) CXX=$(CC) -f $(MAKEFILE) depend
	$(MAKE) CXX=$(CC) -f $(MAKEFILE) $@

clean:
	@rm -f depend.mk
	@touch depend.mk
	$(MAKE) -f $(MAKEFILE) $@

