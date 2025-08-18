#include "Vocksel/Graphics/Mesh/compute_mesh.h"

#include <iostream>

#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"
#include "Vocksel/Core/constants.h"

constexpr uint32_t MAX_VERTICES = (Vocksel::Constants::CUBES_PER_CHUNK + 1) *
                                  (Vocksel::Constants::CUBES_PER_CHUNK + 1) *
                                  (Vocksel::Constants::CUBES_PER_CHUNK + 1) *
                                  15;

Vocksel::ComputeMesh::ComputeMesh() {
    glGenBuffers(1, &vertex_SSBO_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_SSBO_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_VERTICES * sizeof(float) * 4, nullptr, GL_DYNAMIC_COPY);

    glGenBuffers(1, &index_SSBO_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, index_SSBO_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_VERTICES * sizeof(uint32_t), nullptr, GL_DYNAMIC_COPY);

    glGenBuffers(1, &counter_buffer_);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, counter_buffer_);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(uint32_t), nullptr, GL_DYNAMIC_READ);

    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_SSBO_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_SSBO_);
    
    glBindVertexArray(0);
}


void Vocksel::ComputeMesh::updateFromGPU() {
    ZoneScoped;
    TracyGpuZone("Update From GPU");

    triangle_count_ = readTriangleCount();
}

void Vocksel::ComputeMesh::bind() {
    glBindVertexArray(VAO_);

}

void Vocksel::ComputeMesh::unbind() {
    glBindVertexArray(0);
}




uint32_t Vocksel::ComputeMesh::readTriangleCount() const {
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, counter_buffer_);
    uint32_t count;
    glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(uint32_t), &count);
    return count;
}


Vocksel::ComputeMesh::~ComputeMesh() {
    glDeleteBuffers(1, &vertex_SSBO_);
    glDeleteBuffers(1, &index_SSBO_);
    glDeleteBuffers(1, &counter_buffer_);
}

