#version 430 core

layout (location = 0) in vec4 vertex; // x, y, z, w -> position and texture coords
out vec2 TexCoord; // Texture coordinates to pass to the fragment shader

uniform mat4 projection; // Projection matrix to transform the text positions

void main()
{
    gl_Position = projection * vec4(vertex.x, vertex.y, 0.0, 1.0); // Apply projection
    TexCoord = vertex.zw; // Set texture coordinates
}