cmake_minimum_required(VERSION 3.16)
project(gtk_test)

set(CMAKE_CXX_STANDARD 17)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM-3 REQUIRED gtkmm-3.0)

include_directories(${GTKMM-3_INCLUDE_DIRS})
link_directories(${GTKMM-3_LIBRARY_DIRS})
list(APPEND FC_DEP_LIBS ${GTKMM-3_LIBRARIES})

add_executable(gtk_test main.cpp)
target_link_libraries (gtk_test ${FC_DEP_LIBS})