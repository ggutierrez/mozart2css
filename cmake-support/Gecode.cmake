message(STATUS "Gecode configuration")

set(DEFAULT_GECODE_SRC_DIR "${ep_base}/Source/gecode")
set(DEFAULT_GECODE_INSTALL_DIR "${ep_base}/Install/gecode")
set(GECODE_SRC_DIR ${DEFAULT_GECODE_SRC_DIR} CACHE PATH "Path to Gecode sources")
set(GECODE_INSTALL_DIR ${DEFAULT_GECODE_INSTALL_DIR} CACHE PATH "Path to Gecode installation")

set(GECODE_VERSION 3.7.3)

if("${GECODE_SRC_DIR}" STREQUAL "${DEFAULT_GECODE_SRC_DIR}" AND
   NOT EXISTS "${GECODE_BUILD_DIR}/lib/gecode/kernel.hh")
  set(GECODE_BUILD_DIR "${ep_base}/Build/gecode")
  ExternalProject_Add(
    gecode
    URL http://www.gecode.org/download/gecode-${GECODE_VERSION}.tar.gz
    SOURCE_DIR ${GECODE_SRC_DIR}
    BINARY_DIR ${GECODE_BUILD_DIR}
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ${GECODE_SRC_DIR}/configure
      --enable-static --disable-shared --prefix=${GECODE_INSTALL_DIR}
      --libdir=${GECODE_INSTALL_DIR}/lib
      --disable-gist --disable-flatzinc --disable-examples 
      --disable-qt --disable-driver --disable-minimodel
    BUILD_COMMAND make
    INSTALL_COMMAND make install    
  )
endif()

link_directories(${GECODE_INSTALL_DIR}/lib)
set(GECODE_LIBRARIES 
  gecodekernel
  gecodesupport
  gecodeint gecodeset  gecodesearch 
)
set(GECODE_INCLUDES ${GECODE_INSTALL_DIR}/include)

foreach(GECODE_LIB ${GECODE_LIBRARIES})
  add_library(${GECODE_LIB} STATIC IMPORTED)
  set_property(TARGET ${GECODE_LIB} PROPERTY 
               IMPORTED_LOCATION ${GECODE_INSTALL_DIR}/lib/lib${GECODE_LIB}.a)
endforeach()
  
message(STATUS "Gecode configuration ends")