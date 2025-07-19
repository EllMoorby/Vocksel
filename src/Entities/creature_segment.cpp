#include "Vocksel/Entities/creature_segment.h"

#include "Vocksel/Graphics/Mesh/mesh_object.h"

Vocksel::CreatureSegment::CreatureSegment(float radius, glm::vec3 position)
    : radius_(radius), position_(position), body_mesh_(MeshObject::create(position, "teapot" , "stone", glm::vec3(radius))) {

}

void Vocksel::CreatureSegment::update(float delta_time) {
}


void Vocksel::CreatureSegment::setPosition(glm::vec3 position) {
    position_ = position;
    body_mesh_->setPosition(position);
}

void Vocksel::CreatureSegment::render(Shader &shader) {
    body_mesh_->render(shader);
}


Vocksel::CreatureSegment::~CreatureSegment() {
}


