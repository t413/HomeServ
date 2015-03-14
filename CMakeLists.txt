cmake_minimum_required (VERSION 2.6)
project (homeserv)

file(GLOB homeserv_hdrs "src/*.h")
file(GLOB homeserv_srcs "src/*.cpp")

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(_LIBMICROHTTPD libmicrohttpd)
endif (PKG_CONFIG_FOUND)

# target_link_libraries(homeserv ${LIB} gtest gtest_main)

add_executable(homeserv ${homeserv_hdrs} ${homeserv_srcs})
