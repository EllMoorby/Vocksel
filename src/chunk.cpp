#include "Vocksel/chunk.h"


// Face directions (order: +X, -X, +Y, -Y, +Z, -Z)
const glm::ivec3 faceNormals[6] = {
    {1, 0, 0}, {-1, 0, 0},
    {0, 1, 0}, {0, -1, 0},
    {0, 0, 1}, {0, 0, -1}
};

// 6 faces with 4 corners each (quad)
const glm::vec3 faceVertices[6][4] = {
    // +X (right)
    {{0.5f, -0.5f, -0.5f}, {0.5f,  0.5f, -0.5f}, {0.5f,  0.5f,  0.5f}, {0.5f, -0.5f,  0.5f}},
    // -X (left)
    {{-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}},
    // +Y (top)
    {{-0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f,  0.5f}, {0.5f, 0.5f,  0.5f}, {0.5f, 0.5f, -0.5f}},
    // -Y (bottom)
    {{-0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f,  0.5f}},
    // +Z (front)
    {{-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f,  0.5f, 0.5f}, {-0.5f,  0.5f, 0.5f}},
    // -Z (back)
    {{0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, {0.5f,  0.5f, -0.5f}},
};




Vocksel::Chunk::Chunk(glm::vec3 position): position_(position) {
    // set it to random for now
    for (int x = 0; x < kSize; ++x)
        for (int y = 0; y < kSize; ++y)
            for (int z = 0; z < kSize; ++z)
                voxels_[x][y][z] = rand() % 2;

    generateMesh();


}

void Vocksel::Chunk::generateMesh() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int idx_offset = 0;

    for (int x = 0; x < kSize; ++x) {
        for (int y = 0; y < kSize; ++y) {
            for (int z = 0; z < kSize; ++z) {
                if (voxels_[x][y][z] == 0) continue;

                for (int dir = 0; dir < 6; ++dir) {
                    static const glm::ivec3 directions[] = {
                        {1, 0, 0}, {-1, 0, 0},  // +X, -X
                        {0, 1, 0}, {0, -1, 0},  // +Y, -Y
                        {0, 0, 1}, {0, 0, -1}   // +Z, -Z
                    };


                    glm::ivec3 neighbor = glm::ivec3(x,y,z) + directions[dir];

                    bool visible = (neighbor.x < 0 || neighbor.y < 0 || neighbor.z < 0 ||
                        neighbor.x >= kSize || neighbor.y >= kSize || neighbor.z >= kSize) ||
                       (neighbor.x >= 0 && neighbor.y >= 0 && neighbor.z >= 0 &&
                        neighbor.x < kSize && neighbor.y < kSize && neighbor.z < kSize &&
                        voxels_[neighbor.x][neighbor.y][neighbor.z] == 0);

                    if (not visible) continue;


                    glm::vec3 blockPos = glm::vec3(x, y, z);

                    const glm::vec2 uvCoords[4] = {
                        {0.0f, 0.0f},
                        {1.0f, 0.0f},
                        {1.0f, 1.0f},
                        {0.0f, 1.0f}
                    };

                    // Add 4 vertices for this face
                    for (int i = 0; i < 4; ++i) {
                        glm::vec3 v = blockPos + faceVertices[dir][i];
                        vertices.push_back(v.x);
                        vertices.push_back(v.y);
                        vertices.push_back(v.z);

                        vertices.push_back(uvCoords[i].x);
                        vertices.push_back(uvCoords[i].y);
                    }

                    // Add 2 triangles (6 indices)
                    indices.push_back(idx_offset + 0);
                    indices.push_back(idx_offset + 1);
                    indices.push_back(idx_offset + 2);
                    indices.push_back(idx_offset + 2);
                    indices.push_back(idx_offset + 3);
                    indices.push_back(idx_offset + 0);

                    idx_offset += 4;

                }
            }
        }
    }

    mesh_ = std::make_unique<StaticMesh>(vertices.data(), vertices.size(), indices.data(), indices.size(), 5);

}

void Vocksel::Chunk::render(Shader &shader, const Camera &camera) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));

    shader.setMat4("model", model);
    shader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));

    mesh_->bind();
    glDrawElements(GL_TRIANGLES, mesh_->getIndexCount(), GL_UNSIGNED_INT, 0);
    mesh_->unbind();
}

Vocksel::Chunk::~Chunk() {
    mesh_.reset();
}



