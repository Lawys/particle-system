#version 460 core
out vec4 FragColor;

in vec4 FragPos;

uniform vec3 gPoint;
uniform vec3 dColor;
uniform float dOpacity;

void main()
{
    float dist = distance(gPoint, vec3(FragPos));
	float b = clamp((dist - dColor.x) / 200.0, 0, 1) / 2.0;
    float g = clamp((dist - dColor.y) / 200.0, 0, 1) / 2.0;
    float r = clamp((dist - dColor.z) / 200.0, 0, 1) / 2.0;
    vec3 color = vec3(1.0 - r, 1.0 - g, 1.0 - b);

    dist = distance(gPoint, vec3(FragPos));
    float opacity = clamp((dist - dOpacity) / 400.0, 0, 1) / 1.1;

    FragColor = vec4(color, 1.0 - opacity);
}   