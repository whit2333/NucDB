FunctionsLibName := NucDBFunctions
FunctionsLibObjs := lib/NucDBFuncs.o 

FunctionsFortranLibObjs := 

#$(FunctionsFortranLibObjs) : $(builddir)/%.o :
#	gfortran -c $(F77FLAGS) -o $@  $(@:$(builddir)/%.o=$(srcdir)/%.f)
####### FORTRAN FILES ###############
# lib/F1F209.o : 
# 	gfortran -c -o $@  $(@:$(builddir)/%.o=$(srcdir)/%.f)


FunctionsLibHead := $(FunctionsLibObjs:lib/%.o=$(incdir)/%.h)

$(FunctionsLibObjs) : $(builddir)/%.o : %.h
	$(CXX) $(CXXFLAGS) -c -o $@  $(@:$(builddir)/%.o=$(srcdir)/%.C)

#$(srcdir)/$(FunctionsLibName)Dict.cxx $(srcdir)/$(FunctionsLibName)Dict.h : $(FunctionsLibHead)\

$(srcdir)/$(FunctionsLibName)Dict.cxx : $(FunctionsLibHead) $(incdir)/$(FunctionsLibName)_LinkDef.h 
	rootcling -f $(srcdir)/$(FunctionsLibName)Dict.cxx $(RCINTFLAGS)  \
	$(FunctionsLibHead)   $(incdir)/$(FunctionsLibName)_LinkDef.h 
	#mv $(srcdir)/$(FunctionsLibName)Dict.h $(incdir)/.
#_____________________________________________________________________________/
#$(builddir)/$(FunctionsLibName)Dict.o : $(FunctionsLibName)Dict.cxx $(incdir)/$(FunctionsLibName)Dict.h
$(builddir)/$(FunctionsLibName)Dict.o : $(FunctionsLibName)Dict.cxx 
	$(CXX) $(CXXFLAGS) -c -fPIC -o $@  $(srcdir)/$(FunctionsLibName)Dict.cxx 
#_____________________________________________________________________________/
$(FunctionsLibName) : $(FunctionsLibObjs) $(builddir)/$(FunctionsLibName)Dict.o 
	g++ $(CXXFLAGS) -shared  -Wl,-soname,lib$(FunctionsLibName).so.$(LIBMAJOR).$(LIBMINOR) -o \
        lib/lib$(FunctionsLibName).so.$(LIBMAJOR).$(LIBMINOR) $(FunctionsLibObjs) \
	$(builddir)/$(FunctionsLibName)Dict.o \
	$(LDLIBS)
#	$(FunctionsFortranLibObjs)


