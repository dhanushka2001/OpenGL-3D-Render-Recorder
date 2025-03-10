#ifndef TEXT_H    // #include guard
#define TEXT_H

#include <glad/glad.h>
#include <iomanip> // for std::precision(3)    
#include <map>
#include <vector>
#include <learnopengl/shader_s.h>

// FreeType
// --------
#include <ft2build.h>
#include FT_FREETYPE_H

struct Glyph {
    float textureX, textureY;  // Texture coordinates in the atlas
    float width, height;       // Glyph's width and height
    float offsetX, offsetY;    // Offsets (for positioning)
    unsigned int advanceX;     // Horizontal advance (for spacing)
};

FT_Library ft;
FT_Face face;
GLuint textureAtlasID;
const unsigned int atlasWidth = 512;
const unsigned int atlasHeight = 512;
std::map<char, Glyph> glyphs;          // Store info about each glyph
GLuint textVAO, textVBO;
GLuint quadVAO, quadVBO;
extern int lowerLeftCornerOfViewportX;
extern int lowerLeftCornerOfViewportY;
extern const unsigned int  SCR_WIDTH;
extern const unsigned int  SCR_HEIGHT;

// Default text values
// -------------------
const std::string   DEF_FONT    =     "ARIAL";
const FT_UInt       DEF_SIZE    =     48;
const std::string   DEF_BODY    =     "Hello World!";
const float         DEF_X       =     0.0f;
const float         DEF_Y       =     0.0f;
const float         DEF_SCALE   =     1.0f;
const glm::vec3     WHITE       =     glm::vec3(1.0f, 1.0f, 1.0f);


class Text
{
public:
    // Text Attributes
    std::string Font;
    std::string fontFilepath;
    std::string Body;
    FT_UInt Fontsize;
    float X;
    float Y;
    float Scale;
    glm::vec3 Color;

    // constructor
    Text(const std::string &font = DEF_FONT, FT_UInt fontsize = DEF_SIZE, const std::string &text = DEF_BODY, float x = DEF_X, float y = DEF_Y, float scale = DEF_SCALE, glm::vec3 color = WHITE) :
    Font(font), Body(text), Fontsize(fontsize), X(x), Y(y), Scale(scale), Color(color)  // member initializer list
    {
        // load font and create texture atlas
        // ----------------------------------
        fontFilepath = std::string("C:/WINDOWS/FONTS/") + Font + std::string(".TTF");
        std::ifstream file(fontFilepath);
        if (!file.good()) {
            std::cerr << "ERROR: Font file not found at " << fontFilepath << std::endl;
        }
        loadFont();
        createTextureAtlas();
    }

    // Render text (this function is in the render loop)
    void RenderText(Shader &textShader, const std::string &text, float x, float y, float scale, glm::vec3 color) {
        // Use your text rendering shader
        textShader.use();
        textShader.setVec3("textColor", Color);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureAtlasID);  // Bind the texture atlas
        textShader.setInt("textTextureAtlas", 0);

        // Enable 2D rendering
        glEnable(GL_BLEND); // enable transparency
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set up the transformation matrix for the text position
        glm::mat4 projection = glm::ortho(lowerLeftCornerOfViewportX + 0.0f, lowerLeftCornerOfViewportX + static_cast<float>(SCR_WIDTH), lowerLeftCornerOfViewportY + 0.0f, lowerLeftCornerOfViewportY + static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
        textShader.setMat4("projection", projection);

        glBindVertexArray(textVAO);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);

        // Iterate through characters
        for (const char &c : text) {
            // Skip characters that do not exist in the glyph map
            if (glyphs.find(c) == glyphs.end()) {
                continue;
            }
            // Retrieve glyph
            Glyph &glyph = glyphs[c];

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
            x += (glyph.advanceX >> 6) * scale; // Advance in pixels (1/64th units)
        }

