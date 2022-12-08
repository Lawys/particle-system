#version 460 core
in vec4 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec4 FragPos;

void main()
{
    FragPos = aPos;
    gl_Position = projection * view * aPos;
}
