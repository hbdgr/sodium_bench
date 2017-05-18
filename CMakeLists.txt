cmake_minimum_required(VERSION 2.8)


project(sodium_bench)
file(GLOB_RECURSE libraries "./src/*.hpp")
file(GLOB_RECURSE sources "./src/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -g3 -std=c++14")

#add_executable(${PROJECT_NAME} ${libraries} "./src/main.cpp" "./src/utils.cpp" "./src/unit_tests.cpp")
add_executable(${PROJECT_NAME} ${sources} ${libraries})
target_link_libraries(sodium_bench sodium benchmark pthread)
