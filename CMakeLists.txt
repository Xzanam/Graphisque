cmake_minimum_required(VERSION 3.10)
project(Graphisque)


#set C++ Standard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/shaders)
set( ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

find_package(OpenGL REQUIRED)



#add subdirectories
add_subdirectory(src)


