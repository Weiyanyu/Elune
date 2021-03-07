cmake_minimum_required (VERSION 2.8)

project (Elune)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# Bring the headers, such as Student.h into the project
add_library(Elune INTERFACE)
target_sources(Elune INTERFACE "${PROJECT_SOURCE_DIR}/lib/header/Elune.hpp")
target_include_directories(Elune INTERFACE 
    "${PROJECT_SOURCE_DIR}/lib/header"
)
find_package(Threads REQUIRED)

include_directories(./test/header/)

file(GLOB SOURCES "./test/src/*.cpp") 


add_executable(main ${SOURCES} main.cpp)
TARGET_LINK_LIBRARIES(main Threads::Threads Elune ${CMAKE_DL_LIBS})

