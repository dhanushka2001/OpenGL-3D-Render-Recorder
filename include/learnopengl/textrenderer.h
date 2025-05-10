#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glad/glad.h>
#include <learnopengl/fontmanager.h>
#include <learnopengl/shader_s.h>

class TextRenderer {
private:
    FontManager& fontManager;
    GLuint textVAO, textVBO;
    GLuint dummyVAO;
    Shader textShader;
    Shader atlasShader;
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
public:
    TextRenderer(FontManager& fm);
    ~TextRenderer();
    
    void renderText(const std::string& text, float x, float y, float scale, glm::vec3 color, const std::string& fontName);
    void renderAtlas(const std::string &fontName);
};

#endif // TEXTRENDERER_H
