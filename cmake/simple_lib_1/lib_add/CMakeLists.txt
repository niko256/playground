add_library(lib_add STATIC add.cc)

# where should we search for header files
target_include_directories(lib_add PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


