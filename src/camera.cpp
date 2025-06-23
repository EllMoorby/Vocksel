#include "Vocksel/camera.h"

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
