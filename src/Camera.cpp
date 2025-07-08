#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up)
    : _position(position), _front(glm::vec3(0.0f, 0.0f, 0.0f)), _up(up) {
    _right = glm::normalize(glm::cross(_front, _up));
    _lookAt = glm::lookAt(_position, _position + _front, _up);
}   


glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::setPosition(const glm::vec3& position) {
    _position = position;
    _lookAt = glm::lookAt(_position, _position + _front, _up);
}

void Camera::printPosition( ){ 
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
    }
    _lookAt = glm::lookAt(_position, _position + _front, _up);
}   

