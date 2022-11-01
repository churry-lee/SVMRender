#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 mvp;

out vec3 normal;
out vec2 texCoord;
out vec3 position;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    normal = (transpose(inverse(model)) * vec4(aNormal, 0.0)).xyz;
    texCoord = aTexCoord;
    position = (model * vec4(aPos, 1.0)).xyz;
}
