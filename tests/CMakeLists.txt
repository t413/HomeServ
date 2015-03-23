cmake_minimum_required (VERSION 2.6)
project (test_homeserv CXX)


file(GLOB hdrs "*.h")
file(GLOB srcs "*.cpp")

findPoco( Net Util JSON Foundation )

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories("${libhomeserv_SOURCE_DIR}/src")

set(LIBS ${LIBS}
    libhomeserv
    ${GTEST_BOTH_LIBRARIES}
)

add_executable(test_homeserv ${hdrs} ${srcs})
target_link_libraries(test_homeserv ${LIBS})

enable_testing()
add_test(test_homeserv ${test_homeserv_BINARY_DIR})

