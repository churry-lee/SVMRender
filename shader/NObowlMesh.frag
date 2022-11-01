#version 330 core
in vec2 texCoord;

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
    fragColor = vec4(texture(tex, texCoord).rgb, 1.0);
//    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
