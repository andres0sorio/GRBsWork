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
SRCDIR       = .
LIBDIR       = .
EXEDIR       = ../bin

GARBAGE      = *.o $(EXEDIR)/evalxsec.exe $(EXEDIR)/validate.exe $(EXEDIR)/paper01.exe

SOGARBAGE    = *.so

CXXFLAGS    += -I$(INCDIR) $(ROOTCFLAGS) $(GSLINC) $(BOOSTINC)
LINKFLAGS    =

##Libraries
LIBS       += $(ROOTLIBS) -lMinuit -lMathCore -lMathMore -lXMLParser $(GSLLIB) \
	-L$(BOOSTPATH)/lib -lboost_program_options

OBJS   = Graphics.o Integrals.o DOMParameters.o nuN_interpxsec.o PShadow.o \
	ShowerEvents.o MuTrackEvents.o IProbabilityMatrix.o \
	MixingParameters.o NeutrinoOscInVacuum.o NeutrinoOscInMatter.o main.o

VALOBJS = DOMParameters.o MixingParameters.o IProbabilityMatrix.o \
	DensityModels.o NeutrinoOscInVarDensity.o validate.o

PAP01OBJS = DOMParameters.o MixingParameters.o IProbabilityMatrix.o \
	DensityModels.o NeutrinoOscInVarDensity.o paper01.o

all : evalxsec.exe validate.exe paper01.exe

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< 	

evalxsec.exe: $(OBJS)
	 $(CXX) -o $(EXEDIR)/$@ $(OBJS) $(LINKFLAGS) $(LIBS)

validate.exe: $(VALOBJS)
	 $(CXX) -o $(EXEDIR)/$@ $(VALOBJS) $(LINKFLAGS) $(LIBS)

paper01.exe: $(PAP01OBJS)
	 $(CXX) -o $(EXEDIR)/$@ $(PAP01OBJS) $(LINKFLAGS) $(LIBS)

clean	:
	$(RM) $(GARBAGE)

cleanall:
	$(RM) $(GARBAGE) $(SOGARBAGE)
