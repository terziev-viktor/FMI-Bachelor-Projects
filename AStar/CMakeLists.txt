cmake_minimum_required(VERSION 3.0.0)
project(AStar VERSION 0.1.0)

include(CTest)
enable_testing()
set(COMPILE_FLAGS "-std=c++2a -fconcepts")

add_executable(AStar main.cpp)

add_definitions(${COMPILE_FLAGS})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
