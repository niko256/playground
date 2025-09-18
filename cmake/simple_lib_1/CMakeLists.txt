cmake_minimum_required(VERSION 3.30)

project(MyMathLib LANGUAGES CXX)

add_subdirectory(lib_mul)
add_subdirectory(lib_add)
add_subdirectory(lib_constants)

add_executable(MyMathExec main.cc)

target_link_libraries(MyMathExec PRIVATE 
    lib_add
    lib_multiply
)

