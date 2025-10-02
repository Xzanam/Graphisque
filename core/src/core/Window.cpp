//
// Created by Janam Shrestha on 10/2/25.
//

#include "core/Window.h"


#include <glad/gl.h>


namespace Core {
    Window::Window(const WindowSpecification& specification) :
    m_WindowSpec(specification)
    {
    }

    Window::~Window() {
        Destroy();
    }

    void Window::Create() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GL_TRUE);
    }


}
