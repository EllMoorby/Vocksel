#ifndef LEG_SEGMENT_H
#define LEG_SEGMENT_H
#include "glm/vec3.hpp"
#include "Vocksel/Graphics/Mesh/mesh_object.h"

namespace Vocksel {
    class LegSegment {
        public:
        LegSegment(float length, glm::vec3 tip_position);
        ~LegSegment();

        glm::vec3 getTipPosition() const {return tip_position_;}
        float getLength() const {return length_;}

        void setTipPosition(glm::vec3 tip_position) {tip_position_ = tip_position;}
        void setLength(float length) {length_ = length;}

        void render();

        // Make sure the class is movable
        LegSegment(LegSegment&&) noexcept = default;
        LegSegment& operator=(LegSegment&&) noexcept = default;

        // Delete copy operations
        LegSegment(const LegSegment&) = delete;
        LegSegment& operator=(const LegSegment&) = delete;

        private:
        std::unique_ptr<MeshObject> tip_mesh_;
        float length_;
        glm::vec3 tip_position_;
    };
}

#endif //LEG_SEGMENT_H
