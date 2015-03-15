cmake_minimum_required (VERSION 2.6)
project (homeserv)

file(GLOB homeserv_hdrs "src/*.h")
file(GLOB homeserv_srcs "src/*.cpp")

find_library(POCO_NET_LIB PocoNet)
find_library(POCO_UTIL_LIB PocoUtil)
find_library(POCO_JSON_LIB PocoJSON)
find_library(POCO_FOUNDATION_LIB PocoFoundation)
find_library(LIBOPENZWAVE_LIBRARY openzwave )           ## find the lib
find_path(LIBOPENZWAVE_INCLUDE_DIR openzwave/Driver.h ) ## find the headers
include_directories("${LIBOPENZWAVE_INCLUDE_DIR}" "${LIBOPENZWAVE_INCLUDE_DIR}/openzwave")

set(LIBS ${LIBS}
    ${POCO_NET_LIB}
    ${POCO_UTIL_LIB}
    ${POCO_FOUNDATION_LIB}
    ${POCO_JSON_LIB}
    ${LIBOPENZWAVE_LIBRARY}
)

add_executable(homeserv ${homeserv_hdrs} ${homeserv_srcs})
target_link_libraries(homeserv ${LIBS})

