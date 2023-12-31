cmake_minimum_required(VERSION 3.26)

project(Calculator)

set(CMAKE_CXX_STANDARD 23)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp)

add_executable(calculator run.cpp ${sources} ${headers})
target_compile_options(calculator PUBLIC -g -Wall -Werror)
