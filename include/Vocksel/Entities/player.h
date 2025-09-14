#ifndef PLAYER_H
#define PLAYER_H
#include "Vocksel/Core/input_manager.h"
#include "Vocksel/World/world.h"
#include "glm/vec3.hpp"

namespace Vocksel {
class Player {
 public:
  Player();
  ~Player();

  void init(World& world);
  void initDebug();
  void update(float delta_time);
  void handleInput(float deltaTime);
  void handleMouseInput(float xoffset, float yoffset);
  void applyPhysics(float deltaTime);

  const glm::vec3& getPosition() const { return position_; }
  const glm::vec3& getVelocity() const { return velocity_; }
  const glm::vec3& getFront() const { return front_; }
  const glm::vec3& getUp() const { return up_; }
  const glm::vec3& getRight() const { return right_; }
  float getYaw() const { return yaw_; }
  float getPitch() const { return pitch_; }
  const bool getIsGrounded() const { return is_grounded_; }

  void setPosition(const glm::vec3& position) { position_ = position; }
  void setRotation(float yaw, float pitch);
  Camera& getCamera() { return camera_; }

 public:
 private:
  void updateVectors();
  bool checkCollision(float x, float y, float z, const glm::vec3& size);

  Camera camera_;
  World* world_;
  glm::vec3 position_ = glm::vec3(0.f, 10.f, -10.f);
  glm::vec3 velocity_ = glm::vec3(0.f, 0.f, 0.f);
  glm::vec3 acceleration_ = glm::vec3(0.f, 0.f, 0.f);
  glm::vec3 size_ = glm::vec3(0.2f, 4.5f, 0.2f);
  glm::vec3 camera_offset_ = size_ - glm::vec3(0.f, 0.5f, 0.f);

  glm::vec3 front_ = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
  glm::vec3 up_ = Constants::WORLD_UP;
  glm::vec3 right_ = glm::normalize(glm::cross(front_, up_));
  glm::vec3 flatfront_ = glm::normalize(glm::vec3(front_.x, 0.0f, front_.z));

  float yaw_ = 90.0f;
  float pitch_ = 0.0f;

  const float movement_speed_ = Constants::MOVEMENT_SPEED;
  bool is_grounded_ = false;
};
}  // namespace Vocksel

#endif  // PLAYER_H
