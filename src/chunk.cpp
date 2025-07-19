#include "Vocksel/chunk.h"

#include "Vocksel/engine_services.h"

// Face directions (order: +X, -X, +Y, -Y, +Z, -Z)
const glm::ivec3 face_normals[6] = {
    {1, 0, 0}, {-1, 0, 0},
    {0, 1, 0}, {0, -1, 0},
    {0, 0, 1}, {0, 0, -1}
};

const glm::vec3 face_vertices[6][4] = {
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



Vocksel::Chunk::Chunk(glm::vec3 position): position_(position){
    // Set it all to grass
    for (int x = 0; x < Constants::CHUNK_SIZE; ++x)
        for (int y = 0; y < Constants::CHUNK_HEIGHT; ++y)
            for (int z = 0; z < Constants::CHUNK_SIZE; ++z)
                voxels_[x][y][z] = 1;
}

void Vocksel::Chunk::setNeighbor(int direction, Chunk *neighbor) {
    if (direction >= 0 && direction < 6) neighbors_[direction] = neighbor;
}

Vocksel::Chunk *Vocksel::Chunk::getNeighbor(int direction) const {
    if (direction >= 0 && direction < 6) return neighbors_[direction];
    std::cerr << "direction out of bounds: " << direction << std::endl;
    return nullptr;
}



void Vocksel::Chunk::generateMesh() {
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    uint32_t idx_offset = 0;

    TextureAtlas& atlas = EngineServices::resources().getBlockAtlas();

    constexpr uint32_t kSize = Constants::CHUNK_SIZE;

    // For each voxel
    for (int x = 0; x < kSize; ++x) {
        for (int y = 0; y < Constants::CHUNK_HEIGHT; ++y) {
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

                    glm::ivec3 neighbor_pos = glm::ivec3(x, y, z) + directions[dir];

                    bool out_of_bounds = neighbor_pos.x < 0 || neighbor_pos.y < 0 || neighbor_pos.z < 0 ||
                                         neighbor_pos.x >= kSize || neighbor_pos.y >= Constants::CHUNK_HEIGHT || neighbor_pos.z >= kSize;

                    bool visible = false;

                    if (out_of_bounds) {
                        // Try to get neighbor chunk
                        Chunk* neighbor_chunk = getNeighbor(dir);
                        if (neighbor_chunk) {
                            glm::ivec3 local_pos = neighbor_pos;
                            if (local_pos.x < 0) local_pos.x += kSize;
                            if (local_pos.x >= kSize) local_pos.x -= kSize;
                            if (local_pos.z < 0) local_pos.z += kSize;
                            if (local_pos.z >= kSize) local_pos.z -= kSize;

                            uint8_t neighbor_block = neighbor_chunk->getVoxel(local_pos.x, local_pos.y, local_pos.z);
                            visible = (neighbor_block == 0);
                        } else {
                            visible = true; // no neighbor chunk loaded
                        }
                    } else {
                        visible = (voxels_[neighbor_pos.x][neighbor_pos.y][neighbor_pos.z] == 0);
                    }

                    if (not visible) continue;

                    glm::vec3 block_pos = glm::vec3(x, y, z);
                    glm::vec3 block_normal = glm::vec3(face_normals[dir]);

                    // Get the texture coordinate of the texture
                    glm::vec2 uv_offset = atlas.getUVOffset(getBlockType(block_type));
                    float tile_scale = atlas.getTileScale();

                    const glm::vec2 kBaseUVs[4] = {
                        {0.0f, 0.0f}, {tile_scale, 0.0f},
                        {tile_scale, tile_scale}, {0.0f, tile_scale}
                    };

                    for (int i = 0; i < 4; ++i) {
                        glm::vec3 v = block_pos + face_vertices[dir][i];
                        vertices.push_back(v.x);
                        vertices.push_back(v.y);
                        vertices.push_back(v.z);

                        // Apply atlas offset and scaling
                        vertices.push_back(uv_offset.x + kBaseUVs[i].x);
                        vertices.push_back(uv_offset.y + kBaseUVs[i].y);


                        vertices.push_back(block_normal.x);
                        vertices.push_back(block_normal.y);
                        vertices.push_back(block_normal.z);
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
    mesh_ = std::make_unique<StaticMesh>(vertices.data(), vertices.size(), indices.data(), indices.size(), 8);

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
    std::cerr << "Unknown block type: " << block_type << std::endl;
    return "";
}

void Vocksel::Chunk::editVoxel(uint8_t x, uint8_t y, uint8_t z, uint8_t block_type) {
    if (x > Constants::CHUNK_SIZE - 1 || y > Constants::CHUNK_HEIGHT - 1 || z > Constants::CHUNK_SIZE - 1) {
        std::cerr << "Editing block out of range. x: " << x << " y: "<< y << " z: " << z << std::endl;
        return;
    }
    voxels_[x][y][z] = block_type;
}

uint8_t Vocksel::Chunk::getVoxel(uint8_t x, uint8_t y, uint8_t z) {
    if (x > Constants::CHUNK_SIZE - 1 || y > Constants::CHUNK_HEIGHT - 1 || z > Constants::CHUNK_SIZE - 1) {
        std::cerr << "Getting block out of range. x: " << x << " y: "<< y << " z: " << z << std::endl;
        return 0;
    }
    return voxels_[x][y][z];
}

glm::vec3 Vocksel::Chunk::getPosition() {
    return position_;
}



void Vocksel::Chunk::render(Shader &shader) {

    GLint current_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
    if (current_program != shader.ID_) {
        std::cerr << "ERROR: Wrong shader bound! Expected " << shader.ID_
                  << " but found " << current_program << std::endl;
        return;
    }


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));

    shader.setMat4("model", model);
    shader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));

    TextureAtlas& atlas = EngineServices::resources().getBlockAtlas();

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



