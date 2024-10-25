#version 430 core
layout(location = 0) in vec2 aPos;    // Vertex position (2D quad)
layout(location = 1) in vec2 aTexCoord; // Texture coordinates

out vec2 TexCoord;  // Pass texture coordinates to fragment shader

void main()
{
    gl_Position = vec4(aPos.xy, 0.0, 1.0);  // Set the position of each vertex
    TexCoord = aTexCoord;  // Pass texture coordinates to fragment shader
}