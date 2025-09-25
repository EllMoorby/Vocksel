#define GLM_ENABLE_EXPERIMENTAL
#include "Vocksel/Entities/Creature/creature.h"

#include "Vocksel/Core/engine_services.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"
#if DEBUG
#include "tracy/Tracy.hpp"
#endif

Vocksel::Creature::Creature(glm::vec3 position) : position_(position) {
  head_segment_ = std::make_unique<CreatureSegment>(.1f, 0.f, position_);

  addSegment(std::make_unique<CreatureSegment>(
      .1f, 2.f, glm::vec3(position.x - 2.f, position.y, position.z)));
  // addSegment(std::make_unique<CreatureSegment>(.1f,2.f, glm::vec3(position.x
  // - 4.f , position.y, position.z)));
  // addSegment(std::make_unique<CreatureSegment>(1.f,2.f, glm::vec3(position.x
  // - 6.f , position.y, position.z)));
  // addSegment(std::make_unique<CreatureSegment>(1.f,2.f, glm::vec3(position.x
  // - 8.f , position.y, position.z)));
  // addSegment(std::make_unique<CreatureSegment>(1.f,2.f, glm::vec3(position.x
  // - 10.f , position.y, position.z)));
  // addSegment(std::make_unique<CreatureSegment>(1.f,2.f, glm::vec3(position.x
  // - 12.f , position.y, position.z)));

  for (auto& leg : head_segment_->getLegs()) {
    leg.setVelocityAll(glm::vec3(0.f, 5.f, 0.f));
  }
  for (auto& segment : body_segments_) {
    for (auto& leg : segment->getLegs()) {
      leg.setVelocityAll(glm::vec3(0.f, 5.f, 0.f));
    }
  }
}

void Vocksel::Creature::setPosition(glm::vec3 position) {
  position_ = position;
  updateHeadPosition();
  updateSegmentPositions();
}

void Vocksel::Creature::addSegment(
    std::unique_ptr<CreatureSegment> creature_segment) {
  creature_segment->setDirection(front_);
  body_segments_.push_back(std::move(creature_segment));
}

void Vocksel::Creature::updateHeadPosition() {
  head_segment_->setPosition(position_);
}

void Vocksel::Creature::setDesiredMovement(glm::vec3 dir) {
  if (glm::length(dir) > 0) {
    desired_movement_ = glm::normalize(dir) * max_speed_;
  } else {
    desired_movement_ = glm::vec3(0.0f);
  }
  desired_movement_.x = -desired_movement_.x;
}

void Vocksel::Creature::update(float delta_time) {
#if DEBUG
  ZoneScopedN("Creature Update");
#endif
  setDesiredMovement(EngineServices::input().getArrowVector());

  movement_ =
      glm::mix(movement_, desired_movement_, acceleration_ * delta_time);

  if (glm::length(movement_) > 0.1f) {
    front_ = glm::normalize(movement_);
  }

  position_ += movement_ * delta_time;
  updateHeadPosition();
  updateSegmentPositions();

  head_segment_->update(delta_time);
  for (auto& segment : body_segments_) {
    segment->update(delta_time);
  }
}

void Vocksel::Creature::updateSegmentPositions() {
  auto* target_segment = head_segment_.get();
  glm::vec3 smoothed_front =
      glm::normalize(glm::mix(head_segment_->getDirection(), front_, 0.2f));
  head_segment_->setDirection(smoothed_front);
  head_segment_->setAngleToAhead(0.f);

  for (auto& current_segment : body_segments_) {
    const float gap_size = current_segment->getGap();
    const glm::vec3 target_pos = target_segment->getPosition();
    glm::vec3 current_pos = current_segment->getPosition();

    glm::vec3 desired_dir = glm::normalize(current_pos - target_pos);
    if (glm::length(desired_dir) == 0.0f) {
      desired_dir = -target_segment->getDirection();
    }

    glm::vec3 current_dir = -desired_dir;
    glm::vec3 target_dir = target_segment->getDirection();

    float dot = glm::clamp(glm::dot(target_dir, current_dir), -1.0f, 1.0f);
    float angle_between = glm::degrees(glm::acos(dot));

    current_segment->setAngleToAhead(angle_between);

    if (angle_between > 0.01f) {
      float max_angle_deg = 100.0f;
      float max_angle_rad = glm::radians(max_angle_deg);

      // Smooth the rotation with slerp (idk)
      float t = glm::min(1.0f, max_angle_rad / glm::radians(angle_between));
      glm::quat rotation = glm::rotation(target_dir, current_dir);
      glm::quat limited_rot = glm::slerp(glm::quat(), rotation, t);
      current_dir = glm::normalize(limited_rot * target_dir);
    }

    current_segment->setDirection(current_dir);

    const glm::vec3 corrected_pos = target_pos + (-current_dir) * gap_size;

    glm::vec3 smoothed_pos = glm::mix(current_pos, corrected_pos, 0.25f);
    current_segment->setPosition(smoothed_pos);

    target_segment = current_segment.get();
  }
}

void Vocksel::Creature::render(Shader& shader) {
  head_segment_->render(shader);
  for (auto& segment : body_segments_) {
    segment->render(shader);
  }
}

void Vocksel::Creature::cleanUp() {
  head_segment_.reset();
  body_segments_.clear();
}

Vocksel::Creature::~Creature() { cleanUp(); }
