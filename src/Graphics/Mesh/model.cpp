#include "Vocksel/Graphics/Mesh/model.h"

#include <unordered_map>


static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(s);
    while (std::getline(token_stream, token, delimiter)) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

static glm::vec3 calculateFaceNormal(const glm::vec3& v0,
                                   const glm::vec3& v1,
                                   const glm::vec3& v2) {
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    // Check for degenerate triangles
    if (length(edge1) < 0.0001f || length(edge2) < 0.0001f) {
        return glm::vec3(0.0f, 1.0f, 0.0f); // Default up vector
    }

    glm::vec3 normal = glm::cross(edge1, edge2);
    float len = glm::length(normal);

    // Check for zero-length normal (colinear points)
    if (len < 0.0001f) {
        return glm::vec3(0.0f, 1.0f, 0.0f); // Default up vector
    }

    return normal / len; // Same as normalize() but we already have length
}

Vocksel::Model::Model() {

}

void Vocksel::Model::addMesh(std::unique_ptr<StaticMesh> mesh) {
    meshes_.push_back(std::move(mesh));
}


bool Vocksel::Model::loadFromFile(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> position_indices, tex_coord_indices, normal_indices;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::vector<std::string> tokens = split(line, ' ');


        if (tokens[0] == "v") { // Vertex positions
            positions.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
        }else if (tokens[0] == "vt") { // Texture coord
            tex_coords.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]));
        }else if (tokens[0] == "vn") { // Vertex normal
            normals.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
        } else if (tokens[0] == "f") {
            size_t vertex_count = tokens.size() - 1;

            if (vertex_count == 3) { // Triangle
                for (size_t i = 1; i <= 3; i++) {
                    std::vector<std::string> faceTokens = split(tokens[i], '/');
                    position_indices.push_back(std::stoul(faceTokens[0]) - 1);

                    if (faceTokens.size() > 1 && !faceTokens[1].empty()) {
                        tex_coord_indices.push_back(std::stoul(faceTokens[1]) - 1);
                    }
                    if (faceTokens.size() > 2 && !faceTokens[2].empty()) {
                        normal_indices.push_back(std::stoul(faceTokens[2]) - 1);
                    }
                }
            }
            else if (vertex_count == 4) { // Quad - split into two triangles
                // First triangle (vertices 1, 2, 3)
                for (size_t i = 1; i <= 3; i++) {
                    std::vector<std::string> faceTokens = split(tokens[i], '/');
                    position_indices.push_back(std::stoul(faceTokens[0]) - 1);

                    if (faceTokens.size() > 1 && !faceTokens[1].empty()) {
                        tex_coord_indices.push_back(std::stoul(faceTokens[1]) - 1);
                    }
                    if (faceTokens.size() > 2 && !faceTokens[2].empty()) {
                        normal_indices.push_back(std::stoul(faceTokens[2]) - 1);
                    }
                }

                // Second triangle (vertices 1, 3, 4)
                // Vertex 1
                std::vector<std::string> faceTokens1 = split(tokens[1], '/');
                position_indices.push_back(std::stoul(faceTokens1[0]) - 1);
                if (faceTokens1.size() > 1 && !faceTokens1[1].empty()) {
                    tex_coord_indices.push_back(std::stoul(faceTokens1[1]) - 1);
                }
                if (faceTokens1.size() > 2 && !faceTokens1[2].empty()) {
                    normal_indices.push_back(std::stoul(faceTokens1[2]) - 1);
                }

                // Vertex 3
                std::vector<std::string> faceTokens3 = split(tokens[3], '/');
                position_indices.push_back(std::stoul(faceTokens3[0]) - 1);
                if (faceTokens3.size() > 1 && !faceTokens3[1].empty()) {
                    tex_coord_indices.push_back(std::stoul(faceTokens3[1]) - 1);
                }
                if (faceTokens3.size() > 2 && !faceTokens3[2].empty()) {
                    normal_indices.push_back(std::stoul(faceTokens3[2]) - 1);
                }

                // Vertex 4
                std::vector<std::string> faceTokens4 = split(tokens[4], '/');
                position_indices.push_back(std::stoul(faceTokens4[0]) - 1);
                if (faceTokens4.size() > 1 && !faceTokens4[1].empty()) {
                    tex_coord_indices.push_back(std::stoul(faceTokens4[1]) - 1);
                }
                if (faceTokens4.size() > 2 && !faceTokens4[2].empty()) {
                    normal_indices.push_back(std::stoul(faceTokens4[2]) - 1);
                }
            }
            else {
                std::cerr << "Unsupported face with " << vertex_count << " vertices" << std::endl;
            }
        }
    }

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    std::unordered_map<std::string, uint32_t> vertex_to_index;
    uint32_t current_index = 0;

    // Go through each face
    for (size_t i = 0; i < position_indices.size(); i++) {
        // Create a unique key for this vertex combination
        std::string key = std::to_string(position_indices[i]) + "_";
        if (i < tex_coord_indices.size()) key += std::to_string(tex_coord_indices[i]) + "_";
        if (i < normal_indices.size()) key += std::to_string(normal_indices[i]);

        size_t face_vertex_count = 3; // Default to triangles

        // Calculate face normal for the first triangle of this face
        glm::vec3 faceNormal(0, 1, 0);
        if (normals.empty() && i + 2 < position_indices.size()) {
            uint32_t idx0 = position_indices[i];
            uint32_t idx1 = position_indices[i+1];
            uint32_t idx2 = position_indices[i+2];

            // Verify position indices are valid
            if (idx0 < positions.size() && idx1 < positions.size() && idx2 < positions.size()) {
                faceNormal = calculateFaceNormal(
                    positions[idx0],
                    positions[idx1],
                    positions[idx2]
                );
            }
        }

        // Check if we've seen this vertex before
        if (vertex_to_index.find(key) == vertex_to_index.end()) {
            // Add new vertex
            vertex_to_index[key] = current_index++;

            // Position
            const auto& pos = positions[position_indices[i]];
            vertices.push_back(pos.x);
            vertices.push_back(pos.y);
            vertices.push_back(pos.z);

            // Texture coordinate
            if (i < tex_coord_indices.size() && !tex_coords.empty()) {
                const auto& tex = tex_coords[tex_coord_indices[i]];
                vertices.push_back(tex.s);
                vertices.push_back(tex.t);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            // Normal
            if (i < normal_indices.size() && !normals.empty()) {
                const auto& norm = normals[normal_indices[i]];
                vertices.push_back(norm.x);
                vertices.push_back(norm.y);
                vertices.push_back(norm.z);
            } else { // Default up normal
                vertices.push_back(faceNormal.x);
                vertices.push_back(faceNormal.y);
                vertices.push_back(faceNormal.z);
            }
        }

        indices.push_back(vertex_to_index[key]);
    }

    // Create the mesh (8 floats per vertex: position(3) + texcoord(2) + normal(3))
    addMesh(std::make_unique<StaticMesh>(
        vertices.data(),
        static_cast<int>(vertices.size()),
        indices.data(),
        static_cast<int>(indices.size()),
        8
    ));

    return true;

}

void Vocksel::Model::render(Shader &shader) {

    GLint current_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
    if (current_program != shader.ID_) {
        std::cerr << "ERROR: Wrong shader bound! Expected " << shader.ID_
                  << " but found " << current_program << std::endl;
        return;
    }

    shader.setMat4("model", transform_);

    for (const auto& mesh : meshes_) {

        mesh->bind();

        glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
        mesh->unbind();
    }
}

void Vocksel::Model::setTransformMatrix(const glm::mat4 &transform) {
    transform_ = transform;
}

void Vocksel::Model::cleanUp() {
    meshes_.clear();
}

Vocksel::Model::~Model() {
    cleanUp();
}





