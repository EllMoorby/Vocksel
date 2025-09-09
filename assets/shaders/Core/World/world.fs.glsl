#version 430 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vPos;


uniform vec3 lightPos = vec3(100.0, 200.0, 100.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 ambientColor = vec3(0.2, 0.2, 0.2);

void main() {
    vec3 normal = normalize(vNormal);

    float slope = abs(normal.y);

    vec3 grassColor = vec3(0.2, 0.6, 0.1);
    vec3 dirtColor = vec3(0.5, 0.4, 0.2);
    vec3 rockColor = vec3(0.4, 0.4, 0.4);
    vec3 snowColor = vec3(0.9, 0.9, 0.9);

    vec3 terrainColor;
    if (slope > 0.8) {
        terrainColor = grassColor;
    } else if (slope > 0.5) {
        float t = (slope - 0.5) / 0.4;
        terrainColor = mix(dirtColor, grassColor, t);
    } else if (slope > 0.3) {
        float t = (slope - 0.3) / 0.3;
        terrainColor = mix(rockColor, dirtColor, t);
    } else {
        float t = slope / 0.3;
        terrainColor = mix(snowColor, rockColor, t);
    }

    vec3 lightDir = normalize(lightPos - vPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambientColor + diffuse) * terrainColor;
    FragColor = vec4(result, 1.0);
}
