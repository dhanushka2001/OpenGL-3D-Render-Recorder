#version 430 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D browserTexture;

void main() {
    vec2 uv = vec2(TexCoord.x, 1.0 - TexCoord.y); // flip Y
    FragColor = texture(browserTexture, uv);
}
