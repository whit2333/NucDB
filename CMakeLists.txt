#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(NucDB CXX Fortran)

# The version number.
set (NucDB_VERSION_MAJOR 1)
set (NucDB_VERSION_MINOR 0)

set (NucDB_CXX_CFLAGS)
set (NucDB_CXX_LDFLAGS)
set (NucDB_CXX_LIBS)
set (NucDB_CXX_INC_DIR)
set (NucDB_FC_CFLAGS)
set (NucDB_GRID_DATA_DIR)
set (NucDB_PCM_FILES)

set (PROJECT_USE_ROOT6 ${NUCDB_USE_ROOT6})

#-------------------------------------------------------
set(CMAKE_MODULE_PATH
   ${PROJECT_SOURCE_DIR}/cmake/Modules 
   ${PROJECT_SOURCE_DIR}/cmake
   ${CMAKE_MODULE_PATH}
   )

# Grid dir
set (NucDB_DATA_DIR "${PROJECT_SOURCE_DIR}/data")

macro(NucDB_CHECK_OUT_OF_SOURCE_BUILD)
   get_filename_component(bindir_parent ${CMAKE_BINARY_DIR} PATH)
   if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
      file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/Testing)
      file(REMOVE ${CMAKE_SOURCE_DIR}/DartConfiguration.tcl)
      message(FATAL_ERROR "NucDB should be built as an out of source build, to keep the source directory clean. Please create a extra build directory and run the command 'cmake <path_to_source_dir>' in this newly created directory. You have also to delete the directory CMakeFiles and the file CMakeCache.txt in the source directory. Otherwise cmake will complain even if you run it from an out-of-source directory.")
   elseif(IS_SYMLINK ${CMAKE_BINARY_DIR} AND CMAKE_SOURCE_DIR STREQUAL bindir_parent)
      message(FATAL_ERROR "NucDB cannot be built from a sub-directory of the source tree that is a symlink. This is a current limitation of CMake. Please create a real build directory and run the command 'cmake <path_to_source_dir>' in this newly created directory.")
   endif()
endmacro()

#Doesn't work with older cmake versions:
#NucDB_CHECK_OUT_OF_SOURCE_BUILD()

# Check the compiler and set the compile and link flags
#set(CMAKE_BUILD_TYPE Debug)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# not sure this is the best way to do this....
set(NucDB_FC_CFLAGS "${NucDB_FC_CFLAGS} ${CMAKE_Fortran_FLAGS}")
#set(CMAKE_Fortran_FLAGS " -D\"NucDB_PDF_GRID_DIR='${NucDB_PDF_GRID_DIR}'\" -g -O2 -Wall -fPIC -Iinclude -ffixed-line-length-none -x f77-cpp-input ")
set(CMAKE_Fortran_FLAGS " -D\"NucDB_PDF_GRID_DIR='${NucDB_PDF_GRID_DIR}'\" -g -O2 -Wall  -ffixed-line-length-none -x f77-cpp-input ")

#set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} ")

#----------------------------------------------------------------------------
# Find ROOT (required package)
find_package(ROOT QUIET)
if(NOT ROOT_FOUND)
   message(STATUS "ROOT package not found. ")  
   return()
endif()

#----------------------------------------------------------------------------
# Find CLHEP (required package)
#find_package(CLHEP QUIET)
#if(NOT CLHEP_FOUND)
#   message(STATUS "CLHEP package not found. ")  
#   return()
#endif()

#----------------------------------------------------------------------------
# Find MySQL (required package)
#find_package(MySQL QUIET)
#if(NOT MYSQL_FOUND)
#   message(STATUS "MySQL package not found. ")  
#   return()
#endif()

#----------------------------------------------------------------------------
# Find SQLite3 (required package)
#find_package(SQLite3 QUIET)
#if(NOT SQLITE3_FOUND)
#   message(STATUS "SQLite3 package not found. ")  
#   return()
#endif()


