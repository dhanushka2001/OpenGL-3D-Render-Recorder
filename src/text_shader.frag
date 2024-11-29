#version 430 core

in vec2 TexCoord; // Texture coordinates from the vertex shader
out vec4 FragColor; // Final color of the pixel

uniform sampler2D textTextureAtlas; // The texture atlas containing all the glyphs
uniform vec3 textColor; // The color of the text (usually white or any desired color)

void main()
{
    // Sample the texture atlas at the given coordinates
    // Alpha is stored in the red channel of the texture
    float alpha = texture(textTextureAtlas, TexCoord).r;
    
    // If the glyph has an alpha value (not transparent), render it
    if (alpha < 0.1) {
        discard; // Avoid rendering transparent parts of the glyph
    }
    // Apply the text color to the glyph
    FragColor = vec4(textColor, alpha);           // Set the text color, using alpha for transparency

}