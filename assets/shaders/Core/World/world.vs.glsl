#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vNormal;
out vec3 vPos;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vPos = aPos.xyz;
    gl_Position = projection * view * worldPos;
}