
GeneralLibName := NucDBGeneral

GeneralLibObjs := lib/NucDBDataPoint.o \
                  lib/NucDBMeasurement.o \
                  lib/NucDBUnits.o \
                  lib/NucDBManager.o \
                  lib/NucDBBinnedVariable.o \
                  lib/NucDBDependentVariable.o \
                  lib/NucDBCalculation.o \
                  lib/NucDBPaper.o \
                  lib/NucDBReference.o \
                  lib/NucDBExperiment.o \
                  lib/NucDBDatabase.o


FortranLibObjs := 

$(FortranLibObjs) : $(builddir)/%.o :
	$(FC) -c $(F77FLAGS) -o $@  $(@:$(builddir)/%.o=$(srcdir)/%.f)


GeneralLibHead := $(GeneralLibObjs:lib/%.o=$(incdir)/%.h)

$(GeneralLibObjs) : $(builddir)/%.o : %.h
	$(CXX) $(CXXFLAGS) -c -o $@  $(@:$(builddir)/%.o=$(srcdir)/%.C)

<<<<<<< HEAD
$(srcdir)/$(GeneralLibName)Dict.cxx $(srcdir)/$(GeneralLibName)Dict.h : $(GeneralLibHead)\
       $(incdir)/$(GeneralLibName)_LinkDef.h 
	rootcint -v4 -f $(srcdir)/$(GeneralLibName)Dict.cxx $(ROOTCINTFLAGS)  \
=======
#$(srcdir)/$(GeneralLibName)Dict.cxx $(srcdir)/$(GeneralLibName)Dict.h : $(GeneralLibHead)\

$(srcdir)/$(GeneralLibName)Dict.cxx : $(GeneralLibHead) $(incdir)/$(GeneralLibName)_LinkDef.h 
	rootcling -f $(srcdir)/$(GeneralLibName)Dict.cxx $(RCINTFLAGS)  \
>>>>>>> c8421683543365e914f6de77b40d2752d22a7e1b
	$(GeneralLibHead)   $(incdir)/$(GeneralLibName)_LinkDef.h 
	#mv $(srcdir)/$(GeneralLibName)Dict.h $(incdir)/.
#_____________________________________________________________________________/
<<<<<<< HEAD
$(builddir)/$(GeneralLibName)Dict.o : $(GeneralLibName)Dict.cxx $(GeneralLibName)Dict.h
=======
#$(builddir)/$(GeneralLibName)Dict.o : $(GeneralLibName)Dict.cxx $(incdir)/$(GeneralLibName)Dict.h

$(builddir)/$(GeneralLibName)Dict.o : $(GeneralLibName)Dict.cxx 
>>>>>>> c8421683543365e914f6de77b40d2752d22a7e1b
	$(CXX) $(CXXFLAGS) -c -fPIC -o $@  $(srcdir)/$(GeneralLibName)Dict.cxx 
#_____________________________________________________________________________/
$(GeneralLibName) : $(GeneralLibObjs) $(builddir)/$(GeneralLibName)Dict.o $(FortranLibObjs)
	g++ $(CXXFLAGS) -shared  -Wl,-soname,lib$(GeneralLibName).so.$(LIBMAJOR).$(LIBMINOR) -o \
        lib/lib$(GeneralLibName).so.$(LIBMAJOR).$(LIBMINOR) $(GeneralLibObjs) $(builddir)/$(GeneralLibName)Dict.o $(FortranLibObjs)  $(LDLIBS)
