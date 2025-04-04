#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <learnopengl/fontmanager.h>
#include <learnopengl/shader_s.h>

class TextRenderer {
private:
    FontManager& fontManager;
    GLuint textVAO, textVBO;
    GLuint quadVAO, quadVBO;
    Shader textShader;
    Shader atlasShader;
public:
    TextRenderer(FontManager& fm);
    
    void renderText(const std::string& text, float x, float y, float scale, glm::vec3 color, const std::string& fontName);
    void renderAtlas(const std::string &fontName);
};

#endif // TEXTRENDERER_H
