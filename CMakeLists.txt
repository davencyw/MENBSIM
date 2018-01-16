cmake_minimum_required(VERSION 2.8.3)
project(menbsim)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

##___________________________
##
##FLAGS
##

# set default build type to release for single generators
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RELEASE" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

SET (RELEASE_FLAGS " -std=c++11 -O3 -march=native -ftree-vectorize -fopenmp -lboost_program_options -lboost_filesystem -lboost_system -mtune=native ")
SET (DEBG_CMAKE_CXX_FLAGS "-Werror -Wall -g")

if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
  SET (RELEASE_FLAGS "${RELEASE_FLAGS} ")
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
  SET (RELEASE_FLAGS "${RELEASE_FLAGS} -Rpass=loop-vectorize")
endif()

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -mavx2 -mavx -msse -march=native -ftree-vectorize ")
## -fopt-info-vec-optimized

set (CMAKE_CXX_FLAGS_RELEASE "${RELEASE_FLAGS}" CACHE STRING "C++ Release flags" FORCE)
set (CMAKE_C_FLAGS_RELEASE "${RELEASE_FLAGS}" CACHE STRING "C Release flags" FORCE)

##___________________________
##
##FILES & FOLDERS
##

include_directories(include)
include_directories(ext)

#add_subdirectory(xxx)


##DIR
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


file (GLOB SOURCES
		src/main.cc
		src/menbsim.cc
		src/naivesolver.cc
		src/multipolesolver.cc
)

file (GLOB TESTSOURCES
		tests/test_octree.cc
)


##___________________________
##
##LIBRARIES & EXECUTABLES
##
#

add_executable (menbsim ${SOURCES})
add_executable (test ${TESTSOURCES})
