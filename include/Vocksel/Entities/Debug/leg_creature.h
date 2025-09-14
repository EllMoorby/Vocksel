#ifndef LEGCREATURE_H
#define LEGCREATURE_H
#include "Vocksel/Entities/Creature/creature_segment.h"
#include "Vocksel/Graphics/camera.h"

namespace Vocksel::DebugUtils {
class LegCreature {
 public:
  LegCreature();

  void render(Camera camera, float aspect_ratio);
  void update(float delta_time);

  CreatureSegment segment;
  glm::vec3 position = glm::vec3(-10.f, 0.f, -10.f);
  glm::vec3 offset = glm::vec3(0.f);
};
}  // namespace Vocksel::DebugUtils

#endif  // LEGCREATURE_H
