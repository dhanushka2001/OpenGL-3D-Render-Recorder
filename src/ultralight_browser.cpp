#include <learnopengl/ultralight_browser.h>
#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Platform.h>
#include <AppCore/AppCore.h>
#include <cstdio>

using namespace ultralight;

bool UltralightBrowser::init(int w, int h, const char* url) {
    width = w;
    height = h;

    printf("[UltralightBrowser] STEP 1: create renderer\n");

    renderer = ultralight::Renderer::Create();

    printf("[UltralightBrowser] STEP 2: create view\n");

    ultralight::ViewConfig view_config;
    view_config.is_accelerated = false; // CPU path
    view_config.enable_javascript = true;

    view = renderer->CreateView(width, height, view_config, nullptr);

    printf("[UltralightBrowser] STEP 3: load URL\n");

    view->LoadURL(url);

    return true;
}

void UltralightBrowser::update() {
    renderer->Update();
}

void UltralightBrowser::render() {
    renderer->RefreshDisplay(0);
    renderer->Render();
}

unsigned char* UltralightBrowser::getPixels() {
    ultralight::Surface* surface = view->surface();
    if (!surface) return nullptr;

    auto* bitmap_surface = (ultralight::BitmapSurface*)surface;
    ultralight::RefPtr<ultralight::Bitmap> bitmap = bitmap_surface->bitmap();

    return (unsigned char*)bitmap->LockPixels();
}

void UltralightBrowser::unlockPixels() {
    ultralight::Surface* surface = view->surface();
    if (!surface) return;

    auto* bitmap_surface = (ultralight::BitmapSurface*)surface;
    ultralight::RefPtr<ultralight::Bitmap> bitmap = bitmap_surface->bitmap();

    bitmap->UnlockPixels();
}

void UltralightBrowser::fireMouseEvent(const ultralight::MouseEvent& evt) {
    if (view)
        view->FireMouseEvent(evt);
}

void UltralightBrowser::fireKeyEvent(const ultralight::KeyEvent& evt) {
    if (view)
        view->FireKeyEvent(evt);
}

void UltralightBrowser::fireScrollEvent(const ultralight::ScrollEvent& evt) {
    if (view)
        view->FireScrollEvent(evt);
}

void UltralightBrowser::GoBack() {
    if (view)
        view->GoBack();
}

void UltralightBrowser::GoForward() {
    if (view)
        view->GoForward();
}
