#ifndef CREATURE_SEGMENT_H
#define CREATURE_SEGMENT_H
#include "shader.h"
#include "sphere.h"

namespace Vocksel {
    class CreatureSegment {
        public:
        CreatureSegment(ModelManager &model_manager, ResourceManager &resource_manager, float radius, glm::vec3 position);
        ~CreatureSegment();

        void render(Shader& shader);
        void update(float delta_time);

        void setPosition(glm::vec3 position);
        void setDirection(glm::vec3 direction) {direction_ = direction;}

        float getRadius() const {return radius_;}
        glm::vec3 getPosition() const {return position_;}
        glm::vec3 getDirection() const {return direction_;}

        void cleanUp();
        float angle_to_ahead_ = 0.f;
        glm::vec3 new_dir_ = glm::vec3(0.f);


    private:
        std::unique_ptr<Sphere> sphere_;
        float radius_;
        glm::vec3 position_;
        glm::vec3 direction_ = glm::vec3(0.f);

        ResourceManager &resource_manager_;
        ModelManager &model_manager_;

    };
}

#endif //CREATURE_SEGMENT_H