set(LINK_DIRECTORIES
   ${ROOT_LIBRARY_DIR}
   #${MYSQL_LIBRARY_DIR}
   #${CLHEP_LIBRARY_DIR}
   )
link_directories( ${LINK_DIRECTORIES})


set(INCLUDE_DIRECTORIES
   #${CLHEP_INCLUDE_DIR}
   #${MYSQL_INCLUDE_DIR}
   ${ROOT_INCLUDE_DIR} )

include_directories(${PROJECT_SOURCE_DIR}/include 
   "${CMAKE_BINARY_DIR}"
   ${INCLUDE_DIRECTORIES}
   )

message(STATUS " ${INCLUDE_DIRECTORIES} ")  

#---------------------------------------------------------------------------
# ROOT compile flags
EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

EXECUTE_PROCESS(COMMAND insane-config --inc OUTPUT_VARIABLE INSANE_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${INSANE_CXX_FLAGS}")

set(NucDB_CXX_CFLAGS "${NucDB_CXX_CFLAGS} ${CMAKE_CXX_FLAGS}")

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
#file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.C)
#file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the libraries 
#add_library(myObjects ${sources})


#---------------------------------------------------------------------------
# ROOT linking flags 
EXECUTE_PROCESS(COMMAND root-config --evelibs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ROOT_LD_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}   -lGeomPainter -lGeom -lEve -lTreeViewer -lSpectrum -lSpectrumPainter -lFoam ")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ROOT_LD_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -lGeomPainter -lGeom -lEve -lTreeViewer -lSpectrum -lSpectrumPainter -lFoam ")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}   -Wl,-rpath-link,/home/whit/lib ")

 

set(NucDB_CXX_LIBS "-L${CMAKE_INSTALL_PREFIX}/lib -lNucDB ${NucDB_CXX_LIBS} ")
set(NucDB_CXX_LDFLAGS "${NucDB_CXX_LDFLAGS} ${CMAKE_SHARED_LINKER_FLAGS}")
set(NucDB_CXX_INC_DIR "-I${CMAKE_INSTALL_PREFIX}/include/NucDB ")

# configure a header file to pass some of the CMake settings to the source code
configure_file (
   "${PROJECT_SOURCE_DIR}/NucDBConfig.h.in"
   "${PROJECT_BINARY_DIR}/NucDBConfig.h"
   )

#----------------------------------------------------------------------------
# Add the executable
add_executable(nucdb-config nucdb-config.cxx )
#add_executable(${myexe} ${mymain} ${sources} ${headers})

#target_link_libraries(NucDB ${myObjects} 
#   ${MYSQL_LIBRARIES} 
#   ${CLHEP_LIBRARIES} 
#   ${SQLITE3_LIBRARIES} 
#   ${ROOT_LIBRARIES} )

set(NucDB_LIB_NAMES
   General
   Functions
   )

set(NucDB_SHARED_LIBS)
set(NucDB_ROOT_DICTS)
set(NucDB_ROOT_DICT_HEADERS)


# ---------------------------------------
foreach(aSharedLib ${NucDB_LIB_NAMES})
   set(NucDB_SHARED_LIBS ${NucDB_SHARED_LIBS} "${CMAKE_BINARY_DIR}/lib/NucDB${aSharedLib}.so")
   set(NucDB_ROOT_DICT_SRCS ${NucDB_ROOT_DICT_SRCS} "NucDB${aSharedLib}Dict.cxx")
   if(NUCDB_USE_ROOT6)
      set(NucDB_PCM_FILES ${NucDB_PCM_FILES} "NucDB${aSharedLib}Dict_rdict.pcm")
   endif(NUCDB_USE_ROOT6)
   if(NOT NUCDB_USE_ROOT6)
      set(NucDB_ROOT_DICTS ${NucDB_ROOT_DICTS} "NucDB${aSharedLib}Dict.h")
      set(NucDB_ROOT_DICT_HEADERS ${NucDB_ROOT_DICT_HEADERS} "NucDB${aSharedLib}Dict.h")
   endif(NOT NUCDB_USE_ROOT6)
