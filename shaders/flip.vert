#version 430 core

out vec2 TexCoord;                          // Pass texture coordinates to fragment shader

void main()
{
    // full-screen triangle: https://stackoverflow.com/a/59739538/7875204
    vec2 vertices[3] = vec2[3](vec2(-1,-1), vec2(3,-1), vec2(-1,3));
    gl_Position = vec4(vertices[gl_VertexID], 0, 1);
    TexCoord = 0.5 * gl_Position.xy + vec2(0.5);
}