#include "Vocksel/World/marching_cubes.h"
#if DEBUG
#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"
#endif
#include "Vocksel/Core/engine_services.h"
#include "Vocksel/World/mc_lookup.h"
#include "Vocksel/World/world.h"

uint32_t Vocksel::MarchingCubes::edge_table_texture_ = 0;
uint32_t Vocksel::MarchingCubes::tri_table_texture_ = 0;
bool Vocksel::MarchingCubes::created_lookup_ = false;


Vocksel::MarchingCubes::MarchingCubes(): compute_shader_(EngineServices::resources().getShader("marching_cubes")) {
    createLookupTextures();
}



void Vocksel::MarchingCubes::createLookupTextures() {
    if (!created_lookup_) {
        createEdgeTableTexture();
        createTriTableTexture();
        created_lookup_ = true;
    }
}

void Vocksel::MarchingCubes::createEdgeTableTexture() {
    glGenTextures(1, &edge_table_texture_);
    glBindTexture(GL_TEXTURE_1D, edge_table_texture_);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32UI,
                256, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, edge_table);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
}

void Vocksel::MarchingCubes::createTriTableTexture() {
    glGenTextures(1, &tri_table_texture_);
    glBindTexture(GL_TEXTURE_2D, tri_table_texture_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I,
                16, 256, 0, GL_RED_INTEGER, GL_INT, tri_table);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}



void Vocksel::MarchingCubes::computeMesh(const Texture3D &density_tex, ComputeMesh &output) {
#if DEBUG
    TracyGpuZone("Compute mesh");
#endif


    DrawElementsIndirectCommand cmd = {};
    cmd.count = 0;
    cmd.instanceCount = 1;
    cmd.first = 0;
    cmd.baseInstance = 0;

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, output.getIndirectBuffer());
    glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(DrawElementsIndirectCommand), &cmd);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, density_tex.getId());

    glBindImageTexture(0, density_tex.getId(), 0, GL_TRUE, 0, GL_READ_ONLY, GL_R32F);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, edge_table_texture_);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tri_table_texture_);

    compute_shader_.use();
    compute_shader_.setInt("edge_table", 1);
    compute_shader_.setInt("tri_table", 2);
    compute_shader_.setFloat("isolevel", World::isolevel);
    compute_shader_.setInt("voxels_per_axis", Constants::CUBES_PER_CHUNK + 1);
    compute_shader_.setFloat("voxel_size", float(Constants::CHUNK_SIZE) / float(Constants::CUBES_PER_CHUNK));



    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, output.getVertexSSBO());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, output.getIndirectBuffer());



    const int groups = (Constants::CUBES_PER_CHUNK + 7) / 8;

    compute_shader_.dispatchCompute(groups, groups, groups);


}

void Vocksel::MarchingCubes::cleanUp() {
    if (created_lookup_) {
        glDeleteTextures(1, &edge_table_texture_);
        glDeleteTextures(1, &tri_table_texture_);
        edge_table_texture_ = 0;
        tri_table_texture_ = 0;
        created_lookup_ = false;
    }
}

Vocksel::MarchingCubes::~MarchingCubes() {

}




