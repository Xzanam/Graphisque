#ifndef CAMERA_H
#define CAMERA_H


#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <memory>


enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera { 
    private:
        glm::vec3 _position;
        glm::vec3 _front;
        glm::vec3 _up;
        glm::mat4 _lookAt;

        glm::vec3 _right;
        
 
        float _cameraSpeed = 2.5f;

        glm::mat4 projectionMatrix;

    public:


        Camera(glm::vec3 position= glm::vec3(0.0f, 0.0f, 3.0f), 
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) ;
            
        void setProjectionMatrix(glm::mat4 projection) ;

        glm::mat4 getViewMatrix() const ;
        glm::mat4 getProjectionMatrix() const;

        void handleCameraMovement(CameraMovement direction, float deltaTime);

        void setPosition(const glm::vec3& position) ;
        void printPosition();
  
};
#endif // CAMERA_H