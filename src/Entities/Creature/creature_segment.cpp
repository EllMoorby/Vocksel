#include "Vocksel/Entities/Creature/creature_segment.h"

#include "Vocksel/Graphics/Mesh/model_instance.h"

Vocksel::CreatureSegment::CreatureSegment(float radius, float gap,
                                          glm::vec3 position)
    : radius_(radius),
      gap_(gap),
      position_(position),
      body_mesh_(ModelInstance::create(position, "sphere", "stone",
                                       glm::vec3(radius))) {
  // Add Legs
  addLeg(glm::vec3(1.5f, -1.5f, 1.f));
  addLeg(glm::vec3(-1.5f, -1.5f, 1.f));

  for (auto& leg : legs_) {
    leg.addLegSegment(.5f);
    leg.addLegSegment(.5f);
    leg.addLegSegment(.5f);

    leg.addLegSegment(.5f);
    leg.addLegSegment(.75f);
    leg.addLegSegment(1.f);
  }
}

void Vocksel::CreatureSegment::addLeg(glm::vec3 offset) {
  legs_.emplace_back(position_, direction_, offset);
}

void Vocksel::CreatureSegment::update(float delta_time) {
  for (auto& leg : legs_) {
    leg.update(delta_time, position_, direction_);
  }
}

void Vocksel::CreatureSegment::setPosition(glm::vec3 position) {
  position_ = position;
  body_mesh_->setPosition(position);
}

void Vocksel::CreatureSegment::render(Shader& shader) {
  body_mesh_->render(shader);
  for (auto& leg : legs_) {
    leg.render(shader);
  }
}

Vocksel::CreatureSegment::~CreatureSegment() = default;
