set(aname NucDB)
set(needs_libs)
set(libname "${aname}")
set(dictname "${libname}Dict")
set(lib_LINKDEF "${PROJECT_SOURCE_DIR}/src/${aname}/include/${libname}_LinkDef.h")
set(lib_DICTIONARY "${libname}Dict.cxx")

# ROOT dictionary files
if(NUCDB_USE_ROOT6)
   # ROOT6 generates pcm files that should install with into the lib directory
   # todo: fix the install of this
   set(lib_PCM_FILE "${PROJECT_BINARY_DIR}/src/${aname}/lib${libname}Dict_rdict.pcm")
endif(NUCDB_USE_ROOT6)
if(NOT NUCDB_USE_ROOT6)
   set(lib_DICTIONARY_HEADER "${libname}Dict.h")
endif(NOT NUCDB_USE_ROOT6)

include_directories(${PROJECT_SOURCE_DIR}/src/${aname}/include)

# add src file names here (without the extention or src/ prefix)
set(lib_files
   NucDBFuncs
   NucDBUtil
   NucDBErrorBar
   NucDBDataPoint
   NucDBMeasurement
   NucDBUnits
   NucDBManager
   NucDBBinnedVariable
   NucDBDependentVariable
   NucDBCalculation
   NucDBPaper
   NucDBReference
   NucDBExperiment
   NucDBDatabase
   Chi2Function
)

set(lib_Fortran_files
   )

set(lib_SRCS)
set(lib_HEADERS)
foreach(infileName ${lib_files})
   SET(lib_SRCS ${lib_SRCS} ${PROJECT_SOURCE_DIR}/src/${aname}/src/${infileName}.cxx)
   SET(lib_HEADERS ${lib_HEADERS} ${PROJECT_SOURCE_DIR}/src/${aname}/include/${infileName}.h)
endforeach(infileName)

set(lib_Fortran_SRCs)
foreach(infileName ${lib_Fortran_files})
   SET(lib_Fortran_SRCs ${lib_Fortran_SRCs} "${PROJECT_SOURCE_DIR}/src/${aname}/src/${infileName}.f")
endforeach(infileName)

#MESSAGE("(${dictname} ${lib_HEADERS} LINKDEF ${lib_LINKDEF}) ")
ROOT_GENERATE_DICTIONARY(${dictname} ${lib_HEADERS} LINKDEF ${lib_LINKDEF}
   OPTIONS -p)

SET(lib_SRCS ${lib_Fortran_SRCs} ${lib_SRCS} ${lib_DICTIONARY})

SET(lib_VERSION "${${PROJECT_NAME}_VERSION}")
SET(lib_MAJOR_VERSION "${${PROJECT_NAME}_MAJOR_VERSION}")
SET(lib_LIBRARY_PROPERTIES 
    VERSION "${lib_VERSION}"
    SOVERSION "${lib_MAJOR_VERSION}"
    SUFFIX ".so")

add_library(${libname} SHARED ${lib_SRCS})
target_link_libraries(${libname} ${LINK_LIBRARIES})
set_target_properties(${libname} PROPERTIES ${lib_LIBRARY_PROPERTIES})
#add_dependencies(${libname} ${needs_libs})
install(
   TARGETS ${libname} 
   #EXPORT ${PROJECT_NAME}Targets
   DESTINATION lib )

if(NUCDB_USE_ROOT6)
   install(
   FILES ${lib_PCM_FILE} 
   DESTINATION lib )
endif(NUCDB_USE_ROOT6)

install(
   FILES ${lib_HEADERS} 
   DESTINATION include/${PROJECT_NAME} )

#install(TARGETS foo
#  # IMPORTANT: Add the foo library to the "export-set"
#  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
#  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
#  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/foo"
#    COMPONENT dev)
