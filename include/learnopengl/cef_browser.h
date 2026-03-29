#pragma once

#include <cef_client.h>
#include <cef_render_handler.h>
// #include <cef_command_line.h>
#if defined(_WIN32)
    CefEnableHighDPISupport();
#endif
#include <vector>
#include <cstring>

class SimpleRenderHandler : public CefRenderHandler {
public:
    int width, height;
    std::vector<uint8_t> pixelBuffer;
    bool dirty = false;

    SimpleRenderHandler(int w, int h) 
        : width(w), height(h), pixelBuffer(w * h * 4, 0) {}

    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override {
        rect = CefRect(0, 0, width, height);
    }

    void OnPaint(CefRefPtr<CefBrowser> browser,
                 PaintElementType type,
                 const RectList& dirtyRects,
                 const void* buffer,
                 int w, int h) override {
        // Resize buffer if CEF gives us different dimensions than expected
        if (w != width || h != height) {
            width = w;
            height = h;
            pixelBuffer.resize(w * h * 4);
        }
        memcpy(pixelBuffer.data(), buffer, w * h * 4);
        dirty = true;
    }

    IMPLEMENT_REFCOUNTING(SimpleRenderHandler);
};

class SimpleClient : public CefClient, public CefLifeSpanHandler {
public:
    CefRefPtr<SimpleRenderHandler> renderHandler;
    CefRefPtr<CefBrowser> browser; // set when ready

    SimpleClient(int w, int h) {
        renderHandler = new SimpleRenderHandler(w, h);
    }

    CefRefPtr<CefRenderHandler> GetRenderHandler() override { return renderHandler; }
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

    void OnAfterCreated(CefRefPtr<CefBrowser> b) override {
        browser = b; // now safe to send input
    }

    IMPLEMENT_REFCOUNTING(SimpleClient);
};
