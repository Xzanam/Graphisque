#add executable

include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}/Graphisque)



#source files 
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp ${SRC_DIR}/*.c)




add_executable(${PROJECT_NAME} ${SRC_FILES} )

add_custom_command(TARGET Graphisque POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:Graphisque>/shaders
)



target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)