endforeach(aSharedLib)

ADD_CUSTOM_TARGET(ROOTDICTS DEPENDS ${NucDB_ROOT_DICT_SRCS} ${NucDB_ROOT_DICT_HEADERS} )

foreach(aSharedLib ${NucDB_LIB_NAMES})
   include(${aSharedLib})
   add_dependencies(NucDB${aSharedLib} ROOTDICTS)
endforeach(aSharedLib)


FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/include/*.h" "${PROJECT_BINARY_DIR}/*.h")
FILE(GLOB PCM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.pcm" "${PROJECT_SOURCE_DIR}/*.pcm" "${PROJECT_BINARY_DIR}/*.pcm")

# -----------------------------------------------------------
# Set the library version in the main CMakeLists.txt
SET(NucDB_MAJOR_VERSION 0)
SET(NucDB_MINOR_VERSION 0)
SET(NucDB_PATCH_VERSION 0)
SET(NucDB_VERSION "${NucDB_MAJOR_VERSION}.${NucDB_MINOR_VERSION}.${NucDB_PATCH_VERSION}")
SET(NucDB_LIBRARY_PROPERTIES ${NucDB_LIBRARY_PROPERTIES}
    VERSION "${NucDB_VERSION}"
    SOVERSION "${NucDB_MAJOR_VERSION}"
    SUFFIX ".so"
)
set(NucDB_SRCS
   ${General_SRCS}
   ${Functions_SRCS}
   )

add_library(NucDB SHARED ${NucDB_SRCS})
target_link_libraries(NucDB ${ROOT_LIBRARIES})
add_dependencies(NucDB ROOTDICTS)
set_target_properties(NucDB PROPERTIES ${NucDB_LIBRARY_PROPERTIES})

# ------------------------------------------------------------------------------
# Build the shared libraries 

foreach(aLib ${NucDB_LIB_NAMES})
   install(TARGETS "NucDB${aLib}" DESTINATION   lib)
endforeach(aLib)

#install(TARGETS Cluster DESTINATION lib) #${CMAKE_BINARY_DIR}/lib)
#install(TARGETS Cut DESTINATION     lib) #${CMAKE_BINARY_DIR}/lib)
#install(TARGETS Event DESTINATION   lib) #${CMAKE_BINARY_DIR}/lib)
#install(TARGETS General DESTINATION lib) #${CMAKE_BINARY_DIR}/lib)
#install(TARGETS Geo DESTINATION     lib) #${CMAKE_BINARY_DIR}/lib)
#install(TARGETS Math DESTINATION    lib) #${CMAKE_BINARY_DIR}/lib)
#install(TARGETS Physics DESTINATION lib) #${CMAKE_BINARY_DIR}/lib)

# ------------------------------------------------------------------------------
# Build the binary 
#install(TARGETS NucDB DESTINATION ${CMAKE_BINARY_DIR}/lib)
install(TARGETS NucDB DESTINATION lib)
INSTALL(FILES ${files} DESTINATION include/NucDB)
install(FILES ${PCM_FILES} DESTINATION lib)

install(TARGETS nucdb-config DESTINATION bin)

MESSAGE("          NucDB_CXX_LIBS: ${NucDB_CXX_LIBS}")
MESSAGE("       NucDB_CXX_LDFLAGS: ${NucDB_CXX_LDFLAGS}")
MESSAGE("       NucDB_CXX_INC_DIR: ${NucDB_CXX_INC_DIR}")
MESSAGE("             CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
MESSAGE("              FC_COMPILER: ${CMAKE_FC_COMPILER}")
MESSAGE("      CMAKE_Fortran_FLAGS: ${CMAKE_Fortran_FLAGS}")
MESSAGE("                CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
MESSAGE("         CMAKE_LINK_FLAGS: ${CMAKE_LINK_FLAGS}")
MESSAGE("CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")
MESSAGE(" PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
MESSAGE("       PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")


