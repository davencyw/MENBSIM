cmake_minimum_required(VERSION 2.8.3)
project(menbsim)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

##___________________________
##
##BUILD TYPE
##

# SET default build type to release for single generators
if (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE "RELEASE" FORCE)
endif ()

SET (CMAKE_BUILD_TYPE "DEBUG")

message("mode")
message("${CMAKE_BUILD_TYPE}")
message("___________________")
message("___________________")
message("___________________")


##___________________________
##
##BENCHMARKING
##

##___________________________
##
##FLAGS
##

SET (RELEASE_FLAGS " -std=c++11 -O3 -march=native  -mtune=native -mavx2 -mavx -msse -ftree-vectorize")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
  SET (RELEASE_FLAGS "${RELEASE_FLAGS} ")
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
  SET (RELEASE_FLAGS "${RELEASE_FLAGS} -Rpass=loop-vectorize")
endif()

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread  -fopenmp -lboost_program_options -lboost_filesystem -lboost_system ")
## -fopt-info-vec-optimized

SET (CMAKE_CXX_FLAGS_RELEASE "${RELEASE_FLAGS}" CACHE STRING "C++ Release flags" FORCE)

SET (CMAKE_C_FLAGS_RELEASE "${RELEASE_FLAGS}" CACHE STRING "C Release flags" FORCE)

##___________________________
##
##LIBRARIES
##

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

##___________________________
##
##FILES & FOLDERS
##

include_directories(include)
include_directories(ext)

#add_subdirectory(xxx)


##DIR
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dll)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

file(GLOB BENCHSOURCES
		ext/bench/bench.cpp
)

file(GLOB OCTREESOURCES
		src/octree/octreenode.cc
		src/octree/octree.cc
)

file (GLOB SOURCESOLVER
    src/solver/naivesolver.cc
    src/solver/multipolesolver.cc
)

file (GLOB SOURCESSIM
		src/main.cc
		src/menbsim.cc
)

file (GLOB TESTSOURCES
    tests/test_main.cc
		tests/test_octree.cc
    tests/test_multipolesolver.cc
)


##___________________________
##
##LIBRARIES & EXECUTABLES
##

add_library(octree ${OCTREESOURCES})
add_library(bench ${BENCHSOURCES})
add_library(solver ${SOURCESOLVER})
add_library(solvernobench ${SOURCESOLVER})
target_link_libraries(solver bench octree)
target_link_libraries(solvernobench octree)

add_executable (menbsim ${SOURCESSIM})
target_link_libraries(menbsim solvernobench)
add_executable(menbsim_profiling ${SOURCESSIM})
target_link_libraries(menbsim_profiling bench solver)

add_executable (runtests ${TESTSOURCES})
target_link_libraries(runtests ${GTEST_LIBRARIES} pthread solvernobench)


target_compile_definitions(bench PRIVATE -DCCPP_BENCH)
target_compile_definitions(menbsim_profiling PRIVATE -DCCPP_BENCH)
target_compile_definitions(solver PRIVATE -DCCPP_BENCH)
