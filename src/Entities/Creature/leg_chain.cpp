#include "Vocksel/Entities/Creature/leg_chain.h"

#include "Vocksel/Core/engine_services.h"

Vocksel::LegChain::LegChain(glm::vec3 root, glm::vec3 forward): root_(root), forward_(forward),
    root_mesh_(MeshObject::create(root,"cube","plank", glm::vec3(0.1f))),
    rest_mesh_(MeshObject::create(root,"cube","wool", glm::vec3(0.1f))){
    updateVectors();

    auto offset = glm::vec3(right_.x, right_.y - 1.f, right_.z) * 2.f;
    rest_position_ = root + offset;
}

void Vocksel::LegChain::render() {
    root_mesh_->setPosition(root_);
    rest_mesh_->setPosition(rest_position_);
    root_mesh_->render(EngineServices::resources().getShader("default"));
    rest_mesh_->render(EngineServices::resources().getShader("default"));
    for (auto& segment : segments_) {
        segment.render();
    }
}

void Vocksel::LegChain::updateLegShape() {
    if (segments_.empty()) return;

    // Backward pass - from tip to root
    segments_.back().setTipPosition(target_);

    for (int i = segments_.size() - 1; i > 0; i--) {
        glm::vec3 prev_tip = segments_[i-1].getTipPosition();
        glm::vec3 dir = glm::normalize(prev_tip - segments_[i].getTipPosition());
        segments_[i-1].setTipPosition(segments_[i].getTipPosition() + dir * segments_[i-1].getLength());
    }

    // Forward pass - from root to tip
    glm::vec3 current_base = root_;
    for (int i = 0; i < segments_.size(); i++) {
        glm::vec3 dir = glm::normalize(segments_[i].getTipPosition() - current_base);
        segments_[i].setTipPosition(current_base + dir * segments_[i].getLength());
        current_base = segments_[i].getTipPosition();  // Next segment's base is this segment's tip
    }
}




void Vocksel::LegChain::addLeg(float length) {
    glm::vec3 new_pos = root_;
    if (!segments_.empty()) {
        new_pos = segments_.back().getTipPosition();
    }

    new_pos = glm::vec3(new_pos.x, new_pos.y + length, new_pos.z);

    segments_.emplace_back(length, new_pos);
}

void Vocksel::LegChain::update(glm::vec3 root, glm::vec3 forward) {
    root_ = root;
    forward_ = forward;
    updateVectors();

    if (!segments_.empty()) {
        auto horizontal_distance = glm::length(segments_.back().getTipPosition() - rest_position_);

        if (horizontal_distance > 1.5f) {
            target_ = rest_position_;
        }
    }
    updateLegShape();
}

void Vocksel::LegChain::updateVectors() {
    right_ = glm::normalize(glm::cross(forward_, Constants::WORLD_UP));
    rest_position_ = root_ + (right_ * 1.5f) - (Constants::WORLD_UP * 1.5f);
}



Vocksel::LegChain::~LegChain() {

}


