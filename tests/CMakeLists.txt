cmake_minimum_required (VERSION 2.6)
project (test_homeserv CXX)


file(GLOB hdrs "*.h")
file(GLOB srcs "*.cpp")

findPoco( Net Util JSON Foundation )

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_library(LIBOPENZWAVE_LIBRARY openzwave )           ## find the lib
find_path(LIBOPENZWAVE_INCLUDE_DIR openzwave/Driver.h ) ## find the headers
include_directories("${LIBOPENZWAVE_INCLUDE_DIR}" "${LIBOPENZWAVE_INCLUDE_DIR}/openzwave")

include_directories("${libhomeserv_SOURCE_DIR}/src")

set(LIBS ${LIBS}
    ${POCO_LIBS}
    ${LIBOPENZWAVE_LIBRARY}
    ${GTEST_BOTH_LIBRARIES}
)

add_executable(test_homeserv ${hdrs} ${srcs})
target_link_libraries(test_homeserv ${LIBS})

enable_testing()
add_test(test_homeserv ${test_homeserv_BINARY_DIR})

