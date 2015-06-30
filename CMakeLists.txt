#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(NucDB CXX)

set(NucDB_LIB_NAMES 
   NucDB
   )
set(NucDB_EXE_NAMES
   )

# The version number.
set(NucDB_MAJOR_VERSION 1)
set(NucDB_MINOR_VERSION 0)
set(NucDB_PATCH_VERSION 0)
set(NucDB_VERSION ${NucDB_MAJOR_VERSION}.${NucDB_MINOR_VERSION}.${NucDB_PATCH_VERSION})

set (PROJECT_USE_ROOT6 ${NUCDB_USE_ROOT6})
# ---------------------------------------------------------------------------
# Configure the find_package outputs for this library
# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
   set(DEF_INSTALL_CMAKE_DIR CMake)
else()
   set(DEF_INSTALL_CMAKE_DIR lib/${PROJECT_NAME})
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

#MESSAGE("installprefix : ${CMAKE_INSTALL_PREFIX}")
if(NOT CMAKE_INSTALL_PREFIX)
   SET(NucDB_PREFIX "/usr/local")
else() 
   SET(NucDB_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()

# ---------------------------------------------------------------------------
#
set (NucDB_CXX_CFLAGS)
set (NucDB_CXX_LDFLAGS)
set (NucDB_CXX_LIBS)
set (NucDB_CXX_INC_DIR)
set (NucDB_FC_CFLAGS)
set (NucDB_GRID_DATA_DIR)
set (NucDB_PCM_FILES)
set (NucDB_DATA_DIR "${PROJECT_SOURCE_DIR}/data")


set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# ---------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${PROJECT_SOURCE_DIR}/cmake/Modules 
   ${PROJECT_SOURCE_DIR}/cmake
   )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# ---------------------------------------------------------------------------
# Must be out of source build
macro(NucDB_CHECK_OUT_OF_SOURCE_BUILD)
   get_filename_component(bindir_parent ${CMAKE_BINARY_DIR} PATH)
   if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
      file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/Testing)
      file(REMOVE ${CMAKE_SOURCE_DIR}/DartConfiguration.tcl)
      message(FATAL_ERROR "NucDB should be built as an out of source build, to keep the source directory clean. Please create a extra build directory and run the command 'cmake <path_to_source_dir>' in this newly created directory.  You have also to delete the directory CMakeFiles and the file CMakeCache.txt in the source directory. Otherwise cmake will complain even if you run it from an out-of-source directory.")
   elseif(IS_SYMLINK ${CMAKE_BINARY_DIR} AND CMAKE_SOURCE_DIR STREQUAL bindir_parent)
      message(FATAL_ERROR "NucDB cannot be built from a sub-directory of the source tree that is a symlink. This is a current limitation of CMake. Please create a real build directory and run the command 'cmake <path_to_source_dir>' in this newly created directory.")
   endif()
endmacro()
# NOTE: Doesn't work with older cmake versions:
NucDB_CHECK_OUT_OF_SOURCE_BUILD()

# ----------------------------------------------------------------------------
# Compiler Options 

#set(CMAKE_BUILD_TYPE Debug)

# --------
# C++
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} ")

# not sure this is the best way to do this....
set(NucDB_FC_CFLAGS "${NucDB_FC_CFLAGS} ${CMAKE_Fortran_FLAGS}")
#set(CMAKE_Fortran_FLAGS " -D\"NucDB_PDF_GRID_DIR='${NucDB_PDF_GRID_DIR}'\" -g -O2 -Wall -fPIC -Iinclude -ffixed-line-length-none -x f77-cpp-input ")
set(CMAKE_Fortran_FLAGS " -D\"NucDB_PDF_GRID_DIR='${NucDB_PDF_GRID_DIR}'\" -g -O2 -Wall  -ffixed-line-length-none -x f77-cpp-input ")

