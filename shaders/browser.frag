#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D browserTexture;

void main() {
    FragColor = texture(browserTexture, TexCoord);
}
