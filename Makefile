VERSION := 1.0.0

PROGRAM_NAME := make_lib

CC := g++ -Wfatal-errors 
# CC := g++-mp-5 # typical macports compiler name
# CC := g++-5 # typical homebrew compiler name 

#----------- Compilation mode (debug or release)
MODE := R
# R = Release
# D = Debug
# P = Profiling


#---------- Organize in classic directories, build, bin, src
MABOSS_DIR = MaBoSS-env-2.0/engine/
BUILD_DIR = build
SRC_DIR = src
BIN_DIR = bin
CUR_DIR = $(shell pwd)

### MaBoSS directory
BOSS := MaBoSS-env-2.0/engine
BOSS_SRC := $(BOSS)/src
LIB := -L$(CUR_DIR)/$(BOSS)/lib -lMaBoSS
INC := -I$(CUR_DIR)/$(BOSS)/include

#---------- Folders to look in
vpath %.o     $(BUILD_DIR)
vpath %.cpp    $(SRC_DIR)/BioFVM $(SRC_DIR)/core $(SRC_DIR)/base $(SRC_DIR)/tinyxml 
vpath %.h     $(SRC_DIR)/BioFVM $(SRC_DIR)/core $(SRC_DIR)/base $(SRC_DIR)/tinyxml 


#ARCH := core2 # a reasonably safe default for most CPUs since 2007


#---------- Compilation flags for different modes
WARN     := -Wall -Wno-unknown-pragmas # -Wno-unused
DEBUG    := -g3 -fno-rtti -ggdb -std=c++0x -march=native -fopenmp -DNDEBUG
RELEASE  := -fno-tree-vectorize -fno-rtti -ffast-math -funroll-loops -finline-functions -Wno-deprecated
PROFILE  := $(RELEASE) -O0 -fno-inline -pg
FlagsD := -O0 $(DEBUG) $(WARN) 
FlagsR := $(RELEASE) -march=native -O3 -s -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++0x
FlagsP := $(PROFILE)

COMPILE_COMMAND := $(CC) $(Flags$(MODE)) 


#------------ Objexts to compile
BioFVM_OBJECTS := BioFVM_vector.o BioFVM_mesh.o BioFVM_microenvironment.o BioFVM_solvers.o BioFVM_matlab.o \
BioFVM_utilities.o BioFVM_basic_agent.o BioFVM_agent_container.o 

PhysiCell_core_OBJECTS := cell_cycle.o cell_cycle_classic.o cell_cycle_network.o cell_container.o cell.o sphere.o cell_line.o cell_phenotype.o \
simul.o cell_volume.o doSVG.o maboss_network.o

PhysiCell_base_OBJECTS := vector3d.o utilities.o readXML.o writeSVG.o

tinyxml_OBJECTS := tinyxml2.o

liver_OBJECTS := main.o

ALL_OBJECTS := $(BioFVM_OBJECTS) $(PhysiCell_core_OBJECTS) $(PhysiCell_module_OBJECTS) $(PhysiCell_base_OBJECTS) $(tinyxml_OBJECTS)

MAIN := ./src/main/PhysiBoSS.cpp
MAIN2 := ./src/main/createTxt.cpp
MAIN3 := ./src/main/plotTxt.cpp

all: make_lib physiboss create plot

install: maboss make_lib physiboss create plot

make_lib: $(ALL_OBJECTS) 

build:
	@mkdir -p $@

$(ALL_OBJECTS): %.o: %.cpp %.h | build 
	$(COMPILE_COMMAND) $(CXXFLAGS) $(INC) -c $< -o $(BUILD_DIR)/$(@F)

physiboss: $(BIN_DIR) $(MAIN)
	$(COMPILE_COMMAND) $(INC) -o $(BIN_DIR)/PhysiBoSS ./src/main/PhysiBoSS.cpp $(BUILD_DIR)/*.o $(LIB) -ldl

create: $(BIN_DIR) $(MAIN2) 
	$(COMPILE_COMMAND) -o $(BIN_DIR)/PhysiBoSS_CreateInitTxtFile ./src/main/createTxt.cpp $(BUILD_DIR)/*.o $(LIB) -ldl

plot: $(BIN_DIR) $(MAIN3) 
	$(COMPILE_COMMAND) $(INC) -o $(BIN_DIR)/PhysiBoSS_Plot ./src/main/plotTxt.cpp $(BUILD_DIR)/*.o $(LIB) -ldl

maboss:
	cd $(BOSS_SRC) && make clean && make install_alib	

#---------- Generate doc with Doxygen
.PHONY: doc clean cleanbin mrproper zip

doc: 
	doxygen Doxyfile


#---------- Cleaning and archiving

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(PROGRAM_NAME).exe
	rm -f ./examples/test_* 
	rm -f $(BIN_DIR)/* 

cleanbin:
	rm -f $(BIN_DIR)/*

mrproper: clean cleanbin

zip:
	zip $$(date +%b_%d_%Y_%H%M).zip */*.cpp */*.h *akefile* *.cpp *.h */*.hpp *.xml *.tex *.bib *hanges*.txt config/*.xml *.txt
	zip VERSION_$(VERSION).zip *.cpp *.h *akefile* *.xml *.tex *.bib *hanges*.txt *.txt
	mv *.zip archives/
