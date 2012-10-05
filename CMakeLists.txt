cmake_minimum_required(VERSION 2.6)
project(MOZARTVM)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-support")
option(BUILD_CSS "Builds the constraint subsystem" Yes)

include(ExternalProject)
set(ep_base "${CMAKE_BINARY_DIR}/externals")
set_property(DIRECTORY PROPERTY "EP_BASE" ${ep_base})

set(DEFAULT_LLVM_SRC_DIR "${ep_base}/Source/llvm")
set(DEFAULT_LLVM_BUILD_DIR "${ep_base}/Build/llvm")
set(LLVM_SRC_DIR ${DEFAULT_LLVM_SRC_DIR} CACHE PATH "Path to LLVM sources (with Clang)")
set(LLVM_BUILD_DIR ${DEFAULT_LLVM_BUILD_DIR} CACHE PATH "Path to LLVM build (with Clang)")
set(DEFAULT_GTEST_SRC_DIR "${ep_base}/Source/gtest")
set(DEFAULT_GTEST_BUILD_DIR "${ep_base}/Build/gtest")
set(GTEST_SRC_DIR ${DEFAULT_GTEST_SRC_DIR} CACHE PATH "Path to GTest sources")
set(GTEST_BUILD_DIR ${DEFAULT_GTEST_BUILD_DIR} CACHE PATH "Path to GTest build")

add_subdirectory(generator)

set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -O3 -DNDEBUG")

set(GENERATOR_EXTRA_FLAGS)
if(BUILD_CSS)
  include(Gecode)
  separate_arguments(GENERATOR_EXTRA_FLAGS UNIX_COMMAND "-DVM_HAS_CSS=1 -I${GECODE_INCLUDES}")
#  message(STATUS "Gecode headers: ${GECODE_INCLUDES}")
#  message(STATUS "Gecode libraries: ${GECODE_LIBRARIES}")

endif()

add_subdirectory(vm)
add_subdirectory(boostenv)
