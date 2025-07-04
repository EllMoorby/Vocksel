#version 330 core
in vec2 TexCoord;
in vec3 Normal;
out vec4 FragColor;

uniform sampler2D textureAtlas;
uniform bool showNormals = true;

void main() {
    if (showNormals) {
        vec3 normalColor = abs(normalize(Normal)) * 0.5 + 0.5;
        FragColor = vec4(normalColor, 1.0);
    } else {
        FragColor = texture(textureAtlas, TexCoord);
    }
}