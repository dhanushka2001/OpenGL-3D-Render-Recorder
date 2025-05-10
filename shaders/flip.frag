#version 430 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main() {
    // Flip vertically
    FragColor = texture(screenTexture, vec2(TexCoord.x, 1.0 - TexCoord.y));
}
