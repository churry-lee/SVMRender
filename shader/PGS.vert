#version 330 core
layout(location = 0) in vec3 vxposit;
uniform vec3 scale;
uniform vec3 translate;

void main() {
    gl_Position = vec4(scale, 1.0) * (vec4(vxposit, 1.0) + vec4(translate, 0.0));
}