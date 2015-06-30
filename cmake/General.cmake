#---------------------------------------------------------------------------
set(General_files 
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
   )

set(General_SRCS)
set(General_HEADERS)
foreach(infileName ${General_files})
   SET(General_SRCS ${General_SRCS} ${PROJECT_SOURCE_DIR}/src/${infileName}.C)
   SET(General_HEADERS ${General_HEADERS} ${PROJECT_SOURCE_DIR}/include/${infileName}.h)
endforeach(infileName)

# set everything needed for the root dictonary and create the
# dictionary
set(General_LINKDEF ${PROJECT_SOURCE_DIR}/include/NucDBGeneral_LinkDef.h )
set(General_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/NucDBGeneralDict.cxx) 
#ROOT_GENERATE_DICTIONARY("${General_HEADERS}" "${General_LINKDEF}" "${General_DICTIONARY}" "${INCLUDE_DIRECTORIES}")
ROOT_GENERATE_DICTIONARY(${General_DICTIONARY} ${General_HEADERS} LINKDEF ${General_LINKDEF} OPTIONS -p)

# add the dictionary to the list of source files
SET(General_SRCS ${General_SRCS} ${General_DICTIONARY}) 

# Set the library version in the main CMakeLists.txt
SET(General_MAJOR_VERSION 0)
SET(General_MINOR_VERSION 0)
SET(General_PATCH_VERSION 0)
SET(General_VERSION "${General_MAJOR_VERSION}.${General_MINOR_VERSION}.${General_PATCH_VERSION}")
SET(General_LIBRARY_PROPERTIES ${General_LIBRARY_PROPERTIES}
    VERSION "${General_VERSION}"
    SOVERSION "${General_MAJOR_VERSION}"
    SUFFIX ".so"
)

add_library(          NucDBGeneral SHARED ${General_SRCS})
target_link_libraries(NucDBGeneral ${ROOT_LIBRARIES})
set_target_properties(NucDBGeneral PROPERTIES ${General_LIBRARY_PROPERTIES})

#install(TARGETS General DESTINATION ${CMAKE_BINARY_DIR}/lib)

