cmake_minimum_required(VERSION 3.16)

project(Algorithm)

add_executable(${PROJECT_NAME} source.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_TREE}MarkovAlgorithms.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
