cmake_minimum_required (VERSION 2.6)
project (homeserv)


## -------------- ##
##  find sources  ##
## -------------- ##

file(GLOB homeserv_hdrs "src/*.h")
file(GLOB homeserv_srcs "src/*.cpp")

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(_JSONCPP REQUIRED jsoncpp)

macro(findPoco) #variable args are each desired package
  set(POCO_LIBS "")
  foreach(f ${ARGN})
    find_library(POCO_LIB_${f} Poco${f})
    if ("${POCO_LIB_${f}}" STREQUAL "POCO_LIB_${f}-NOTFOUND")
      message(SEND_ERROR "can't find poco library '${f}'")
    endif()
    set(POCO_LIBS ${POCO_LIBS} ${POCO_LIB_${f}})
  endforeach()
endmacro()


## -------------- ##
## find libraries ##
## -------------- ##

findPoco( Net Util JSON Foundation )

find_library(LIBOPENZWAVE_LIBRARY openzwave )           ## find the lib
find_path(LIBOPENZWAVE_INCLUDE_DIR openzwave/Driver.h ) ## find the headers
include_directories("${LIBOPENZWAVE_INCLUDE_DIR}" "${LIBOPENZWAVE_INCLUDE_DIR}/openzwave")

set(LIBS ${LIBS}
    ${POCO_LIBS}
    ${LIBOPENZWAVE_LIBRARY}
)

add_executable(homeserv ${homeserv_hdrs} ${homeserv_srcs})
target_link_libraries(homeserv ${LIBS})


## ---------------------- ##
##  c++11 compiler check  ##
## ---------------------- ##

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(SEND_ERROR "requires c++11 ! (have ${CMAKE_CXX_COMPILER})")
endif()
