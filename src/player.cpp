#include "Vocksel/player.h"

Vocksel::Player::Player(): world_() {
    updateVectors();
}

void Vocksel::Player::init(World &world) {
    world_ = &world;
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
        acceleration_ += move_dir * 500.f * deltaTime;
    }


}

void Vocksel::Player::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    right_ = -glm::normalize(glm::cross(front_, Constants::WORLD_UP));
    up_ = glm::normalize(glm::cross(right_, front_));

    flatfront_ = glm::normalize(glm::vec3(front_.x, .0f, front_.z));
}

void Vocksel::Player::applyPhysics(float deltaTime) {
    if (!is_grounded_) {
        acceleration_.y += Constants::GRAVITY;
    }
    velocity_ += acceleration_;
    velocity_ *= std::pow(0.95f, deltaTime * 120.f);

    if (glm::length(velocity_) > movement_speed_) {
        velocity_ = glm::normalize(velocity_) * movement_speed_;
    }
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
    float minX = x - size.x;
    float maxX = x + size.x;
    float minY = y;
    float maxY = y + size.y;
    float minZ = z - size.z;
    float maxZ = z + size.z;

    for (float bx = std::floor(minX); bx <= std::ceil(maxX); bx += 1.0f) {
        for (float by = std::floor(minY); by <= std::ceil(maxY); by += 1.0f) {
            for (float bz = std::floor(minZ); bz <= std::ceil(maxZ); bz += 1.0f) {
                if (world_->getBlockAtWorldPos(bx, by, bz) != 0) {
                    if (maxX > bx - 0.5f && minX < bx + 0.5f &&
                        maxY > by - 0.5f && minY < by + 0.5f &&
                        maxZ > bz - 0.5f && minZ < bz + 0.5f) {
                        return true;
                        }
                }
            }
        }
    }
    return false;
}

void Vocksel::Player::update(InputManager &input_manager, float delta_time) {
    handleInput(input_manager, delta_time);
    applyPhysics(delta_time);
    camera_.setPosition(position_ + camera_offset_);
}

Vocksel::Player::~Player() {

}





