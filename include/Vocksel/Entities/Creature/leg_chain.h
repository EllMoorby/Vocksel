#ifndef LEG_CHAIN_H
#define LEG_CHAIN_H
#include <vector>

#include "Vocksel/Entities/Creature/leg_segment.h"
#include "glm/vec3.hpp"

namespace Vocksel {

    class LegChain {
    public:
        LegChain(glm::vec3 root, glm::vec3 forward);
        ~LegChain();

        void update(glm::vec3 root, glm::vec3 forward);
        void render();

        void updateVectors();

        void setTarget(glm::vec3 target) {target_ = target;};
        glm::vec3 getTarget() const {return target_;};

        void addLeg(float length);
        glm::vec3 forward_;
        glm::vec3 right_ = glm::vec3(0.f);
        std::vector<LegSegment> segments_;



    private:
        void updateLegShape();
    private:
        glm::vec3 root_;
        std::unique_ptr<MeshObject> root_mesh_;
        std::unique_ptr<MeshObject> rest_mesh_;

        glm::vec3 target_;
        glm::vec3 rest_position_;

    };
}

#endif //LEG_CHAIN_H
