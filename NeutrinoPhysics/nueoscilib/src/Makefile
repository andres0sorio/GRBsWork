SHELL = /bin/sh
UNAME = $(shell uname)

# Root variables
ROOTCFLAGS   = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS     = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --libs) 
ROOTGLIBS    = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --glibs)

################
##linux
CXX          = g++
RM           = rm -f 
AR	     = ar cru

##Flags
CXXFLAGS     = -O0 -g -Wall -fPIC
#CXXFLAGS     += -D_DEBUG

GSLINC       =$(shell $(GSLPATH)/bin/gsl-config --cflags)
GSLLIB       =$(shell $(GSLPATH)/bin/gsl-config --libs)

BOOSTINC     =-I$(BOOSTPATH)/include

INCDIR       = .
LIBDIR       = ../lib

GARBAGE      = *.o $(LIBDIR)/*.so 

SOGARBAGE    = *.so 

CXXFLAGS    += -I$(INCDIR) $(ROOTCFLAGS) $(GSLINC) $(BOOSTINC)
LINKFLAGS    =

##Libraries
LIBS       += $(ROOTLIBS) -lMinuit -lMathCore -lMathMore -lXMLParser $(GSLLIB) \
	-L$(BOOSTPATH)/lib -lboost_program_options

OBJS  = Integrals.o \
	DOMParameters.o \
	nuN_interpxsec.o \
	PShadow.o \
	ShowerEvents.o \
	MuTrackEvents.o \
	IProbabilityMatrix.o \
	MixingParameters.o \
	ModelParameters.o \
	NeutrinoOscInVacuum.o \
	NeutrinoOscInMatter.o \
	DensityModels.o \
	NeutrinoOscInVarDensity.o

all : libnueosci.so

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< 	

libnueosci.so: $(OBJS)
	     $(CXX) -o $(LIBDIR)/$@ -shared $(OBJS) $(LINKFLAGS) $(LIBS) 

clean	:
	$(RM) $(GARBAGE)

cleanall:
	$(RM) $(GARBAGE) $(SOGARBAGE)

