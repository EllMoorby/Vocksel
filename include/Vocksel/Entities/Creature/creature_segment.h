#ifndef CREATURE_SEGMENT_H
#define CREATURE_SEGMENT_H
#include <vector>

#include "leg_chain.h"
#include "Vocksel/Graphics/Mesh/mesh_object.h"
#include "Vocksel/Graphics/shader.h"

namespace Vocksel {
    class CreatureSegment {
        public:
        CreatureSegment(float radius, float gap, glm::vec3 position);
        ~CreatureSegment();

        void render(Shader& shader);
        void update(float delta_time);

        void setPosition(glm::vec3 position);
        void setDirection(glm::vec3 direction) {direction_ = direction;}
        void setAngleToAhead(float angle) {angle_to_ahead_ = angle;}

        float getRadius() const {return radius_;}
        glm::vec3 getPosition() const {return position_;}
        glm::vec3 getDirection() const {return direction_;}
        float getGap() const {return gap_;}
        float getAngleToAhead() const {return angle_to_ahead_;}

        LegChain leg_;


    private:

        std::unique_ptr<MeshObject> body_mesh_;
        float radius_;
        float gap_;

        glm::vec3 position_;
        glm::vec3 direction_ = glm::vec3(0.f);
        float angle_to_ahead_ = 0.f;
        glm::vec3 new_dir_ = glm::vec3(0.f);
    };
}

#endif //CREATURE_SEGMENT_H
