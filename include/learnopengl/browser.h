#pragma once

#include <Ultralight/Ultralight.h>

class Browser {
public:
    bool init(int width, int height, const char* url);

    void update();
    void render();

    unsigned char* getPixels();
    void unlockPixels();

    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    ultralight::RefPtr<ultralight::Renderer> renderer;
    ultralight::RefPtr<ultralight::View> view;

    int width;
    int height;
};
