#include <learnopengl/browser.h>
#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Platform.h>
#include <AppCore/AppCore.h>
#include <cstdio>

using namespace ultralight;

bool Browser::init(int w, int h, const char* url) {
    width = w;
    height = h;

    printf("[browser] STEP 1: create renderer\n");

    renderer = ultralight::Renderer::Create();

    printf("[browser] STEP 2: create view\n");

    ultralight::ViewConfig view_config;
    view_config.is_accelerated = false; // CPU path
    view_config.enable_javascript = true;

    view = renderer->CreateView(width, height, view_config, nullptr);

    printf("[browser] STEP 3: load URL\n");

    view->LoadURL(url);

    return true;
}

void Browser::update() {
    renderer->Update();
}

void Browser::render() {
    renderer->RefreshDisplay(0);
    renderer->Render();
}

unsigned char* Browser::getPixels() {
    ultralight::Surface* surface = view->surface();
    if (!surface) return nullptr;

    auto* bitmap_surface = (ultralight::BitmapSurface*)surface;
    ultralight::RefPtr<ultralight::Bitmap> bitmap = bitmap_surface->bitmap();

    return (unsigned char*)bitmap->LockPixels();
}

void Browser::unlockPixels() {
    ultralight::Surface* surface = view->surface();
    if (!surface) return;

    auto* bitmap_surface = (ultralight::BitmapSurface*)surface;
    ultralight::RefPtr<ultralight::Bitmap> bitmap = bitmap_surface->bitmap();

    bitmap->UnlockPixels();
}

void Browser::fireMouseEvent(const ultralight::MouseEvent& evt)
{
    if (view)
        view->FireMouseEvent(evt);
}

void Browser::fireKeyEvent(const ultralight::KeyEvent& evt) {
    if (view)
        view->FireKeyEvent(evt);
}

void Browser::fireScrollEvent(const ultralight::ScrollEvent& evt) {
    if (view)
        view->FireScrollEvent(evt);
}

void Browser::GoBack() {
    if (view)
        view->GoBack();
}

void Browser::GoForward() {
    if (view)
        view->GoForward();
}
