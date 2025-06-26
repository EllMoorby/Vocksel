#include "Vocksel/shader.h"

#include <filesystem>

Vocksel::Shader::Shader(): ID(0) {}

Vocksel::Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    init(vertexPath, fragmentPath);
}

void Vocksel::Shader::init(const char *vertexPath, const char *fragmentPath) {
    // If the shader has already initialised
    if (ID != 0) {
        std::cerr << "Shader already initialized. Ignoring init() call." << std::endl;
        return;
    }

    // Otherwise initialise
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile(vertexPath), fShaderFile(fragmentPath);

    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


void Vocksel::Shader::use() {
    glUseProgram(ID);
}

void Vocksel::Shader::setMat4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Vocksel::Shader::checkCompileErrors(GLuint object, std::string type) {
    int  success;
    char infoLog[512];

    if (object == ID) {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
    }else {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    }

    if(!success)
    {
        if (object == ID) {
            glGetProgramInfoLog(object, 512, NULL, infoLog);
        }else {
            glGetShaderInfoLog(object, 512, NULL, infoLog);
        }
        std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

Vocksel::Shader::~Shader() {
    glDeleteProgram(ID);

}

