#ifndef CREATURE_H
#define CREATURE_H
#include <vector>

#include "creature_segment.h"
#include "Vocksel/Core/input_manager.h"
#include "Vocksel/Graphics/shader.h"

namespace Vocksel {
    class Creature {
        public:
        Creature(glm::vec3 position);
        ~Creature();

        void update(float delta_time);
        void render(Shader& shader);

        void setDesiredMovement(glm::vec3 dir);

        void addSegment(std::unique_ptr<CreatureSegment> creature_segment);

        void setPosition(glm::vec3 position);

        const glm::vec3& getPosition() const {return position_;}
        std::unique_ptr<CreatureSegment>& getHeadSegment() {return head_segment_;}
        std::vector<std::unique_ptr<CreatureSegment>>& getBodySegments() {return body_segments_;}

        void cleanUp();


    private:
        void updateHeadPosition();
        void updateSegmentPositions();
    private:
        glm::vec3 front_ = glm::vec3(0.f, 0.f, 1.f);
        glm::vec3 movement_ = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 desired_movement_ = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 position_;

        float acceleration_ = 10.0f;
        float max_speed_ = 10.0f;
        float max_turn_angle_ = glm::radians(45.0f);

        std::unique_ptr<CreatureSegment> head_segment_;
        std::vector<std::unique_ptr<CreatureSegment>> body_segments_;
    };
}


#endif //CREATURE_H
