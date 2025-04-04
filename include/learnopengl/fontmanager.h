#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <learnopengl/font.h>
#include <map>

class FontManager {
private:
    std::map<std::string, Font*> fonts;
public:
    ~FontManager();
    
    bool loadFont(const std::string& name, int size);
    Font* getFont(const std::string& name);
};

#endif // FONTMANAGER_H
