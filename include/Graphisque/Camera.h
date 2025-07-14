#ifndef CAMERA_H
#define CAMERA_H


#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Globals.h"
#include <memory>



enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT, 
    UP, 
    DOWN
};

// Default camera values

class Camera { 
    private:
        glm::vec3 _position;
        glm::vec3 _front;
        glm::vec3 _up;
        glm::mat4 _lookAt;


        glm::vec3 _right;
        glm::vec3 _worldUp;

        float _yaw, _pitch; 
        float _movementSpeed, _mouseSensitivity;


        
        bool _devMode;
 
        float _cameraSpeed = 2.5f;

        glm::mat4 projectionMatrix;

        void updateCameraVectors() ;

    public:


        Camera(glm::vec3 position= glm::vec3(0.0f, 0.0f, 3.0f), 
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
               float yaw = GLOBAL::YAW, 
               float pitch = GLOBAL::PITCH) ;
            
        void setProjectionMatrix(glm::mat4 projection) ;

        glm::mat4 getViewMatrix() const ;
        glm::mat4 getProjectionMatrix() const;

        void handleCameraMovement(CameraMovement direction, float deltaTime);
        void handleMouseMovement(float xOffset, float yOffset, GLboolean const constraintPitch);

        void setPosition(const glm::vec3& position) ;
        void printPosition();
        void toggleDevCam();
  
};
#endif // CAMERA_H