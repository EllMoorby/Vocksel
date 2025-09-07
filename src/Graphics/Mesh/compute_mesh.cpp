#include "Vocksel/Graphics/Mesh/compute_mesh.h"

#include <iostream>
#include "Vocksel/Core/constants.h"

constexpr uint32_t MAX_VERTICES = (Vocksel::Constants::CUBES_PER_CHUNK + 1) *
                                  (Vocksel::Constants::CUBES_PER_CHUNK + 1) *
                                  (Vocksel::Constants::CUBES_PER_CHUNK + 1) *
                                  15;

Vocksel::ComputeMesh::ComputeMesh() {
    glGenBuffers(1, &vertex_SSBO_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_SSBO_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_VERTICES * sizeof(float) * 4, nullptr, GL_DYNAMIC_COPY);

    glGenBuffers(1, &indirect_buffer_);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer_);

    // Necessary for AMD systems (after 12 hours of debugging D:)
    DrawElementsIndirectCommand cmd = {};
    cmd.count = 0;
    cmd.instanceCount = 1;
    cmd.first = 0;
    cmd.baseInstance = 0;
    glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand), &cmd, GL_DYNAMIC_DRAW);


    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);


    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);


    glBindBuffer(GL_ARRAY_BUFFER, vertex_SSBO_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void Vocksel::ComputeMesh::bind() {
    glBindVertexArray(VAO_);

}

void Vocksel::ComputeMesh::unbind() {
    glBindVertexArray(0);
}

Vocksel::ComputeMesh::~ComputeMesh() {
    glDeleteBuffers(1, &vertex_SSBO_);
    glDeleteBuffers(1, &indirect_buffer_);
}

