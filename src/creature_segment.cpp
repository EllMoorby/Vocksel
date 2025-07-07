#include "Vocksel/creature_segment.h"

Vocksel::CreatureSegment::CreatureSegment(ModelManager &model_manager, ResourceManager &resource_manager, float radius, glm::vec3 position)
    : radius_(radius), position_(position), model_manager_(model_manager), resource_manager_(resource_manager),
        sphere_(Sphere::create(model_manager, resource_manager, position, radius, "stone")) {

}

void Vocksel::CreatureSegment::update(float delta_time) {

}


void Vocksel::CreatureSegment::setPosition(glm::vec3 position) {
    position_ = position;
    sphere_->setPosition(position);
}

void Vocksel::CreatureSegment::render(Shader &shader) {
    sphere_->render(shader);
}

void Vocksel::CreatureSegment::cleanUp() {
    sphere_->cleanUp();
}


Vocksel::CreatureSegment::~CreatureSegment() {
    cleanUp();
}


