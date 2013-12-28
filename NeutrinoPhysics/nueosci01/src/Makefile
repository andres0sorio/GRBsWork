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

NUOSCIDIR    = ../../nueoscilib

INCDIR       = ../../nueoscilib/src -I.
SRCDIR       = .
LIBDIR       = .
EXEDIR       = ../bin

GARBAGE      = *.o $(EXEDIR)/*.so $(EXEDIR)/*.d $(EXEDIR)/getratio.exe $(EXEDIR)/validate.exe $(EXEDIR)/paper01.exe $(EXEDIR)/earthval.exe

SOGARBAGE    = *.so 

CXXFLAGS    += -I$(INCDIR) $(ROOTCFLAGS) $(GSLINC) $(BOOSTINC)
LINKFLAGS    =

##Libraries
LIBS       += $(ROOTLIBS) -lMinuit -lMathCore -lMathMore -lXMLParser $(GSLLIB)
LIBS       += -L$(BOOSTPATH)/lib -lboost_program_options
LIBS       += -L$(NUOSCIDIR)/lib -lnueosci

OBJS   = Graphics.o \
	 main.o

VALOBJS =  Graphics.o \
	   NeutrinosInMediumPaper.o \
	   validate.o

PAP01OBJS = Graphics.o \
	    NeutrinosInMediumPaper.o \
	    paper01.o

EARTHOBJS = Graphics.o \
	    NeutrinosInMediumPaper.o \
	    earthval.o

all : getratio.exe validate.exe earthval.exe paper01.exe

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< 	

getratio.exe: $(OBJS)
	 $(CXX) -o $(EXEDIR)/$@ $(OBJS) $(LINKFLAGS) $(LIBS)

validate.exe: $(VALOBJS)
	 $(CXX) -o $(EXEDIR)/$@ $(VALOBJS) $(LINKFLAGS) $(LIBS)

paper01.exe: $(PAP01OBJS)
	 $(CXX) -o $(EXEDIR)/$@ $(PAP01OBJS) $(LINKFLAGS) $(LIBS)

earthval.exe: $(EARTHOBJS)
	 $(CXX) -o $(EXEDIR)/$@ $(EARTHOBJS) $(LINKFLAGS) $(LIBS)

clean	:
	$(RM) $(GARBAGE)

cleanall:
	$(RM) $(GARBAGE) $(SOGARBAGE)
