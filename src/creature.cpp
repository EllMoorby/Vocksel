#include "Vocksel/creature.h"

Vocksel::Creature::Creature(ModelManager &model_manager, ResourceManager &resource_manager, glm::vec3 position): resource_manager_(resource_manager), model_manager_(model_manager) ,position_(position) {
    head_segment_ = std::make_unique<CreatureSegment>(model_manager_, resource_manager_, 1.f, position_);

    addSegment(std::make_unique<CreatureSegment>(model_manager_, resource_manager_, 1.f, glm::vec3(position.x - 2.f , position.y, position.z)));
    addSegment(std::make_unique<CreatureSegment>(model_manager_, resource_manager_, 1.f, glm::vec3(position.x - 4.f , position.y, position.z)));
    addSegment(std::make_unique<CreatureSegment>(model_manager_, resource_manager_, 1.f, glm::vec3(position.x - 6.f , position.y, position.z)));
}

void Vocksel::Creature::setPosition(glm::vec3 position) {
    position_ = position;
    updateHeadPosition();
    updateSegmentPositions();
}


void Vocksel::Creature::addSegment(std::unique_ptr<CreatureSegment> creature_segment) {
    body_segments_.push_back(std::move(creature_segment));
}

void Vocksel::Creature::updateHeadPosition() {
    head_segment_->setPosition(position_);
}


void Vocksel::Creature::setDesiredMovement(glm::vec3 dir) {
    if (glm::length(dir) > 0) {
        desired_movement_ = glm::normalize(dir) * max_speed_;
    } else {
        desired_movement_ = glm::vec3(0.0f);
    }
}

void Vocksel::Creature::update(InputManager& input_manager, float delta_time) {
    setDesiredMovement(input_manager.getArrowVector());

    movement_ = glm::mix(movement_, desired_movement_, acceleration_ * delta_time);

    if (glm::length(movement_) > 0.1f) {
        front_ = glm::normalize(movement_);
    }

    position_ += movement_ * delta_time;
    updateHeadPosition();
    updateSegmentPositions();

    head_segment_->update(delta_time);
    for (auto &segment : body_segments_) {
        segment->update(delta_time);
    }
}


void Vocksel::Creature::updateSegmentPositions() {
    auto* current_segment = head_segment_.get();

    head_segment_->setDirection(front_);

    for (auto& next_segment : body_segments_) {
        const float gap_size = current_segment->getRadius() + next_segment->getRadius();
        const glm::vec3 current_pos = current_segment->getPosition();
        glm::vec3 next_pos = next_segment->getPosition();

        const glm::vec3 dir = next_pos - current_pos;
        const glm::vec3 normalized_dir = glm::normalize(dir);
        next_segment->setDirection(-normalized_dir);

        const float distance = glm::length(dir);

        if (distance > gap_size) {
            const glm::vec3 corrected_pos = current_pos + normalized_dir * gap_size;
            next_segment->setPosition(corrected_pos);
        }

        current_segment = next_segment.get();
    }

}



void Vocksel::Creature::render(Shader &shader) {
    head_segment_->render(shader);
    for (auto& segment : body_segments_) {
        segment->render(shader);
    }
}

void Vocksel::Creature::cleanUp() {
    head_segment_->cleanUp();
    head_segment_.reset();
    for (auto& segment : body_segments_) {
        segment->cleanUp();
    }
    body_segments_.clear();
}


Vocksel::Creature::~Creature() {
    cleanUp();
}




