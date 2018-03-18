#
# MaBoSS project Makefile to install as library
#
# Institut Curie, France
# Gaelle Letort

CC = g++

install:
	make clean
	cd src && make CC=$(CC) && \
	ar rcs libMaBoSS.a MaBEstEngine.o BooleanGrammar.o Cumulator.o ProbaDist.o RunConfigGrammar.o BooleanNetwork.o RunConfig.o LogicalExprGen.o  && \
	mkdir -p ../lib/ && \
	mv libMaBoSS.a ../lib && \
	mkdir -p ../include/ && \
	cp *.h ../include/
	
all: clean install

clean:
	cd src && make clean && cd ../ 

