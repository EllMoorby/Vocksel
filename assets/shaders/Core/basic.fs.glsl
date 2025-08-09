#version 430 core
in vec2 TexCoord;
in vec3 Normal;
out vec4 FragColor;

uniform sampler2D textureAtlas;

void main() {
    FragColor = texture(textureAtlas, TexCoord);
}