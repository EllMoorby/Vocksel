#include "../include/Vocksel/model.h"

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
    std::vector<unsigned int> position_indices, tex_coord_indices, normal_indices;

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
            for (size_t i = 1; i < tokens.size(); i++) {
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
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::unordered_map<std::string, unsigned int> vertex_to_index;
    unsigned int current_index = 0;

    // Go through each face
    for (size_t i = 0; i < position_indices.size(); i++) {
        // Create a unique key for this vertex combination
        std::string key = std::to_string(position_indices[i]) + "_";
        if (i < tex_coord_indices.size()) key += std::to_string(tex_coord_indices[i]) + "_";
        if (i < normal_indices.size()) key += std::to_string(normal_indices[i]);

        size_t face_vertex_count = 3; // Default to triangles

        // Calculate face normal for the first triangle of this face
        glm::vec3 faceNormal(0, 1, 0); // Default up normal
        if (normals.empty() && face_vertex_count >= 3) {
            faceNormal = calculateFaceNormal(
                positions[position_indices[i]],
                positions[position_indices[i+1]],
                positions[position_indices[i+2]]
            );
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





