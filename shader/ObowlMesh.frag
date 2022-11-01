#version 330 core
in vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D mask;

out vec4 fragColor;

void main()
{
    fragColor = vec4(texture(tex, texCoord).rgb, texture(mask, texCoord).r);
}
