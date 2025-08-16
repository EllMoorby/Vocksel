#version 430 core
out vec4 FragColor;

in vec3 vPos;

void main() {
    vec3 color = normalize(abs(vPos));
    FragColor = vec4(color, 1.0);
}
