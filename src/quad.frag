#version 430 core
out vec4 FragColor;  // Final output color

in vec2 TexCoord;    // Texture coordinates from vertex shader

uniform sampler2D screenTexture;  // The texture to sample from

void main()
{
    FragColor = texture2D(screenTexture, TexCoord);  // Sample the texture at given coordinates
}