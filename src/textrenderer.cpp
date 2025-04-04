#include <learnopengl/textrenderer.h>
#include <learnopengl/shader_s.h>
#include <iostream>

TextRenderer::TextRenderer(FontManager &fm)
    : fontManager(fm),
      textShader("text_shader.vert", "text_shader.frag"),
      atlasShader("atlas.vert", "atlas.frag")
{

    // build and compile our text shader program
    // textShader("text_shader.vert", "text_shader.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);

    // build and compile our fullscreen atlas shader program
    // atlasShader("atlas.vert", "atlas.frag");

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
}

void TextRenderer::renderText(const std::string &text, float x, float y, float scale, glm::vec3 color, const std::string &fontName) {
    Font *font = fontManager.getFont(fontName);
    if (!font) {
        std::cerr << "Font " << fontName << " not found.\n";
        return;
    }

    int atlasWidth = font->getAtlasWidth();
    int atlasHeight = font->getAtlasHeight();

    textShader.use();
    textShader.setVec3("textColor", color);

    glBindVertexArray(textVAO);

    // Iterate through characters
    for (const char &c : text) {
        // Skip characters that do not exist in the glyph map
        // if (glyphs.find(c) == glyphs.end()) {
        //     continue;
        // }

        // Retrieve glyph
        const Glyph &glyph = font->getGlyph(c);

        // Calculate position and size of quad
        float xpos = x + glyph.offsetX * scale;
        float ypos = y + (glyph.offsetY - glyph.height) * scale;
        float w = glyph.width * scale;
        float h = glyph.height * scale;

        // Update VBO
        float tx = glyph.textureX;
        float ty = glyph.textureY; 
        float tw = glyph.width / atlasWidth;
        float th = glyph.height / atlasHeight;

        float vertices[6][4] = {
        // positions          // texture coords
            { xpos,     ypos + h,   tx,      ty + th }, // Top-left
            { xpos,     ypos,       tx,      ty      }, // Bottom-left
            { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right

            { xpos,     ypos + h,   tx,      ty + th }, // Top-left
            { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right
            { xpos + w, ypos + h,   tx + tw, ty + th }  // Top-right
        };

        glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas());
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);            // ideal for small subset updates
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // better for reallocating and initializing large buffers

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursor
        x += (glyph.advanceX >> 6) * scale; // Advance in pixels (1/64th units) to next character
    }

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void TextRenderer::renderAtlas(const std::string &fontName) {
    Font *font = fontManager.getFont(fontName);
    if (!font) {
        std::cerr << "Font " << fontName << " not found.\n";
        return;
    }

    atlasShader.use();
    // Enable 2D rendering
    glEnable(GL_BLEND); // enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // bind VAO
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    // Bind the texture (the texture atlas in this case)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas()); // atlasTextureId is the texture containing the atlas
    atlasShader.setInt("screenTexture", 0);
    glm::vec3 color(1.0f, 1.0f, 1.0f); // White text
    atlasShader.setVec3("textColor", color);

    // Draw the full-screen triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}