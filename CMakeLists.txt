cmake_minimum_required(VERSION 3.9)
project(assignment1)

set(CMAKE_CXX_STANDARD 14)

include_directories("include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

add_library(common STATIC
        src/vector_space_model.cpp
        src/porter_stemmer.cpp
        src/util.cpp
        src/tokenizer.cpp
        src/file_manager.cpp
        src/parser.cpp)

add_executable(lexrank src/main_lexrank.cpp src/lexrank.cpp)
add_executable(idf src/main_idf.cpp)

target_link_libraries(lexrank common)
target_link_libraries(idf common)

set_target_properties(lexrank PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)
set_target_properties(idf PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)
