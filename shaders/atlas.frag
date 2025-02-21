#version 430 core
in vec2 TexCoord;    // Texture coordinates from vertex shader
out vec4 FragColor;  // Final output color

uniform sampler2D screenTexture;    // The texture to sample from
uniform vec3 textColor;             // The color of the text (usually white or any desired color)

void main()
{
    float alpha = texture(screenTexture, TexCoord).r;   // Sample the texture at given coordinates
    // If the glyph has an alpha value (not transparent), render it
    if (alpha < 0.1) {
        discard; // Avoid rendering transparent parts of the glyph
    }
    // Apply the text color to the glyph
    FragColor = vec4(textColor, alpha); // Set the text color, using alpha for transparency
}