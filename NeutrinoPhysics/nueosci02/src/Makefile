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

INCDIR       = ../../nueoscilib/src -I.
SRCDIR       = .
NUOSCIDIR    = ../../nueoscilib
EXEDIR       = ../bin

GARBAGE      = *.o $(EXEDIR)/*.so $(EXEDIR)/*.d $(EXEDIR)/paper02.exe

SOGARBAGE    = *.so 

CXXFLAGS    += -I$(INCDIR) $(ROOTCFLAGS) $(GSLINC) $(BOOSTINC)
LINKFLAGS    =

##Libraries
LIBS       += $(ROOTLIBS) -lMinuit -lMathCore -lMathMore -lXMLParser $(GSLLIB)
LIBS       += -L$(BOOSTPATH)/lib -lboost_program_options 
LIBS       += -L$(NUOSCIDIR)/lib -lnueosci

PAP02OBJS =  Graphics.o \
	NeutrinosDetectionPaper.o \
	paper02.o

all : paper02.exe 

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< 	

paper02.exe: $(PAP02OBJS)
	 $(CXX) -o $(EXEDIR)/$@ $(PAP02OBJS) $(LINKFLAGS) $(LIBS)

clean	:
	$(RM) $(GARBAGE)

cleanall:
	$(RM) $(GARBAGE) $(SOGARBAGE)

