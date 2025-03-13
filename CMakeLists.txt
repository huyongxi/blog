cmake_minimum_required(VERSION 3.0)

project(blog)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_DEMO ON)

include_directories(BEFORE third_party)

aux_source_directory(src SRC_LIST)

add_library(blog SHARED ${SRC_LIST})
target_include_directories(blog 
    PUBLIC include
)

if (BUILD_DEMO)
    add_subdirectory(demo)
endif()