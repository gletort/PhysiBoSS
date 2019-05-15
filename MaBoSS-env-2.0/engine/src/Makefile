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
	$(MAKE) -f $(MAKEFILE) init
	$(MAKE) -f $(MAKEFILE) depend
	$(MAKE) -f $(MAKEFILE) $@

clean:
	@rm -f depend.mk
	@touch depend.mk
	$(MAKE) -f $(MAKEFILE) $@

