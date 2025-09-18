add_library(lib_multiply STATIC multiply.cc)

target_include_directories(lib_multiply PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lib_multiply PUBLIC lib_constants)
