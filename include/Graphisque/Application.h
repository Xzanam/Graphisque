#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream> 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include "Shader.h"
#include "GLBuffer.h"
#include "GLVertexArray.h"
#include "Grid.h"

#define WinWidth 1200
#define WinHeight 720

extern float aspectRatio;



class Application { 
    private: 
        GLFWwindow* window; // Pointer to the GLFW window
        std::string title; // Title of the application
        int WIN_WIDTH; // Width of the window
        int WIN_HEIGHT; // Height of the window
        float lastX, lastY; 

        glm::mat4 projectionMatrix;
        float left, right, bottom, top; //For Projection matrix


        //HardCoded Shaders for now 
        //will remove this after implementing a shader manager class
        Shader *gridShader, *drawShader ;

        Grid graph;

        bool initGLFW();
        bool initShader();
        void setUpProjectionMatrix();

        void processInput();
        void setUpCallbacks();
   
    public: 
        Application(const std::string& title = "Default", int width = WinWidth, int height=WinHeight);
        static Application* getApplicationPtr(GLFWwindow* window);
        bool init(); 


        static void mouse_callback( GLFWwindow* window,double xPos, double yPos);
        static void frameBufferSize_callback(GLFWwindow* window ,int width, int height);  
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffse);

        void render();
        ~Application() ;



        
        void run() ;
};




#endif // APPLICATION_H