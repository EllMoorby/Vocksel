#include "Vocksel/player.h"

Vocksel::Player::Player(): camera_() {
    updateVectors();
}

void Vocksel::Player::handleMouseInput(float xoffset, float yoffset) {
    yaw_ += xoffset * Constants::CAMERA_SENS;
    pitch_ += yoffset * Constants::CAMERA_SENS;

    // Clamp pitch
    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;

    updateVectors();
    camera_.setRotation(yaw_, pitch_);
}


void Vocksel::Player::setRotation(float yaw, float pitch) {
    yaw_ = yaw;
    pitch_ = pitch;

    // Constrain pitch to avoid gimbal lock
    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;

    updateVectors();
}

void Vocksel::Player::handleInput(InputManager &input, float deltaTime) {
    glm::vec3 movement = input.getWASDVector();
    glm::vec3 move_dir = flatfront_ * movement.z + right_ * movement.x;

    if (input.isKeyPressed(GLFW_KEY_SPACE)) {
        move_dir.y = 1.f;
    }

    if (input.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
        move_dir.y = -1.f;
    }

    if (glm::length(move_dir) > 0.0f) {
        move_dir = glm::normalize(move_dir);
        acceleration_ += move_dir * movement_speed_;
    }


}

void Vocksel::Player::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, Constants::WORLD_UP));
    up_ = glm::normalize(glm::cross(right_, front_));

    flatfront_ = glm::normalize(glm::vec3(front_.x, .0f, front_.z));
}

void Vocksel::Player::applyPhysics(float deltaTime) {
    //acceleration_.y += gravity_;
    velocity_ += acceleration_ * deltaTime;

    velocity_ *= .95f;

    position_ += velocity_ * deltaTime;

    acceleration_ = glm::vec3(0.f);

    // TODO: Ground detection
    if (position_.y <= 0.0f) {
        position_.y = 0.0f;
        velocity_.y = 0.0f;
        is_grounded_ = true;
    }
}

void Vocksel::Player::update(InputManager &input_manager, float delta_time) {
    handleInput(input_manager, delta_time);
    applyPhysics(delta_time);
    camera_.setPosition(position_);
}

Vocksel::Player::~Player() {

}





