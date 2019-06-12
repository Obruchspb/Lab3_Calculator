cmake_minimum_required(VERSION 2.8) 

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") 
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd") 

add_subdirectory(src/) 
add_subdirectory(test/) 
add_subdirectory(gtest/) 