        // Cleanup
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Disable blend mode after rendering the text
        // glDisable(GL_BLEND); // disable transparency
    }

    void RenderAtlas(Shader &atlasShader, GLuint &atlasTexture) {
        atlasShader.use();
        // Enable 2D rendering
        glEnable(GL_BLEND); // enable transparency
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // bind VAO
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    
        // Bind the texture (the texture atlas in this case)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlasTexture); // atlasTextureId is the texture containing the atlas
        atlasShader.setInt("screenTexture", 0);
        glm::vec3 color(1.0f, 1.0f, 1.0f); // White text
        atlasShader.setVec3("textColor", color);
    
        // Draw the full-screen triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
    
        // Cleanup
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Delete() {
        glDeleteVertexArrays(1, &textVAO);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &textVBO);
        glDeleteBuffers(1, &quadVBO);
    }
private:
    // Load the font face (you should have a valid file path to the font)
    bool loadFont() {
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "Could not initialize FreeType Library" << std::endl;
            return false;
        }
        if (FT_New_Face(ft, (std::string("C:/WINDOWS/FONTS/") + Font + std::string(".TTF")).c_str(), 0, &face)) {
            std::cerr << "Failed to load font" << std::endl;
            return false;
        }
        if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
            std::cerr << "Failed to set Unicode character map." << std::endl;
            return false;
        }
        // Set the pixel size for glyphs
        if (FT_Set_Pixel_Sizes(face, 0, Fontsize)) {
            std::cerr << "ERROR::FREETYPE: Failed to set pixel size." << std::endl;
            return false;
        }
        if (!face) {
            std::cerr << "Failed to load the font face. Ensure the file path is correct." << std::endl;
            return false;
        }
        else {
            std::cout << "FreeType successfully loaded font!" << std::endl;
            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            return true;
        }
    }

    // Create texture atlas with all the glyphs
    void createTextureAtlas() {
        // Variables for positioning glyphs in the atlas
        int offsetX         = 0;
        int offsetY         = 0;
        int padding         = 1;
        int rowHeight       = 0;
        // Variables for calculating area used/wasted
        int maxWidth        = 0;
        int totalglyphArea  = 0;
        int wastedArea      = 0;
        int minWastedArea   = 0;

        // int maxAscent, maxDescent = 0;
        glyphs.clear();

        // Create the texture atlas
        glGenTextures(1, &textureAtlasID);
        glBindTexture(GL_TEXTURE_2D, textureAtlasID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error after glTexImage2D: " << error << std::endl;
            return;
        }
        // Set texture filtering and wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Iterate over all printable ASCII characters
        for (unsigned char c = 32; c < 127; ++c) {
            // std::cout << "Processing character: " << c << std::endl;
            unsigned int glyphIndex = FT_Get_Char_Index(face, c);
            if (glyphIndex == 0) {
                std::cerr << "Character not found in font: " << c << " (" << static_cast<int>(c) << ")" << std::endl;
                continue;
            }
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "Failed to load character: " << c << " (" << static_cast<int>(c) << ")" << std::endl;
                continue;
            }
            FT_GlyphSlot g = face->glyph;
            if (g->bitmap.buffer == nullptr || g->bitmap.width == 0 || g->bitmap.rows == 0) {
                // std::cerr << "Warning: Glyph '" << c << "' has no valid bitmap data!" << std::endl;
                // continue; // Skip this character (comment out to allow for spaces)
            }
            if (glyphs.find(c) != glyphs.end()) {
                std::cerr << "Error: Character " << c << " already exists in glyph map!" << std::endl;
                break;
            }
            // Check if character doesn't fit in the row
            if (offsetX + g->bitmap.width > atlasWidth) {
                std::cerr << "REACHED ATLAS WIDTH LIMIT. STARTING NEW ROW. " << offsetX << " + " << g->bitmap.rows << " = " << offsetX + static_cast<int>(g->bitmap.width) << " >= " << atlasWidth << std::endl;
                maxWidth = std::max(maxWidth, offsetX);
                offsetX = 0;
                offsetY += rowHeight + padding;
                rowHeight = 0;
            }
            // Check if character doesn't fit in the atlas
            if (offsetY + g->bitmap.rows > atlasHeight) {
                std::cerr << "Texture atlas too small!" << std::endl;
                break;
            }

            FT_Bitmap &bitmap = face->glyph->bitmap;
            // Flip the bitmap vertically before uploading
            std::vector<unsigned char> flippedBitmap(bitmap.width * bitmap.rows);
            for (int y = 0; y < (int)bitmap.rows; ++y) {
                std::memcpy(
                    &flippedBitmap[y * bitmap.width],
                    &bitmap.buffer[(bitmap.rows - 1 - y) * bitmap.width],
                    bitmap.width);
            }

            // Copy glyph bitmap to the atlas
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            offsetX, offsetY,
                            g->bitmap.width, g->bitmap.rows,
                            GL_RED, GL_UNSIGNED_BYTE, flippedBitmap.data()
            );
            float scale = Fontsize / (float) face->units_per_EM;
            float ascenderPx = face->ascender * scale;
            float descenderPx = face->descender * scale;

            // Store glyph information
            glyphs[c] = Glyph{
                static_cast<float>(offsetX) / (float)atlasWidth,        // TextureX
                static_cast<float>(offsetY) / (float)atlasHeight,       // TextureY
                static_cast<float>(g->bitmap.width),                    // width
                static_cast<float>(g->bitmap.rows),                     // height
                static_cast<float>(g->bitmap_left),                     // OffsetX
                static_cast<float>(g->bitmap_top),                      // OffsetY
                static_cast<unsigned int>(g->advance.x)                 // AdvanceX
            };
            // glyphs[c].offsetY = (ascenderPx - g->bitmap_top);// - (g->bitmap.rows - descenderPx);
            // maxAscent = int(face->ascender * (face->size->metrics.y_scale / 65536.0)) >> 6;
            // maxDescent = int(abs(face->descender * (face->size->metrics.y_scale / 65536.0))) >> 6;
            totalglyphArea += static_cast<int>(g->bitmap.width) * static_cast<int>(g->bitmap.rows);
            offsetX += g->bitmap.width + padding;
            rowHeight = std::max(rowHeight, static_cast<int>(g->bitmap.rows));

            std::cout << "Loaded character: " << c << " (" << static_cast<int>(c) << ") Asc: " << ascenderPx << " Desc: " << descenderPx << std::endl;
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);

        wastedArea = atlasWidth * atlasHeight - totalglyphArea;
        minWastedArea = wastedArea - (atlasHeight*(atlasWidth-maxWidth)) - (maxWidth*(atlasHeight-offsetY));
        
        std::cout << " | Texture atlas created: " << atlasWidth << "x" << atlasHeight
                << " | Wasted area: " << wastedArea*100/(atlasWidth*atlasHeight) << "%"
                << " | Minimum size: " << maxWidth << "x" << offsetY
                << " | Minimum wasted area: " << minWastedArea*100/(maxWidth*offsetY) << "% |"
                << std::endl;

        // Format output in columns: https://stackoverflow.com/a/49295288
        for (const auto& [key, glyph] : glyphs) {
            std::cout.precision(5);
            // std::cout << std::fixed;
            std::cout << " | " << "Glyph: "                     << static_cast<char>(key)   << " | "
                               << "TextureX: "  << std::setw(9) << glyph.textureX           << " | "
                               << "TextureY: "  << std::setw(9) << glyph.textureY           << " | "
                               << "Width: "     << std::setw(2) << glyph.width              << " | "
                               << "Height: "    << std::setw(2) << glyph.height             << " | "
                               << "OffsetX: "   << std::setw(2) << glyph.offsetX            << " | "
                               << "OffsetY: "   << std::setw(2) << glyph.offsetY            << " | "
                               << "AdvanceX: "  << std::setw(5) << (glyph.advanceX >> 6)    << " | "
                               << std::endl;
        }
    }
};
#endif /* TEXT_H */
