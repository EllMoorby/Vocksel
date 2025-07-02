#include "Vocksel/chunk.h"

// Face directions (order: +X, -X, +Y, -Y, +Z, -Z) [UNUSED]
const glm::ivec3 faceNormals[6] = {
    {1, 0, 0}, {-1, 0, 0},
    {0, 1, 0}, {0, -1, 0},
    {0, 0, 1}, {0, 0, -1}
};

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



Vocksel::Chunk::Chunk(glm::vec3 position, ResourceManager& resource_manager): position_(position), resource_manager_(resource_manager) {
    // set it to random for now
    for (int x = 0; x < kSize; ++x)
        for (int y = 0; y < kSize; ++y)
            for (int z = 0; z < kSize; ++z)
                voxels_[x][y][z] = rand() % 5;

    generateMesh();
}

void Vocksel::Chunk::generateMesh() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int idx_offset = 0;

    TextureAtlas& atlas = resource_manager_.getBlockAtlas();

    // For each voxel
    for (int x = 0; x < kSize; ++x) {
        for (int y = 0; y < kSize; ++y) {
            for (int z = 0; z < kSize; ++z) {
                if (voxels_[x][y][z] == 0) continue;

                // Skip on air
                int block_type = voxels_[x][y][z];

                // Check for each face
                for (int dir = 0; dir < 6; ++dir) {
                    static const glm::ivec3 directions[] = {
                        {1, 0, 0}, {-1, 0, 0},  // +X, -X
                        {0, 1, 0}, {0, -1, 0},  // +Y, -Y
                        {0, 0, 1}, {0, 0, -1}   // +Z, -Z
                    };


                    glm::ivec3 neighbor = glm::ivec3(x,y,z) + directions[dir];

                    // Is this face blocked?
                    bool visible = (neighbor.x < 0 || neighbor.y < 0 || neighbor.z < 0 ||
                        neighbor.x >= kSize || neighbor.y >= kSize || neighbor.z >= kSize) ||
                       (neighbor.x >= 0 && neighbor.y >= 0 && neighbor.z >= 0 &&
                        neighbor.x < kSize && neighbor.y < kSize && neighbor.z < kSize &&
                        voxels_[neighbor.x][neighbor.y][neighbor.z] == 0);

                    if (not visible) continue;

                    glm::vec3 blockPos = glm::vec3(x, y, z);

                    // Get the texture coordinate of the texture
                    glm::vec2 uv_offset = atlas.getUVOffset(getBlockType(block_type));
                    float tile_scale = atlas.getTileScale();

                    const glm::vec2 baseUVs[4] = {
                        {0.0f, 0.0f}, {tile_scale, 0.0f},
                        {tile_scale, tile_scale}, {0.0f, tile_scale}
                    };

                    for (int i = 0; i < 4; ++i) {
                        glm::vec3 v = blockPos + faceVertices[dir][i];
                        vertices.push_back(v.x);
                        vertices.push_back(v.y);
                        vertices.push_back(v.z);

                        // Apply atlas offset and scaling
                        vertices.push_back(uv_offset.x + baseUVs[i].x);
                        vertices.push_back(uv_offset.y + baseUVs[i].y);
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

    // Create the mesh
    mesh_ = std::make_unique<StaticMesh>(vertices.data(), vertices.size(), indices.data(), indices.size(), 5);

}

std::string Vocksel::Chunk::getBlockType(int block_type) {
    switch (block_type) {
        case 0:
        return "air";
        case 1:
        return "grass";
        case 2:
        return "stone";
        case 3:
        return "plank";
        case 4:
        return "wool";
    }
}

void Vocksel::Chunk::editVoxel(uint8_t x, uint8_t y, uint8_t z, uint8_t block_type) {
    voxels_[x][y][z] = block_type;
}

glm::vec3 Vocksel::Chunk::getPosition() {
    return position_;
}



void Vocksel::Chunk::render(Shader &shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));

    shader.setMat4("model", model);
    shader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));

    TextureAtlas& atlas = resource_manager_.getBlockAtlas();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas.getAtlasTexture());
    shader.setInt("textureAtlas", 0);

    mesh_->bind();
    glDrawElements(GL_TRIANGLES, mesh_->getIndexCount(), GL_UNSIGNED_INT, 0);
    mesh_->unbind();
}

void Vocksel::Chunk::cleanUp() {
    mesh_.reset();
}


Vocksel::Chunk::~Chunk() {
    cleanUp();
}



