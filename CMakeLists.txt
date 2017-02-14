cmake_minimum_required(VERSION 2.8)


project(sodium_bench)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O0 -std=c++14 -Wall -pedantic")

set("authentication.hpp")

add_executable(${PROJECT_NAME} "./src/main.cpp")
target_link_libraries(sodium_bench sodium benchmark pthread)
