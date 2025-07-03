#ifndef PLAYER_H
#define PLAYER_H
#include "input_manager.h"
#include "glm/vec3.hpp"
#include "glm/ext/quaternion_geometric.hpp"

namespace Vocksel {
    class Player {
        public:
        Player();
        ~Player();

        void update(InputManager& input_manager, float delta_time);
        void handleInput(InputManager& input, float deltaTime);
        void handleMouseInput(float xoffset, float yoffset);
        void applyPhysics(float deltaTime);

        const glm::vec3& getPosition() const { return position_; }
        const glm::vec3& getVelocity() const { return velocity_; }
        const glm::vec3& getFront() const { return front_; }
        const glm::vec3& getUp() const { return up_; }
        const glm::vec3& getRight() const { return right_; }
        float getYaw() const { return yaw_; }
        float getPitch() const { return pitch_; }

        void setPosition(const glm::vec3& position) { position_ = position; }
        void setRotation(float yaw, float pitch);
        void setMovementSpeed(float speed) { movement_speed_ = speed; }
        Camera& getCamera() { return camera_; }

        private:
        void updateVectors();
        private:
        Camera camera_;
        glm::vec3 position_ = glm::vec3(0.f, 10.f, -10.f);
        glm::vec3 velocity_ = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 acceleration_ = glm::vec3(0.f, 0.f, 0.f);

        glm::vec3 front_ = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 up_ = Constants::WORLD_UP;
        glm::vec3 right_ = glm::normalize(glm::cross(front_, up_));
        glm::vec3 flatfront_ = glm::normalize(glm::vec3(front_.x, 0.0f, front_.z));

        float yaw_ = 90.0f;
        float pitch_ = 0.0f;

        float movement_speed_ = Constants::MOVEMENT_SPEED;
        bool is_grounded_ = false;


    };
}

#endif //PLAYER_H
