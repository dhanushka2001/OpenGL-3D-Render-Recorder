#include <learnopengl/browser.h>

using namespace ultralight;

bool Browser::init(int w, int h, const char* url) {
    width = w;
    height = h;

    // Create renderer (NO config passed anymore)
    renderer = Renderer::Create();

    // View config (new API requirement)
    ViewConfig view_config;
    view_config.is_accelerated = false; // CPU path

    // Create view (note new signature)
    view = renderer->CreateView(width, height, view_config, nullptr);

    view->LoadURL(url);

    return true;
}

void Browser::update() {
    renderer->Update();
}

void Browser::render() {
    renderer->Render();
}

unsigned char* Browser::getPixels() {
    ultralight::Surface* surface = view->surface();
    if (!surface) return nullptr;

    auto bitmap_surface = (ultralight::BitmapSurface*)surface;
    ultralight::RefPtr<ultralight::Bitmap> bitmap = bitmap_surface->bitmap();

    return (unsigned char*)bitmap->LockPixels();  // <-- THIS is the fix
}

void Browser::unlockPixels() {
    ultralight::Surface* surface = view->surface();
    if (!surface) return;

    auto bitmap_surface = (ultralight::BitmapSurface*)surface;
    bitmap_surface->bitmap()->UnlockPixels();
}
