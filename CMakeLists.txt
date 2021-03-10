project(Algorithm)

add_executable(${PROJECT_NAME} source.cpp)
file(COPY "${CMAKE_CURRENT_SOURCE_TREE}MarkovAlgorithm.txt" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
