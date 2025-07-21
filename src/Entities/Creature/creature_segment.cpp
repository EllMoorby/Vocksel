#include "Vocksel/Entities/Creature/creature_segment.h"

#include "Vocksel/Graphics/Mesh/mesh_object.h"

Vocksel::CreatureSegment::CreatureSegment(float radius, float gap, glm::vec3 position)
    :  radius_(radius), position_(position), gap_(gap),
        body_mesh_(MeshObject::create(position, "sphere", "stone", glm::vec3(radius))),
        leg_(LegChain(position, direction_)){
    leg_.addLeg(1.f);
    leg_.addLeg(1.f);
    leg_.addLeg(1.f);
}

void Vocksel::CreatureSegment::update(float delta_time) {
    leg_.update(position_,direction_);
}

void Vocksel::CreatureSegment::setPosition(glm::vec3 position) {
    position_ = position;
    body_mesh_->setPosition(position);
}

void Vocksel::CreatureSegment::render(Shader &shader) {
    body_mesh_->render(shader);
    leg_.render();
}


Vocksel::CreatureSegment::~CreatureSegment() {
}


