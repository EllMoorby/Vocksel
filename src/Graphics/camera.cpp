#include "Vocksel/Graphics/camera.h"


glm::mat4 Vocksel::Camera::getProjectionMatrix(float aspect_ratio) const {
    return glm::perspective(fov_, aspect_ratio, nearplane_, farplane_);
}

glm::mat4 Vocksel::Camera::getViewMatrix() const {
    return glm::lookAt(position_, position_+front_, up_);
}

void Vocksel::Camera::setPosition(glm::vec3 position) {
    position_ = position;
}

void Vocksel::Camera::setRotation(float yaw, float pitch) {
    yaw_ = yaw;
    pitch_ = pitch;

    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;
    recalculateVectors();
}

void Vocksel::Camera::recalculateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, glm::vec3(0.0f, 1.0f, 0.0f)));
    up_ = glm::normalize(glm::cross(right_, front_));
}






