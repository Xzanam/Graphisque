#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream> 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Shader.h"

#define WinWidth 800
#define WinHeight 600



class Application { 
    private: 
        GLFWwindow* window; // Pointer to the GLFW window
        std::string title; // Title of the application
        int WIN_WIDTH; // Width of the window
        int WIN_HEIGHT; // Height of the window
        float lastX, lastY; 

        std::shared_ptr<Shader> _mainShader;
    
    public: 
        Application(const std::string& title = "Default", int width = WinWidth, int height=WinHeight);
        bool init(); 
        bool initGLFW();
        bool initShader();
        void processInput();


        void render();
        ~Application() ;



        
        void run() ;
};




#endif // APPLICATION_H