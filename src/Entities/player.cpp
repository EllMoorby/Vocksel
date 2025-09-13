#include "Vocksel/Entities/player.h"

#include "tracy/Tracy.hpp"
#include "Vocksel/Core/engine_services.h"

Vocksel::Player::Player(): world_() {
    updateVectors();
}

void Vocksel::Player::init(World &world) {
    world_ = &world;
    initDebug();
}

void Vocksel::Player::initDebug() {
    EngineServices::debugGUI().addPanel("Player", [this]() {
        ImGui::Text("Position: (%.2f, %.2f, %.2f)", position_.x, position_.y, position_.z);
        ImGui::Text("Velocity: (%.2f, %.2f, %.2f)", velocity_.x, velocity_.y, velocity_.z);
        ImGui::Text("Acceleration: (%.2f, %.2f, %.2f)", acceleration_.x, acceleration_.y, acceleration_.z);
        ImGui::Separator();
        ImGui::Text("Pitch: %.2f || Yaw: %.2f", pitch_, yaw_);
        ImGui::Text("Camera Offset: (%.2f, %.2f, %.2f)", camera_offset_.x, camera_offset_.y, camera_offset_.z);
        ImGui::Text("Size: (%.2f, %.2f, %.2f)", size_.x, size_.y, size_.z);
    });
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

void Vocksel::Player::handleInput(float deltaTime) {
    auto input = EngineServices::input();
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
        acceleration_ += move_dir * 500.f;
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
    if (!is_grounded_) {
        acceleration_.y += Constants::GRAVITY;
    }
    velocity_ += acceleration_ * deltaTime * 100.f;
    velocity_ *= std::pow(0.95f, deltaTime * 120.f);

    glm::vec3 horizontal_vel = glm::vec3(velocity_.x, 0.0f, velocity_.z);
    glm::vec3 vertical_vel = glm::vec3(0.0f, velocity_.y, 0.0f);

    if (glm::length(horizontal_vel) > movement_speed_) {
        horizontal_vel = glm::normalize(horizontal_vel) * movement_speed_;
    }

    if (glm::length(vertical_vel) > movement_speed_) {
        vertical_vel = glm::normalize(vertical_vel) * movement_speed_;
    }
    velocity_ = horizontal_vel + vertical_vel;
    glm::vec3 new_pos = position_ + velocity_ * deltaTime;



    if (checkCollision(new_pos.x, position_.y, position_.z, size_)) {
        velocity_.x = 0;
    } else {
        position_.x = new_pos.x;
    }

    if (checkCollision(position_.x, new_pos.y, position_.z, size_)) {
        velocity_.y = 0;
        is_grounded_ = (new_pos.y < position_.y);
    } else {
        position_.y = new_pos.y;
        is_grounded_ = false;
    }

    if (checkCollision(position_.x, position_.y, new_pos.z, size_)) {
        velocity_.z = 0;
    } else {
        position_.z = new_pos.z;
    }
    acceleration_ = glm::vec3(0.f);
}

bool Vocksel::Player::checkCollision(float x, float y, float z, const glm::vec3& size) {
    return false;
}

void Vocksel::Player::update(float delta_time) {
#if DEBUG
    ZoneScopedN("Player Update");
#endif
    handleInput(delta_time);
    applyPhysics(delta_time);
    camera_.setPosition(position_ + camera_offset_);
}

Vocksel::Player::~Player() {

}





