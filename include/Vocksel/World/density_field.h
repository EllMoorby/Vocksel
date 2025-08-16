#ifndef DENSITY_FIELD_H
#define DENSITY_FIELD_H
#include <vector>
#include "glm/glm.hpp"
#include "Vocksel/Graphics/texture_3d.h"

namespace Vocksel {
    class DensityField {
        public:
        DensityField(glm::ivec3 size, uint32_t gpu_format);

        void init(glm::ivec3 size);

        const Texture3D& getTexture() const { return texture_; }
        bool isInitialized() const { return texture_.getId() != 0; }

        void setVoxel(int x, int y, int z, float value);
        float getVoxel(int x, int y, int z) const;
        void uploadToGPU();

        private:

        inline size_t index(int x, int y, int z) const;
        Texture3D texture_;
        glm::ivec3 size_;
        std::vector<float> voxels_;

    };
}
#endif //DENSITY_FIELD_H
