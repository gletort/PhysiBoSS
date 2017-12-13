#
# MaBoSS project Makefile to install as library
#
# Institut Curie, France
# Gaelle Letort
#
# Contact: physiboss@gmail.com

install:
	cd src && make  && \
	ar rcs libMaBoSS.a MaBEstEngine.o BooleanGrammar.o Cumulator.o ProbaDist.o RunConfigGrammar.o BooleanNetwork.o RunConfig.o LogicalExprGen.o  && \
	mkdir -p ../lib/ && \
	mv libMaBoSS.a ../lib && \
	mkdir -p ../include/ && \
	cp *.h ../include/
	
all: install
	

