cmake_minimum required(VERSION 3.25.1)
project(Modeler)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk-4.0)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

set(SOURCES window.cpp)

add_executable(myproject ${SOURCES})

target_link_libraries(Modeler ${GTK4_LIBRARIES})
