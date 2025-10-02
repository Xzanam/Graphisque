#
# app
#


#set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
#set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
#list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/Graphisque ${CMAKE_SOURCE_DIR}/include/imgui)

#set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/shaders)
#set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#if (POLICY CMP0072)
#    cmake_policy(SET CMP0072 NEW)
##endif()
#
#find_package(OpenGL REQUIRED)
#

set(SOURCES src/main.cpp)

add_executable(app)
target_sources(app PRIVATE ${SOURCES})

#source_group()


target_compile_definitions(app PRIVATE GLFW_INCLUDE_NONE)
target_include_directories(app PRIVATE include)

target_link_libraries(app PRIVATE core)







