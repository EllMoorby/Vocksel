#version 430 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vPos;
in float vWorldHeight;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform vec3 grassColor;
uniform vec3 dirtColor;
uniform vec3 rockColor;

uniform float snowHeight;
uniform float snowTransition;


void main() {
    vec3 normal = normalize(vNormal);

    float slope = abs(normal.y);

    vec3 snowColor = vec3(1.0, 1.0, 1.0);

    float snowIntensity = smoothstep(snowHeight, snowHeight + snowTransition, vWorldHeight);

    vec3 snowBlendedColor = mix(grassColor, snowColor, snowIntensity);

    vec3 terrainColor;
    if (slope > 0.8) {
        terrainColor = snowBlendedColor;
    } else if (slope > 0.5) {
        float t = (slope - 0.5) / 0.4;
        terrainColor = mix(dirtColor, snowBlendedColor, t);
    } else {
        float t = (slope - 0.3) / 0.3;
        terrainColor = mix(rockColor, dirtColor, t);
    }

    vec3 lightDir = normalize(lightPos - vPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambientColor + diffuse) * terrainColor;
    FragColor = vec4(result, 1.0);
}
