#include "Vocksel/Graphics/Mesh/compute_mesh.h"

#include <iostream>

#include "tracy/Tracy.hpp"
#include "Vocksel/Core/constants.h"

constexpr uint32_t MAX_VERTICES = Vocksel::Constants::CHUNK_SIZE *
                                  Vocksel::Constants::CHUNK_SIZE *
                                  Vocksel::Constants::CHUNK_SIZE *
                                  15;

Vocksel::ComputeMesh::ComputeMesh() {
    glGenBuffers(1, &vertex_SSBO_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_SSBO_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_VERTICES * sizeof(float) * 4, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &index_SSBO_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, index_SSBO_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_VERTICES * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &counter_buffer_);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, counter_buffer_);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(float) * 4, nullptr, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_VERTICES * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
    
    glBindVertexArray(0);
}


void Vocksel::ComputeMesh::updateFromGPU() {
    ZoneScoped;
    uint32_t tri_count = readTriangleCount();
    uint32_t vertex_count = tri_count * 3;
    uint32_t index_count = tri_count * 3;

    glBindBuffer(GL_COPY_READ_BUFFER, vertex_SSBO_);
    glBindBuffer(GL_COPY_WRITE_BUFFER, VBO_);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
                        0, 0, vertex_count * sizeof(float) * 4);

    glBindBuffer(GL_COPY_READ_BUFFER, index_SSBO_);
    glBindBuffer(GL_COPY_WRITE_BUFFER, EBO_);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
                        0, 0, index_count * sizeof(uint32_t));
}

void Vocksel::ComputeMesh::bind() {
    ZoneScoped;
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
}

void Vocksel::ComputeMesh::unbind() {
    ZoneScoped;
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

