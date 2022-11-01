#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;
uniform mat4 mvp;
out vec3 fColor;

void DrawPGS(vec4 offset, vec3 color) {
   fColor = color;

   gl_Position = mvp * (gl_in[0].gl_Position + offset);
   EmitVertex();
   gl_Position = mvp * (gl_in[1].gl_Position + offset);
   EmitVertex();
   gl_Position = mvp * (gl_in[2].gl_Position + offset);
   EmitVertex();
   EndPrimitive();
}

void main() {
    DrawPGS(vec4(-0.2, 0.0, 0.8, 0.0), vec3(0.2, 1.0, 0.2));
    DrawPGS(vec4(-0.2, 0.0, 1.4, 0.0), vec3(1.0, 1.0, 0.4));
    DrawPGS(vec4(-0.2, 0.0, 2.0, 0.0), vec3(1.0, 0.2, 0.2));
}
