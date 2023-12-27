cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

project(TurkishC)

# Find JsonCpp library
find_library(JSONCPP_LIBRARY jsoncpp)

# Check if the library is found
if(NOT JSONCPP_LIBRARY)
  message(FATAL_ERROR "JsonCpp library not found!")
endif()

file(GLOB SOURCES "*.cpp")
add_executable(transpile ${SOURCES})
target_link_libraries(transpile PRIVATE ${JSONCPP_LIBRARY})
