#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream> 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "Globals.h"
#include "GLBuffer.h"
#include "GLVertexArray.h"
#include "Shader.h"
#include "Camera.h"

#include "Grid3D.h"




class Application { 
    private: 
        GLFWwindow* window; // Pointer to the GLFW window
        std::string title; // Title of the application
        int WIN_WIDTH; // Width of the window
        int WIN_HEIGHT; // Height of the window
        float lastX, lastY; 
        glm::mat4 projection;


        float deltaTime;
        float lastFrame;


        Application* getApplicationPtr(); // Static method to get the application pointer from GLFW window user pointer
        std::shared_ptr<Shader> _mainShader;
        std::shared_ptr<Camera> devCamera;

        std::shared_ptr<Grid3D> grid3D;


    
    public: 
        Application(const std::string& title = "Default", int width = G_WinWidth, int height=G_WinHeight);
        bool init(); 
        bool initGLFW();
        bool initShader();
        void initGrid3D();
        void processInput(float deltaTime);

        void render();
        void run() ;
        

        //update projection matrix
        void updateProjectionMatrix();
        void updateViewMatrix() ;
        void processCameraMovement(float deltaTime);
        



        //callback functions 
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height) ;

        ~Application() ;
};




#endif // APPLICATION_H