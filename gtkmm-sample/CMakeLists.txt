cmake_minimum_required(VERSION 3.2)
project(gtkmm-sample)
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
 
set(SOURCE_FILES main.cc examplewindow.cc)
add_executable(gtkmm-sample ${SOURCE_FILES})
 
# configure modules
find_package(PkgConfig REQUIRED)
 
# gtkmm-3.0
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0>=3.22)
 
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
 
add_definitions(${GTKMM_CFLAGS_OTHER})
target_link_libraries(gtkmm-sample ${GTKMM_LIBRARIES})
