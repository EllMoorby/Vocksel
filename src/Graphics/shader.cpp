#include "Vocksel/Graphics/shader.h"

#include <filesystem>

Vocksel::Shader::Shader(): ID_(0) {}

Vocksel::Shader::Shader(const char *vertexPath, const char *fragmentPath): ID_(0) {
    init(vertexPath, fragmentPath);
}

void Vocksel::Shader::init(const char *vertexPath, const char *fragmentPath) {
    if (ID_ != 0) {
        std::cerr << "Shader already initialized. Ignoring init() call." << std::endl;
        return;
    }

    // Otherwise initialise
    std::string vertex_code, fragment_code;
    std::ifstream v_shader_file(vertexPath), f_shader_file(fragmentPath);

    std::stringstream v_shader_stream, f_shader_stream;

    v_shader_stream << v_shader_file.rdbuf();
    f_shader_stream << f_shader_file.rdbuf();

    v_shader_file.close();
    f_shader_file.close();

    vertex_code = v_shader_stream.str();
    fragment_code = f_shader_stream.str();

    const char* vertex_source = vertex_code.c_str();
    const char* fragment_source = fragment_code.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_source, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_source, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID_ = glCreateProgram();

    glAttachShader(ID_, vertex);
    glAttachShader(ID_, fragment);
    glLinkProgram(ID_);
    checkCompileErrors(ID_, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Vocksel::Shader::use() {
    glUseProgram(ID_);
}

void Vocksel::Shader::setMat4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Vocksel::Shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(vec));
}

void Vocksel::Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}

void Vocksel::Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), value ? 1 : 0);
}

void Vocksel::Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}

void Vocksel::Shader::checkCompileErrors(GLuint object, std::string type) {
    int  success;
    char infoLog[512];

    if (object == ID_) {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
    }else {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    }

    if(!success)
    {
        if (object == ID_) {
            glGetProgramInfoLog(object, 512, NULL, infoLog);
        }else {
            glGetShaderInfoLog(object, 512, NULL, infoLog);
        }
        std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

Vocksel::Shader::~Shader() {
    if (ID_ != 0) {
        glDeleteProgram(ID_);
    }
}

