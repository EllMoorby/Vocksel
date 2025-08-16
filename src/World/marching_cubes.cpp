#include "Vocksel/World/marching_cubes.h"

#include "Vocksel/Core/engine_services.h"
#include "Vocksel/World/mc_lookup.h"

Vocksel::MarchingCubes::MarchingCubes(): compute_shader_(EngineServices::resources().getShader("marching_cubes")) {
    createLookupTextures();
}



void Vocksel::MarchingCubes::createLookupTextures() {
    createEdgeTableTexture();
    createTriTableTexture();
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
    compute_shader_.setFloat("isolevel", 0.0f);
    compute_shader_.setInt("chunk_size", Constants::CHUNK_SIZE);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, output.getVertexSSBO());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output.getIndexSSBO());
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, output.getCounterBuffer());

    uint32_t zero = 0;
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, output.getCounterBuffer());
    glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(uint32_t), &zero);

    const int groups = (Constants::CHUNK_SIZE + 7) / 8;
    compute_shader_.dispatchCompute(groups, groups, groups);

    output.updateFromGPU();
}

Vocksel::MarchingCubes::~MarchingCubes() {
    glDeleteTextures(1, &edge_table_texture_);
    glDeleteTextures(1, &tri_table_texture_);
}




