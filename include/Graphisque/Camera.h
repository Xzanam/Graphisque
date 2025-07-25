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
    protected:
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

        virtual void updateCameraVectors() ;

    public:


        Camera(glm::vec3 position= glm::vec3(0.0f, 0.0f, 3.0f), 
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
               float yaw = GLOBAL::YAW, 
               float pitch = GLOBAL::PITCH) ;
            
        void setProjectionMatrix(glm::mat4 projection) ;

        virtual glm::mat4 getViewMatrix() const ;
        glm::mat4 getProjectionMatrix() const;

        void handleCameraMovement(CameraMovement direction, float deltaTime);
        virtual void handleMouseMovement(float xOffset, float yOffset, GLboolean const constraintPitch);

        void setPosition(const glm::vec3& position) ;
        void printPosition();
        void toggleDevCam();


        virtual ~Camera() = default;
  
};


class OrbitalCamera  : public Camera { 
    private:
        glm::mat4 _lookAt;
        glm::mat4 _viewMatrix;

    protected: 
        void updateCameraVectors() override;




    public:
    OrbitalCamera(glm::vec3 target = glm::vec3(0.0f), float distance = 30.0f, float yaw = 0.0f, float pitch = 0.0f)
        : Camera(glm::vec3(30.0f, 60.0f,distance)),  // Position the camera at a distance along Z
          _target(target), _distance(distance), _yaw(yaw), _pitch(pitch)
    {}
    glm::vec3 _target= glm::vec3(0.0f);
    float _distance = 30.0f;
    float _yaw = 0.0f;
    float _pitch = 0.0f;
    glm::mat4 getViewMatrix() const override;

    void handleMouseMovement(float xOffset, float yOffset, GLboolean const constraintPitch) override;
    void update(float deltaX, float deltaY);

};
#endif // CAMERA_H