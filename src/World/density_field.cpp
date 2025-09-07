#include "Vocksel/World/density_field.h"

#include <iostream>
#include <ostream>
#if DEBUG
#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"
#endif

#include "Vocksel/Core/constants.h"

Vocksel::DensityField::DensityField(glm::ivec3 size, uint32_t gpu_format): size_(size), texture_(), voxels_(size.x * size.y * size.z, 0.f) {
}

inline size_t Vocksel::DensityField::index(int x, int y, int z) const {
    return x + y * size_.x + z * size_.x * size_.y;
}

float Vocksel::DensityField::getVoxel(int x, int y, int z) const {
    return voxels_[index(x, y, z)];
}

void Vocksel::DensityField::setVoxel(int x, int y, int z, float value) {
    voxels_[index(x, y, z)] = value;
}


void Vocksel::DensityField::uploadToGPU() {
#if DEBUG
    TracyGpuZone("UploadToGPU");
#endif
    
    if (voxels_.empty()) return;

    texture_.createIfNeeded(nullptr, size_);

    texture_.bind();

    glTexSubImage3D(
        GL_TEXTURE_3D,
        0,
        0, 0, 0,
        texture_.getSize().x,
        texture_.getSize().y,
        texture_.getSize().z,
        GL_RED,
        GL_FLOAT,
        voxels_.data()
    );

    texture_.unbind();

}





