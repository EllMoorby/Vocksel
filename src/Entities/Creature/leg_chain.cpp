#include "Vocksel/Entities/Creature/leg_chain.h"

#include "Vocksel/Core/engine_services.h"

Vocksel::LegChain::LegChain(glm::vec3 root, glm::vec3 forward, glm::vec3 offset): root_(root), forward_(forward), offset_(offset),
    root_mesh_(MeshObject::create(root,"cube","plank", glm::vec3(0.1f))),
    rest_mesh_(MeshObject::create(root,"cube","wool", glm::vec3(0.1f))){
    updateVectors();
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

void Vocksel::LegChain::updateLegShape(float delta_time) {
    if (segments_.empty()) return;


    float total_length = 0.f;
    for (auto& segment : segments_) {
        total_length += segment.getLength();
    }

    if (glm::distance(target_, root_) > total_length) {
        glm::vec3 dir = glm::normalize(target_ - root_);
        glm::vec3 current = root_;
        for (auto& segment : segments_) {
            segment.setTipPosition(current + dir * segment.getLength());
            current = segment.getTipPosition();
        }
        last_target_ = target_;
        return;
    }

    if (glm::distance(target_, last_target_) < 0.0001f) {
        return;
    }

    for (int x = 0; x < 20; x++) {

        uint32_t idx = 0;
        for (auto& segment : segments_) {
            segment.setTipPosition(segment.getTipPosition() + velocities_[idx] * delta_time * 2.f);
            idx++;
        }

        // Backward pass - from tip to root
        segments_.back().setTipPosition(target_);

        for (int i = segments_.size() - 1; i > 0; i--) {
            glm::vec3 dir = segments_[i-1].getTipPosition() - segments_[i].getTipPosition();
            if (glm::length(dir) > 0.0001f) {
                dir = glm::normalize(dir);
                segments_[i-1].setTipPosition(segments_[i].getTipPosition() + dir * segments_[i-1].getLength());
            }
        }

        // Forward pass - from root to tip
        glm::vec3 current_base = root_;
        for (int i = 0; i < segments_.size(); i++) {
            glm::vec3 dir = segments_[i].getTipPosition() - current_base;
            if (glm::length(dir) > 0.0001f) {
                dir = glm::normalize(dir);
                segments_[i].setTipPosition(current_base + dir * segments_[i].getLength());
                current_base = segments_[i].getTipPosition();
            }
        }

        if (glm::distance(segments_.back().getTipPosition(), target_) < tolerance_) {
            break;
        }

        last_target_ = target_;
    }
}




void Vocksel::LegChain::addLegSegment(float length, glm::vec3 velocity) {
    glm::vec3 new_pos = root_;
    if (!segments_.empty()) {
        new_pos = segments_.back().getTipPosition();
    }

    new_pos = glm::vec3(new_pos.x, new_pos.y + length, new_pos.z);

    segments_.emplace_back(length, new_pos);
    velocities_.push_back(velocity);
}

void Vocksel::LegChain::update(float delta_time, glm::vec3 root, glm::vec3 forward) {
    root_ = root;
    forward_ = forward;
    updateVectors();

    if (!segments_.empty()) {
        auto horizontal_distance = glm::length(segments_.back().getTipPosition() - rest_position_);

        if (horizontal_distance > reset_distance_) {
            target_ = rest_position_;
        }
    }
    updateLegShape(delta_time);
}

void Vocksel::LegChain::updateVectors() {
    right_ = glm::normalize(glm::cross(forward_, Constants::WORLD_UP));
    rest_position_ = root_ + (right_ * offset_.x) + (forward_ * offset_.z) + (Constants::WORLD_UP * offset_.y);
}

void Vocksel::LegChain::setVelocity(glm::vec3 velocity, uint32_t idx) {
    if (idx+1 > segments_.size() || idx+1 > velocities_.size()) {
        std::cerr << "Index out of bounds when setting velocity for leg joint" << std::endl;
    }
    velocities_[idx] = velocity;
}




Vocksel::LegChain::~LegChain() {

}


