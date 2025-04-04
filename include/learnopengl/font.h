#ifndef FONT_H
#define FONT_H

#include <map>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Glyph {
    float textureX, textureY; // Position in texture atlas
    float width, height;      // Glyph size
    float offsetX, offsetY;   // Offset from baseline
    unsigned int advanceX;    // Advance to next character
};

class Font {
private:
    std::map<char, Glyph> glyphs;
    GLuint textureAtlasID;
    int atlasWidth, atlasHeight;

public:
    // constructor and destructor
    Font(const std::string& name, int size);
    ~Font();

    // getters
    const Glyph& getGlyph(char c) const;
    GLuint getTextureAtlas() const { return textureAtlasID; }
    int getAtlasWidth() const { return atlasWidth; }
    int getAtlasHeight() const { return atlasHeight; }
};

#endif // FONT_H
