cmake_minimum_required(VERSION 2.8)


project(sodium_bench)
file(GLOB_RECURSE libraries "./src/*.hpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -std=c++14 -Wall -pedantic")

add_executable(${PROJECT_NAME} ${libraries} "./src/main.cpp" "./src/utils.cpp" "./src/unit_tests.cpp")
target_link_libraries(sodium_bench sodium benchmark pthread)
