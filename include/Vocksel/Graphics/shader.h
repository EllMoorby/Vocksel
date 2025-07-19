#ifndef SHADER_H
#define SHADER_H
#include "glad/glad.h"
#include <string>

#include "glm/fwd.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

namespace Vocksel {
    class Shader {
    public:
        Shader(const char* vertexPath, const char* fragmentPath);
        Shader();
        ~Shader();

        void use();
        void init(const char* vertexPath, const char* fragmentPath);
        void setMat4(const std::string& name, const glm::mat4& matrix) const;
        void setVec3(const std::string& name, const glm::vec3& vector) const;
        void setInt(const std::string& name, int value) const;
        void setBool(const std::string& name, bool value) const;
    public:
        GLuint ID_;
    private:
        void checkCompileErrors(GLuint object, std::string type);
    };
}
#endif //SHADER_H
