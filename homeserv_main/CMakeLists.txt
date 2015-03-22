cmake_minimum_required (VERSION 2.6)
project (homeserv_main)


file(GLOB hdrs "*.h")
file(GLOB srcs "*.cpp")

include_directories("${libhomeserv_SOURCE_DIR}/src")

set(LIBS ${LIBS}
    libhomeserv
)

add_executable(homeserv_main ${hdrs} ${srcs})
target_link_libraries(homeserv_main ${LIBS})
