#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : _position(position), _front(glm::vec3(0.0f, 0.0f, -1.0f)), _up(up), _worldUp(up),  _yaw(yaw), _pitch(pitch) {
    _right = glm::normalize(glm::cross(_front,_up));
    _lookAt = glm::lookAt(_position, _position + _front, _up);
    _devMode = false;
}   


glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::setPosition(const glm::vec3& position) {
    _position = position;
    _lookAt = glm::lookAt(_position, _position + _front, _up);
}

void Camera::printPosition(){ 
    std::cout << "Camera Position: " 
        << _position.x << ", " 
        << _position.y << ", " 
        << _position.z << std::endl; 
}
void Camera::handleCameraMovement(CameraMovement direction, float deltaTime) {
    float velocity = _cameraSpeed * deltaTime;
    switch (direction) {
        case FORWARD:
            _position += _front * velocity;
            break;
        case BACKWARD:
            _position -= _front * velocity;
            break;
        case LEFT:
            _position -= _right * velocity;
            break;
        case RIGHT:
            _position += _right * velocity;
            break;
        case UP: 
            _position += _up * velocity;
            break;
        case DOWN: 
            _position -= _up * velocity;
            break;
   
    }
    _lookAt = glm::lookAt(_position, _position + _front, _up);
    printPosition();
}   


void Camera::handleMouseMovement(float xOffset, float yOffset, GLboolean const constraintPitch) { 
    float sensitivity = GLOBAL::IS_CURSOR_DISABLED ? 0.1f : 0.5f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    _yaw += xOffset;
    _pitch += yOffset;


    if(constraintPitch) { 
        if(_pitch > 89.0f)
            _pitch = 89.0f;
        if(_pitch < -89.0f) 
            _pitch = -89.0f;
    }
    this->updateCameraVectors();

}


void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    _right = glm::normalize(glm::cross(_front, _worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    _up    = glm::normalize(glm::cross(_right, _front));
}



void Camera::setProjectionMatrix(glm::mat4 projection){ 

            projectionMatrix = projection;
        }
    
glm::mat4 Camera::getProjectionMatrix() const{ 
    return projectionMatrix;
}

void Camera::toggleDevCam(){ 
    _devMode = !_devMode;
}

OrbitalCamera::OrbitalCamera(glm::vec3 target , float distance, float yaw, float pitch) 
   : Camera(glm::vec3(20.0f)),  // Position the camera at a distance along x, y , z, 
          _target(target), _distance(distance)
    {
        initYawPitch();
    }

void OrbitalCamera::initYawPitch() { 
    glm::vec3 offset = _position - _target;
    _distance = glm::length(offset);
    _pitch = glm::degrees(asin(offset.y / _distance));
    _yaw = glm::degrees(atan2(offset.x, offset.z));

}



glm::mat4 OrbitalCamera::getViewMatrix() const {
    return glm::lookAt(_position, _target, _worldUp);
}


void OrbitalCamera::update(float deltaX, float deltaY) { 

}

void OrbitalCamera::updateCameraVectors() { 
    glm::vec3 direction; 
    std::cout<< "Yaw = " << _yaw << std::endl;
    std::cout<< "Pitch = " << _pitch << std::endl;
    direction.x = cos(glm::radians(_pitch)) * sin(glm::radians(_yaw));
    direction.y = sin(glm::radians(_pitch));
    direction.z = cos(glm::radians(_pitch)) * cos(glm::radians(_yaw));
    _position = _target + direction * _distance ;
}


void OrbitalCamera::handleMouseMovement(float xOffset, float yOffset, GLboolean const constraintPitch) { 
    float sensitivity = GLOBAL::IS_CURSOR_DISABLED ? 0.1f : 0.5f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    _yaw -= xOffset;
    _pitch -= yOffset;


    if(constraintPitch) { 
        if(_pitch > 89.0f)
            _pitch = 89.0f;
        if(_pitch < -89.0f) 
            _pitch = -89.0f;
    }
    updateCameraVectors();

}