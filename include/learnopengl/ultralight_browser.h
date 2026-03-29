#pragma once

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
using namespace ultralight;

class UltralightBrowser {
public:
    bool init(int width, int height, const char* url);

    void update();
    void render();

    unsigned char* getPixels();
    void unlockPixels();

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void fireMouseEvent(const ultralight::MouseEvent& evt);
    void fireKeyEvent(const ultralight::KeyEvent& evt);
    void fireScrollEvent(const ultralight::ScrollEvent& evt);

    void GoBack();
    void GoForward();

private:
    ultralight::RefPtr<ultralight::Renderer> renderer;
    ultralight::RefPtr<ultralight::View> view;

    int width;
    int height;
};
