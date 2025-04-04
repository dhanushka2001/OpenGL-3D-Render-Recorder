#include <learnopengl/font.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <cstring>

Font::Font(const std::string& name, int size) {

    // LOAD FONT
    // ---------
    FT_Library ft;
    FT_Face face;

    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not initialize FreeType Library" << std::endl;
        return;
    }
    if (!face) {
        std::cerr << "Failed to load the font face. Ensure the file path is correct." << std::endl;
        return;
    }
    if (FT_New_Face(ft, (std::string("C:/WINDOWS/FONTS/") + name + std::string(".TTF")).c_str(), 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        std::cerr << "Failed to set Unicode character map." << std::endl;
        return;
    }
    // Set the pixel size for glyphs
    if (FT_Set_Pixel_Sizes(face, 0, size)) {
        std::cerr << "ERROR::FREETYPE: Failed to set pixel size." << std::endl;
        return;
    }
    else {
        std::cout << "FreeType successfully loaded font!" << std::endl;
        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        return;
    }

    // CREATE TEXTURE ATLAS
    // --------------------

    // Calculate texture atlas size (simplified)
    atlasWidth = 512;
    atlasHeight = 512;

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
        rowHeight = std::max(rowHeight, static_cast<int>(g->bitmap.rows));

        // std::cout << "Loaded character: " << c << " (" << static_cast<int>(c) << ") Asc: " << ascenderPx << " Desc: " << descenderPx << std::endl;
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

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Font::~Font() {
    glDeleteTextures(1, &textureAtlasID);
}

const Glyph& Font::getGlyph(char c) const {
    return glyphs.at(c);
}
