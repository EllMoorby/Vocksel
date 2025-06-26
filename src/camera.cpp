#include "Vocksel/camera.h"

void Vocksel::Camera::recalculateVectors() {
    right_ = glm::normalize(glm::cross(front_, Constants::WORLD_UP));
    up_ = glm::normalize(glm::cross(right_, front_));
}


glm::mat4 Vocksel::Camera::getProjectionMatrix(float aspect_ratio) const {
    return glm::perspective(fov_, aspect_ratio, nearplane_, farplane_);
}

glm::mat4 Vocksel::Camera::getViewMatrix() const {
    return glm::lookAt(position_, position_+front_, up_);
}



void Vocksel::Camera::moveForward(float speed) {
    position_ += speed * front_;
}

void Vocksel::Camera::moveBackward(float speed) {
    position_ -= speed * front_;
}

void Vocksel::Camera::moveRight(float speed) {
    position_ += speed * right_;
}

void Vocksel::Camera::moveLeft(float speed) {
    position_ -= speed * right_;
}

void Vocksel::Camera::moveUp(float speed) {
    position_ += speed * up_;
}

void Vocksel::Camera::moveDown(float speed) {
    position_ -= speed * up_;
}

