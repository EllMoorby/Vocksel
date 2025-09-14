#include "Vocksel/Entities/Debug/leg_creature.h"

#include "Vocksel/Core/engine_services.h"

Vocksel::DebugUtils::LegCreature::LegCreature()
    : segment(CreatureSegment(.1f, 0.f, position)) {
  segment.addLeg(glm::vec3(1.5f, -1.5f, 1.f));
  segment.getLegs().back().addLegSegment(1.f);
  segment.getLegs().back().addLegSegment(1.f);
  segment.getLegs().back().addLegSegment(1.f);
  segment.getLegs().back().addLegSegment(1.f);
  segment.getLegs().back().addLegSegment(1.f);
  segment.setDirection(glm::vec3(0.0f, 0.f, 1.0f));
}

void Vocksel::DebugUtils::LegCreature::render(Camera camera,
                                              float aspect_ratio) {
  segment.render(EngineServices::resources().get<Shader>("default"));
  glm::vec3 currentBase = segment.getPosition();

  for (auto& leg_seg : segment.getLegs().back().segments_) {
    EngineServices::debug().drawLine(currentBase, leg_seg.getTipPosition(),
                                     glm::vec3(0, 1, 0),
                                     camera);  // Different color for body legs
    currentBase = leg_seg.getTipPosition();
  }
}

void Vocksel::DebugUtils::LegCreature::update(float delta_time) {
  segment.setPosition(position);
  segment.getLegs().back().setOffset(offset);
  segment.getLegs().back().setTarget(
      segment.getLegs().back().getRestPosition());
  segment.update(delta_time);
}
