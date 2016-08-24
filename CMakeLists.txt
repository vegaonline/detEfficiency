cmake_minimum_required(VERSION 3.0)
if (POLICY CMP0042)
    cmake_policy (SET CMP0042 OLD)
endif()

project(detEfficiency)
set(${PROJECT_NAME}_VERSION "1.0.0")
set(${PROJECT_NAME}_VERSION_MAJOR "1")
set(${PROJECT_NAME}_VERSION_MINOR "0")
set(${PROJECT_NAME}_VERSION_PATCH "0")
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "-w -fPIC -O0")
add_definitions(-std=c++11)

include_directories(include)
include_directories(src)

#set(LIBS )

set(exec_INCS include/main.h)

set(exec_SRCS src/main.cpp)

add_executable(detEff.e ${exec_INCS} ${exec_SRCS})

# target_link_libraries(detEff.e ${LIBS})