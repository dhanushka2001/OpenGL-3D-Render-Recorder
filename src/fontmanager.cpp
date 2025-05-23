#include <learnopengl/fontmanager.h>
#include <learnopengl/shader_s.h>
#include <iostream>

FontManager::~FontManager() {
    for (auto& pair : fonts) {
        delete pair.second;
    }
}

bool FontManager::loadFont(const std::string& name, int size) {
    if (fonts.find(name) != fonts.end()) {
        std::cerr << "[FontManager] Font " << name << " already loaded.\n";
        return false;
    }

    fonts[name] = new Font(name, size);
    std::cout << "[FontManager] New font created: " << name << ", size " << size << "\n";

    return true;
}

Font* FontManager::getFont(const std::string& name) {
    if (fonts.find(name) != fonts.end()) {
        return fonts[name];
    }
    return nullptr;
}
