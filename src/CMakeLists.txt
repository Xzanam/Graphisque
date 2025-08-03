#add executable

include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}/Graphisque)
include_directories(${INCLUDE_DIR}/imgui)



#source files 
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp ${SRC_DIR}/*.c)

# If the ImGui backend .cpp files are under src/imgui/
# (adjust the paths below if needed)
list(APPEND SRC_FILES
        ${SRC_DIR}/imgui/imgui_impl_glfw.cpp
        ${SRC_DIR}/imgui/imgui_impl_opengl3.cpp
)


add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)


add_executable(${PROJECT_NAME} ${SRC_FILES}
        ../include/Graphisque/Equations.h)

add_custom_command(TARGET Graphisque POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:Graphisque>/shaders
)



target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)





