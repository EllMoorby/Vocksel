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
        Shader(const char* vertex_path, const char* fragment_path);
        Shader(const char* compute_path);
        Shader();
        ~Shader();

        void use();
        void init(const char* vertex_path, const char* fragment_path);
        void initCompute(const char* compute_path);
        void setMat4(const std::string& name, const glm::mat4& matrix) const;
        void setVec3(const std::string& name, const glm::vec3& vector) const;
        void setInt(const std::string& name, int value) const;
        void setBool(const std::string& name, bool value) const;
        void setFloat(const std::string& name, float value) const;

        void reloadShader();


        void dispatchCompute(uint32_t x, uint32_t y, uint32_t z);

    public:
        GLuint ID_ = 0;
    private:
        void cleanUpShader();
        void checkCompileErrors(GLuint object, std::string type);
    private:
        std::string vertex_path_;
        std::string fragment_path_;
        std::string compute_path_;
    };
}
#endif //SHADER_H
