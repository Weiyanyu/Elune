cmake_minimum_required (VERSION 2.8)

project (Elune)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

#Bring the headers, such as Student.h into the project
add_library(Elune INTERFACE)
target_sources(Elune INTERFACE "${PROJECT_SOURCE_DIR}/../lib/header/Elune.hpp")
target_include_directories(Elune INTERFACE 
    "${PROJECT_SOURCE_DIR}/../lib/header"
)
include_directories(./header/)

find_package(Threads REQUIRED)
  
file(GLOB SOURCES "./src/*.cpp")

add_library(test SHARED src/test.cpp)  
SET_TARGET_PROPERTIES (test PROPERTIES VERSION 1.2 SOVERSION 1)

target_link_libraries(test Elune Threads::Threads)
