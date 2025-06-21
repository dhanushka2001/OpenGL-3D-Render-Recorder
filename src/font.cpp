#include <glad/glad.h>
#include <learnopengl/font.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <cstring>

// FreeType
// --------
#ifdef _WIN32
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

Font::Font(const std::string& name, int size) {
    loadFont(name, size);
    createTextureAtlas();

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Font::~Font() {
    glDeleteTextures(1, &textureAtlasID);
}

void Font::loadFont(const std::string& name, int size) {
    std::cout << "[Font] Constructor called for " << name << ", size " << size << "\n";
    std::cout << "[Font] Loading font...\n";

    // LOAD FONT
    // ---------
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "[Font] Could not initialize FreeType Library" << std::endl;
        return;
    }
    std::string fontPath = "../assets/fonts/" + name + ".ttf";
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "[Font] Failed to load font face from: " << fontPath << std::endl;
        return;
    }
    if (!face) {
        std::cerr << "[Font] Failed to load the font face. Ensure the file path is correct." << std::endl;
        return;
    }
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        std::cerr << "[Font] Failed to set Unicode character map." << std::endl;
        return;
    }
    // Set the pixel size for glyphs
    if (FT_Set_Pixel_Sizes(face, 0, size)) {
        std::cerr << "[Font] ERROR::FREETYPE: Failed to set pixel size." << std::endl;
        return;
    }
    else {
        std::cout << "[Font] FreeType successfully loaded font!" << std::endl;
        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        return;
    }
}

void Font::createTextureAtlas() {
    // CREATE TEXTURE ATLAS
    // --------------------
    // Calculate texture atlas size (simplified)
    atlasWidth = 512;
    atlasHeight = 512;
    std::cout << "[Font] Creating texture atlas with size: " << atlasWidth << "x" << atlasHeight << "\n";

    // Variables for positioning glyphs in the atlas
    int offsetX         = 0;
    int offsetY         = 0;
    int padding         = 1;
    int maxRowHeight    = 0;
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
        std::cerr << "[Font] OpenGL Error after glTexImage2D: " << error << std::endl;
        return;
    }
    // Set texture filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Iterate over all printable ASCII characters
    for (unsigned char c = 32; c < 127; ++c) {
        // std::cout << "[Font] Processing character: " << c << "\n";
        unsigned int glyphIndex = FT_Get_Char_Index(face, c);
        if (glyphIndex == 0) {
            std::cerr << "[Font] WARNING: Character not found in font: " << c << " (" << static_cast<int>(c) << ")" << std::endl;
            continue;
        }
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "[Font] WARNING: Failed to load character: " << c << " (" << static_cast<int>(c) << ")" << std::endl;
            continue;
        }
        FT_GlyphSlot g = face->glyph;
        if (g->bitmap.buffer == nullptr || g->bitmap.width == 0 || g->bitmap.rows == 0) {
            // std::cerr << "Warning: Glyph '" << c << "' has no valid bitmap data!" << std::endl;
            // continue; // Skip this character (comment out to allow for spaces)
        }
        if (glyphs.find(c) != glyphs.end()) {
            std::cerr << "[Font] ERROR: Character " << c << " already exists in glyph map!" << std::endl;
            break;
        }
        // Check if character doesn't fit in the row
        if (offsetX + static_cast<int>(g->bitmap.width) > atlasWidth) {
            maxWidth = std::max(maxWidth, offsetX);
            offsetY += maxRowHeight + padding; // next row + padding
            std::cout << "[Font] Reached atlas width limit: " << offsetX << " + " << g->bitmap.width << " = " << offsetX + static_cast<int>(g->bitmap.width) << " > " << atlasWidth << ". Starting new row. Height: " << offsetY << "/" << atlasHeight << "\n";
            offsetX = 0;        // reset
            maxRowHeight = 0;   // reset
        }
        // Check if character doesn't fit in the atlas
        if (offsetY + static_cast<int>(g->bitmap.rows) > atlasHeight) {
            std::cerr << "[Font] ERROR: Exceeded atlas height! " << offsetY << " + " << g->bitmap.rows << " = " << offsetY + static_cast<int>(g->bitmap.rows) << " > " << atlasHeight << ". Texture atlas too small!" << std::endl;
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
        // float scale = fontSize / (float) face->units_per_EM;
        // float ascenderPx = face->ascender * scale;
        // float descenderPx = face->descender * scale;

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
        maxRowHeight = std::max(maxRowHeight, static_cast<int>(g->bitmap.rows));

        // std::cout << "Loaded character: " << c << " (" << static_cast<int>(c) << ") Asc: " << ascenderPx << " Desc: " << descenderPx << std::endl;
        // std::cout << "Successfully loaded glyph: " << c << std::endl;
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);

    wastedArea = atlasWidth * atlasHeight - totalglyphArea;
    minWastedArea = wastedArea - (atlasHeight*(atlasWidth-maxWidth)) - (maxWidth*(atlasHeight-offsetY));
    
    std::cout << "[Font] Texture atlas created: " << atlasWidth << "x" << atlasHeight
            << " | Wasted area: " << wastedArea*100/(atlasWidth*atlasHeight) << "%"
            << " | Minimum size: " << maxWidth << "x" << offsetY
            << " | Minimum wasted area: " << minWastedArea*100/(maxWidth*offsetY) << "%"
            << "\n";

    // Format output in columns: https://stackoverflow.com/a/49295288
    // for (const auto& [key, glyph] : glyphs) {
    //     std::cout.precision(5);
    //     // std::cout << std::fixed;
    //     std::cout << " | " << "Glyph: "                     << static_cast<char>(key)   << " | "
    //                        << "TextureX: "  << std::setw(9) << glyph.textureX           << " | "
    //                        << "TextureY: "  << std::setw(9) << glyph.textureY           << " | "
    //                        << "Width: "     << std::setw(2) << glyph.width              << " | "
    //                        << "Height: "    << std::setw(2) << glyph.height             << " | "
    //                        << "OffsetX: "   << std::setw(2) << glyph.offsetX            << " | "
    //                        << "OffsetY: "   << std::setw(2) << glyph.offsetY            << " | "
    //                        << "AdvanceX: "  << std::setw(5) << (glyph.advanceX >> 6)    << " | "
    //                        << std::endl;
    // }
}

const Glyph& Font::getGlyph(char c) const {
    auto it = glyphs.find(c);
    if (it != glyphs.end()) {
        return it->second;
    }

    static Glyph fallback {};  // a default zero glyph
    std::cerr << "[Font] WARNING: Glyph not found for character '" << c << "' (" << static_cast<int>(c) << ")" << std::endl;
    return fallback;
}
