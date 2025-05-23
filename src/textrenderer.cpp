#include <glad/glad.h>
#include <learnopengl/textrenderer.h>
#include <learnopengl/shader_s.h>
#include <learnopengl/Settings.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

TextRenderer::TextRenderer(FontManager &fm)
    : fontManager(fm),
      textShader("text_shader.vert", "text_shader.frag"),
      atlasShader("atlas.vert", "atlas.frag")
{
    // set up vertex data (and buffer(s)) and configure vertex attributes for text
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);

    glBindVertexArray(textVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    
    // Position and texture attribute
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // You must bind a VAO even if you don't use vertex attributes for full-screen atlas
    glGenVertexArrays(1, &dummyVAO);
}

TextRenderer::~TextRenderer()
{
    textShader.Delete();
    atlasShader.Delete();
    glDeleteVertexArrays(1, &textVAO);
    glDeleteVertexArrays(1, &dummyVAO);
    glDeleteBuffers(1, &textVBO);
}

// multiple draw calls for each character in text
void TextRenderer::renderText(const std::string &text, float x, float y, float scale, glm::vec3 color, const std::string &fontName) {
    using namespace Settings;

    Font *font = fontManager.getFont(fontName);
    if (!font) {
        std::cerr << "[TextRenderer] Font " << fontName << " not found.\n";
        return;
    }

    int atlasWidth = font->getAtlasWidth();
    int atlasHeight = font->getAtlasHeight();

    // Use text rendering shader
    textShader.use();
    textShader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas());
    textShader.setInt("textTextureAtlas", 0);

    // Enable 2D rendering
    glEnable(GL_BLEND); // enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up the transformation matrix for the text position
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
    textShader.setMat4("projection", projection);

    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);

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

// single draw call (~10x faster)
void TextRenderer::renderTextFast(const std::string &text, float x, float y, float scale, glm::vec3 color, const std::string &fontName) {
    using namespace Settings;

    Font *font = fontManager.getFont(fontName);
    if (!font) {
        std::cerr << "[TextRenderer] ERROR: Font '" << fontName << "' not found!\n";
        return;
    }

    int atlasWidth = font->getAtlasWidth();
    int atlasHeight = font->getAtlasHeight();

    // Use text rendering shader
    textShader.use();
    textShader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas());
    textShader.setInt("textTextureAtlas", 0);

    // Enable 2D rendering
    glEnable(GL_BLEND); // enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up the transformation matrix for the text position
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
    textShader.setMat4("projection", projection);

    std::vector<float> vertexData;
    vertexData.reserve(text.size() * 6 * 4);

    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);

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

        float quad[6][4] = {
        // positions          // texture coords
            { xpos,     ypos + h,   tx,      ty + th }, // Top-left
            { xpos,     ypos,       tx,      ty      }, // Bottom-left
            { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right

            { xpos,     ypos + h,   tx,      ty + th }, // Top-left
            { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right
            { xpos + w, ypos + h,   tx + tw, ty + th }  // Top-right
        };

        for (auto &v : quad)
            vertexData.insert(vertexData.end(), v, v + 4);

        // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);            // ideal for small subset updates
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // better for reallocating and initializing large buffers

        // Render quad
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursor
        x += (glyph.advanceX >> 6) * scale; // Advance in pixels (1/64th units) to next character
    }

    // Upload vertex buffer data in one call
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW);

    // Draw all characters with a single draw call
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexData.size() / 4));

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::renderAtlas(const std::string &fontName) {
    Font *font = fontManager.getFont(fontName);
    if (!font) {
        std::cerr << "[TextRenderer] ERROR: Font '" << fontName << "' not found!\n";
        return;
    }

    atlasShader.use();
    // Enable 2D rendering
    glEnable(GL_BLEND); // enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // bind VAO
    glBindVertexArray(dummyVAO);

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