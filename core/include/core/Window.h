//
// Created by Janam Shrestha on 10/2/25.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

namespace Core {
    struct WindowSpecification {
        std::string Title;
        uint32_t Width = 1280;
        uint32_t Height = 720;
        bool IsResizable = true;
        bool VSync = true;
    };
    class Window {
    public :
        Window(const WindowSpecification& spec = WindowSpecification());
        ~Window();

        void Create();
        void Destroy();
        void Update();
        glm::vec2  GetFramebufferSize();
        bool ShouldClose() const;

        GLFWwindow* GetHandle() const {return m_Handle;}

    private:
        WindowSpecification m_WindowSpec;
        GLFWwindow* m_Handle;
    };
}

#endif //WINDOW_H
