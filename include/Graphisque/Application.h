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

#include <chrono>

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
        float targetLeft, targetRight, targetBottom, targetTop;
        bool isPanning;
        float panSmoothness = 8.0f;

        bool showFPS = false;

        //HardCoded Shaders for now 
        //will remove this after implementing a shader manager class
        Shader *gridShader, *drawShader ;

        Grid* grid;

        bool initGLFW();
        bool initShader();
        void updateProjectionMatrix();

        void processInput(float deltaTime);
        void setUpCallbacks();
        void panGrid(float deltaX, float deltaY);

        float lerpPan(float start, float end, float t);
        float expDecayLerp(float, float, float , float);
   
    public: 
        Application(const std::string& title = "Default", int width = WinWidth, int height=WinHeight);
        static Application* getApplicationPtr(GLFWwindow* window);
        bool init(); 


        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void cursor_position_callback( GLFWwindow* window,double xPos, double yPos);
        static void frameBufferSize_callback(GLFWwindow* window ,int width, int height);  
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffse);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


        void render();
        ~Application() ;



        
        void run() ;
};




#endif // APPLICATION_H