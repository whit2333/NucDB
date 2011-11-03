SHELL = /bin/bash
.SUFFIXES: .C .o .h .so 
VPATH = lib:src:include:
DESTDIR = bin 
basedir := ./
incdir := include
srcdir := src
builddir := lib

PROJECTNAME := NucDB
LIBNAME	  := NucDB

SOLIBNAMES := NucDBGeneral
LIBRARY	  = lib$(LIBNAME).so
LIBMAJOR  = 1
LIBOBJS	  = 1
LIBMINOR  = 1
LIBREVIS  = 0
SOLIBS := $(SOLIBNAMES:%=lib/lib%.so.$(LIBMAJOR).$(LIBMINOR) )


CPPFLAGS += $(shell root-config --cflags )
CPPFLAGS += -Iinclude -I.
#CPPFLAGS += -I$(shell lhapdf-config --incdir ) 
#LDLIBS += $(shell lhapdf-config --libdir ) 
CPPFLAGS += $(shell mysql_config --include ) 
CPPFLAGS +=  -g -pipe  -D_LARGEFILE_SOURCE -fno-strict-aliasing

#LDLIBS += $(shell lhapdf-config --ldflags)
LDLIBS += $(shell root-config --ldflags)
# LDLIBS +=  -lGeomPainter -lGeom -lSpectrum -lSpectrumPainter
#LDLIBS += $(shell gsl-config --libs)
LDLIBS += $(shell root-config --glibs --libs ) 
#-lTreePlayer -lGeomPainter -lGed -lRGL -lEve -lEG 
LDLIBS += -lg2c -lgfortran
LDLIBS += $(shell mysql_config --libs) 

CXX = g++
FC  = gfortran


CXXFLAGS  += -O2 -Wall -fPIC 
CXXFLAGS  += $(CPPFLAGS)

F77FLAGS += -g -O2 -Wall -fPIC -Iinclude 
##############################################################################/
default : nucdb

nucdb : shared_libs
	g++ -o nucdb NucDB.C $(CXXFLAGS) $(LDLIBS)
	mv nucdb bin/.


# Shared Libraries
##############################################################################/

convert : $(SOLIBNAMES)
	g++ $(CXXFLAGS) -shared  -Wl,-soname,$(LIBRARY).$(LIBMAJOR).$(LIBMINOR)\
	 -o lib/$(LIBRARY).$(LIBMAJOR).$(LIBMINOR)  \
	  $(SOLIBS) $(LDLIBS)


# Shared Libraries
##############################################################################/

shared_libs : $(SOLIBNAMES)
	g++ $(CXXFLAGS) -shared  -Wl,-soname,$(LIBRARY).$(LIBMAJOR).$(LIBMINOR)\
	 -o lib/$(LIBRARY).$(LIBMAJOR).$(LIBMINOR)  \
	  $(SOLIBS) $(LDLIBS)
#	ln -s $(builddir)/libInSANE.so.1.1 $(builddir)/libInSANE.so
#	ln -s $(builddir)/libInSANE.so.1.1 $(builddir)/libInSANE.so.1
#	ln -s $(builddir)/libInSANE.so.1.1 $(builddir)/libInSANE.so
#_____________________________________________________________________________/
# Utilitiy
##############################################################################/

doc: 
	doxygen doc/NucDB_Doxyfile #	doxygen doc/Doxyfile_insaneweb
	cd ..
	@echo "HTML Documentation created"

.PHONY : clean printstuff doc snapshot

snapshot:
	git archive HEAD --format=tar | gzip >NucDB-`date +%m_%d_%Y_`.tar.gz

#git archive HEAD --format=tar | gzip >InSANE-`date +%m_%d_%Y_`.tar.gz

clean:
	rm src/*Dict.cxx
	rm -rf lib/libNucDB*
	rm -rf lib/InSANE*Dict.o
	rm include/*Dict.h 
	rm -rf lib/*.o
	rm -rf bin/nucdb

printstuff : 
	@echo $(SOLIBS)
#	@ls lib/*.o

makefiledir := build
include  $(makefiledir)/General.mk

link:
# 	cd lib/ ; ln -sf libInSANEGeneral.so.$(LIBMAJOR).$(LIBMINOR) libInSANEGeneral.so
# 	cd lib/ ; ln -sf libInSANECut.so.$(LIBMAJOR).$(LIBMINOR) libInSANECut.so
# 	cd lib/ ; ln -sf libInSANEGeo.so.$(LIBMAJOR).$(LIBMINOR) libInSANEGeo.so
# 	cd lib/ ; ln -sf libInSANECluster.so.$(LIBMAJOR).$(LIBMINOR) libInSANECluster.so
# 	cd lib/ ; ln -sf libInSANEEvent.so.$(LIBMAJOR).$(LIBMINOR) libInSANEEvent.so
# 	cd lib/ ; ln -sf libInSANE.so.$(LIBMAJOR).$(LIBMINOR) libInSANE.so