#---------------------------------------------------------------------------
# ROOT compile flags
#EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
#
#EXECUTE_PROCESS(COMMAND insane-config --inc OUTPUT_VARIABLE INSANE_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${INSANE_CXX_FLAGS}")
#
#set(NucDB_CXX_CFLAGS "${NucDB_CXX_CFLAGS} ${CMAKE_CXX_FLAGS}")

#----------------------------------------------------------------------------
# Find ROOT (required package)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist MathMore Graf Graf3d
   Gpad GX11 GeomPainter X3d Gviz3d RGL
   Eve EG Foam GenVector Geom GLEW Gui HistPainter MathCore Matrix MLP Net
   Physics Spectrum Thread Tree TreeViewer)
include(${ROOT_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

find_package(CLHEP REQUIRED)


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

#message(STATUS " ${INCLUDE_DIRECTORIES} ")  

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

# ---------------------------------------
set(NucDB_CXX_LIBS  "-lNucDB${aSharedLib} ")

# configure a header file to pass some of the CMake settings to the source code
configure_file (
   "${PROJECT_SOURCE_DIR}/NucDBConfig.h.in"
   "${PROJECT_BINARY_DIR}/NucDBConfig.h"
   )

#----------------------------------------------------------------------------
# Libraries 
foreach(aSharedLib ${NucDB_LIBRARIES})
  include_directories(${PROJECT_SOURCE_DIR}/${aSharedLib}/include)
  add_subdirectory("src/${aSharedLib}")
endforeach(aSharedLib)

# ---------------------------------------------------------------------------
# Executables 
#add_subdirectory(config)
#add_subdirectory(runtest)

#----------------------------------------------------------------------------
# Add the executable
#add_executable(nucdb-config nucdb-config.cxx )
#add_executable(${myexe} ${mymain} ${sources} ${headers})

#target_link_libraries(NucDB ${myObjects} 
#   ${MYSQL_LIBRARIES} 
#   ${CLHEP_LIBRARIES} 
#   ${SQLITE3_LIBRARIES} 
#   ${ROOT_LIBRARIES} )

set(NucDB_SHARED_LIBS)
set(NucDB_ROOT_DICTS)
set(NucDB_ROOT_DICT_HEADERS)

# ---------------------------------------
foreach(aSharedLib ${NucDB_LIB_NAMES})
   set(NucDB_CXX_LIBS      "${NucDB_CXX_LIBS} -l${aSharedLib} ")
endforeach(aSharedLib)

# ---------------------------------------
foreach(aSharedLib ${NucDB_LIB_NAMES})
   set(NucDB_SHARED_LIBS ${NucDB_SHARED_LIBS} "${CMAKE_BINARY_DIR}/lib/${aSharedLib}.so")
   set(NucDB_ROOT_DICT_SRCS ${NucDB_ROOT_DICT_SRCS} "${aSharedLib}Dict.cxx")
   if(NUCDB_USE_ROOT6)
      #set(lib_PCM_FILE "${PROJECT_BINARY_DIR}/src/${aname}/${libname}Dict_rdict.pcm")
      set(NucDB_PCM_FILES ${NucDB_PCM_FILES} "${PROJECT_BINARY_DIR}/${aSharedLib}Dict_rdict.pcm")
   endif(NUCDB_USE_ROOT6)
   if(NOT NUCDB_USE_ROOT6)
      set(NucDB_ROOT_DICTS ${NucDB_ROOT_DICTS} "${aSharedLib}Dict.h")
      set(NucDB_ROOT_DICT_HEADERS ${NucDB_ROOT_DICT_HEADERS} "${aSharedLib}Dict.h")
   endif(NOT NUCDB_USE_ROOT6)
endforeach(aSharedLib)

foreach(aSharedLib ${NucDB_LIB_NAMES})
   #include(${aSharedLib})
   add_subdirectory("src/${aSharedLib}")
endforeach(aSharedLib)

foreach(anExeName ${NucDB_EXE_NAMES})
   #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/${anExeName}/include)
   add_subdirectory("src/${anExeName}")
endforeach(anExeName)

add_subdirectory("config")

export(PACKAGE ${PROJECT_NAME})

FILE(GLOB GENERATED_HEADERS 
   "${CMAKE_CURRENT_SOURCE_DIR}/*.h" 
   "${PROJECT_BINARY_DIR}/NucDBConfig.h")
FILE(GLOB PCM_FILES "${PROJECT_BINARY_DIR}/*.pcm")

# -----------------------------------------------------------
# Set the library version in the main CMakeLists.txt
SET(NucDB_LIBRARY_PROPERTIES ${NucDB_LIBRARY_PROPERTIES}
    VERSION "${NucDB_VERSION}"
    SOVERSION "${NucDB_MAJOR_VERSION}"
    SUFFIX ".so"
)

set(NucDB_SRCS
   ${Cluster_SRCS}
   ${Cut_SRCS}
   ${Event_SRCS}
   ${General_SRCS}
   ${Geo_SRCS}
   ${Math_SRCS}
   ${Neural_SRCS}
   ${Physics_SRCS}
   ${Physics2_SRCS}
   )
set_source_files_properties(
   ${NucDB_SRCS}
   PROPERTIES COMPILE_FLAGS "")
#   PROPERTIES COMPILE_FLAGS "-Wall  -Wextra -Wcomment ")

#add_library(NucDB SHARED ${NucDB_SRCS})
#target_link_libraries(NucDB ${ROOT_LIBRARIES})
#add_dependencies(NucDB ROOTDICTS)
#set_target_properties(NucDB PROPERTIES ${NucDB_LIBRARY_PROPERTIES})

# ------------------------------------------------------------------------------
# Install the libraries 
#foreach(aLib ${NucDB_LIB_NAMES})
#   install(TARGETS "NucDB${aLib}" DESTINATION   lib)
#endforeach(aLib)

# ------------------------------------------------------------------------------
# Build the binary 
#install(TARGETS NucDB DESTINATION lib)
INSTALL(FILES ${files} DESTINATION include/NucDB)
install(FILES ${PCM_FILES} DESTINATION lib)
#install(TARGETS NucDB-config DESTINATION bin)


# ----------------------------------------------------------------------------
# The cmake stuff goes here

# Add all targets to the build-tree export set
#export(TARGETS NucDB FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")
# TODO: replace "NucDB" with smaller libraries

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE ${PROJECT_NAME})

set(CONF_LIBRARIES 
   ${PROJECT_NAME}  # TODO: Break big library into smaller ones
   )

set(CONF_EXECUTABLES 
   NucDB-config
   )

set(CONF_INCLUDE_DIRS 
   ${CMAKE_INSTALL_PREFIX}/include
   ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}
   )

set(CONF_LIBRARY_DIRS 
   ${CMAKE_INSTALL_PREFIX}/lib
   )

configure_file(${PROJECT_NAME}Config.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY)
configure_file(${PROJECT_NAME}ConfigVersion.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)

# Install the FooBarConfig.cmake and FooBarConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT ${PROJECT_NAME}Targets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)

#MESSAGE("          NucDB_CXX_LIBS: ${NucDB_CXX_LIBS}")
#MESSAGE("       NucDB_CXX_LDFLAGS: ${NucDB_CXX_LDFLAGS}")
#MESSAGE("       NucDB_CXX_INC_DIR: ${NucDB_CXX_INC_DIR}")
#MESSAGE("             CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
#MESSAGE("              FC_COMPILER: ${CMAKE_FC_COMPILER}")
#MESSAGE("      CMAKE_Fortran_FLAGS: ${CMAKE_Fortran_FLAGS}")
#MESSAGE("                CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
#MESSAGE("         CMAKE_LINK_FLAGS: ${CMAKE_LINK_FLAGS}")
#MESSAGE("CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")
#MESSAGE(" PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
#MESSAGE("       PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
