cmake_minimum_required(VERSION 2.8) 

project(Calculator) 


set(SOURSE 
src/main.cpp 
) 

set(HEDERS
src/Calculator.hpp 
) 


add_library(calculator STATIC ${SOURSE} ${HEDERS} )