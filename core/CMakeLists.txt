set(SRC_DIR src/)
set(INCLUDE_DIR include)
list(APPEND INCLUDE_DIR include/core)
list(APPEND INCLUDE_DIR include/core/Renderer)

file(GLOB_RECURSE CORE_SOURCES ${SRC_DIR}/*.cpp )
file(GLOB_RECURSE CORE_HEADERS ${INCLUDE_DIR}/*.h)


add_library(core STATIC )

target_sources(core PRIVATE ${CORE_SOURCES} ${CORE_HEADERS})
target_include_directories(core PUBLIC ${INCLUDE_DIR})


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES  ${CORE_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include FILES  ${CORE_HEADERS})


target_compile_definitions(core PUBLIC GLFW_INCLUDE_NONE)

target_link_libraries(core
        PRIVATE glfw glad
        PUBLIC glm
)


