#ifndef LEG_CHAIN_H
#define LEG_CHAIN_H
#include <vector>

#include "Vocksel/Entities/Creature/leg_segment.h"
#include "glm/vec3.hpp"

namespace Vocksel {

class LegChain {
 public:
  LegChain(glm::vec3 root, glm::vec3 forward, glm::vec3 offset);
  ~LegChain();

  LegChain(const LegChain&) = delete;
  LegChain& operator=(const LegChain&) = delete;

  // Explicitly define move operations
  LegChain(LegChain&&) noexcept = default;
  LegChain& operator=(LegChain&&) noexcept = default;

  void update(float delta_time, glm::vec3 root, glm::vec3 forward);
  void render(Shader& shader);
  void updateVectors();
  void addLegSegment(float length, glm::vec3 velocities = glm::vec3(0.0f));

  void setTarget(glm::vec3 target) { target_ = target; }
  void setOffset(glm::vec3 offset) { offset_ = offset; }
  void setVelocity(glm::vec3 velocity, uint32_t idx);
  void setVelocityAll(glm::vec3 velocity);

  glm::vec3 getTarget() const { return target_; }
  glm::vec3 getOffset() const { return offset_; }
  glm::vec3 getRestPosition() const { return rest_position_; }

  std::vector<LegSegment> segments_;
  std::vector<glm::vec3> velocities_;

  float reset_distance_ = 2.5f;

 private:
  void updateLegShape(float delta_time);

  float tolerance_ = 0.01f;

  std::unique_ptr<ModelInstance> root_mesh_;
  std::unique_ptr<ModelInstance> rest_mesh_;

  glm::vec3 root_ = glm::vec3(0.0f);
  glm::vec3 last_root_ = glm::vec3(0.0f);

  glm::vec3 forward_ = glm::vec3(0.0f);
  glm::vec3 right_ = glm::vec3(0.f);
  glm::vec3 offset_ = glm::vec3(0.f);

  glm::vec3 target_ = glm::vec3(0.0f);
  glm::vec3 last_target_ = glm::vec3(0);
  glm::vec3 rest_position_ = glm::vec3(0.0f);
};
}  // namespace Vocksel

#endif  // LEG_CHAIN_H
