#include "Vocksel/Entities/Creature/leg_segment.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include "Vocksel/Core/engine_services.h"

Vocksel::LegSegment::LegSegment(float length, glm::vec3 tip_position): length_(length), tip_position_(tip_position), tip_mesh_(ModelInstance::create(tip_position, "sphere", "default", glm::vec3(0.1f, 0.1f, 0.1f))){

}

void Vocksel::LegSegment::render() {
    tip_mesh_->setPosition(tip_position_);
    tip_mesh_->render(EngineServices::resources().getShader("default"));
}





Vocksel::LegSegment::~LegSegment() {

}


