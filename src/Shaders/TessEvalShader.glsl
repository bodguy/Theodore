#version 410 core

layout(triangles, equal_spacing, cw) in;
in vec3 tcPosition[];
uniform mat4 Modelview;

void main()
{
    vec3 p0 = gl_TessCoord.x * tcPosition[0];
    vec3 p1 = gl_TessCoord.y * tcPosition[1];
    vec3 p2 = gl_TessCoord.z * tcPosition[2];
    gl_Position = Modelview * vec4(normalize(p0 + p1 + p2), 1);
}