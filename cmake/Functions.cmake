#---------------------------------------------------------------------------
set(Functions_files 
   NucDBFuncs
   )

set(Functions_SRCS)
set(Functions_HEADERS)
foreach(infileName ${Functions_files})
   SET(Functions_SRCS ${Functions_SRCS} ${PROJECT_SOURCE_DIR}/src/${infileName}.C)
   SET(Functions_HEADERS ${Functions_HEADERS} ${PROJECT_SOURCE_DIR}/include/${infileName}.h)
endforeach(infileName)

# set everything needed for the root dictonary and create the
# dictionary
set(Functions_LINKDEF ${PROJECT_SOURCE_DIR}/include/NucDBFunctions_LinkDef.h )
set(Functions_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/NucDBFunctionsDict.cxx) 
ROOT_GENERATE_DICTIONARY("${Functions_HEADERS}" "${Functions_LINKDEF}" "${Functions_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

# add the dictionary to the list of source files
SET(Functions_SRCS ${Functions_SRCS} ${Functions_DICTIONARY}) 

# Set the library version in the main CMakeLists.txt
SET(Functions_MAJOR_VERSION 0)
SET(Functions_MINOR_VERSION 0)
SET(Functions_PATCH_VERSION 0)
SET(Functions_VERSION "${Functions_MAJOR_VERSION}.${Functions_MINOR_VERSION}.${Functions_PATCH_VERSION}")
SET(Functions_LIBRARY_PROPERTIES ${Functions_LIBRARY_PROPERTIES}
    VERSION "${Functions_VERSION}"
    SOVERSION "${Functions_MAJOR_VERSION}"
    SUFFIX ".so"
)

add_library(          NucDBFunctions SHARED ${Functions_SRCS})
target_link_libraries(NucDBFunctions ${ROOT_LIBRARIES})
set_target_properties(NucDBFunctions PROPERTIES ${Functions_LIBRARY_PROPERTIES})

#install(TARGETS Functions DESTINATION ${CMAKE_BINARY_DIR}/lib)

