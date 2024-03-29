# Mozart VM library

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../main" "${CMAKE_CURRENT_BINARY_DIR}/../main")

# GTest libraries

add_library(custom_gtest STATIC IMPORTED)
set_property(TARGET custom_gtest PROPERTY
             IMPORTED_LOCATION "${GTEST_BUILD_DIR}/libgtest.a")

add_library(custom_gtest_main STATIC IMPORTED)
set_property(TARGET custom_gtest_main PROPERTY
             IMPORTED_LOCATION "${GTEST_BUILD_DIR}/libgtest_main.a")

include_directories("${GTEST_SRC_DIR}" "${GTEST_SRC_DIR}/include")

# The testing executable
if(BUILD_CSS)
  include_directories(${GECODE_INCLUDES})
  add_definitions(-DVM_HAS_CSS=1)
endif()

add_executable(vmtest testutils.cc sanitytest.cc coderstest.cc utftest.cc
  smallinttest.cc floattest.cc atomtest.cc stringtest.cc virtualstringtest.cc
  bytestringtest.cc gctest.cc csttest.cc)
target_link_libraries(vmtest mozartvm pthread custom_gtest custom_gtest_main)
