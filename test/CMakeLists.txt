cmake_minimum_required(VERSION 2.8.8) 

set(SOURSE 
test_main.cpp 
) 

add_executable(test_m ${SOURSE}) 

target_link_libraries(test_m PUBLIC gtest) 
target_link_libraries(test_m PUBLIC calculator) 