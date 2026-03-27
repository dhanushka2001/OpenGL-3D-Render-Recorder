#pragma once

#include <cef_app.h>

class SimpleApp : public CefApp {
public:
    SimpleApp() = default;

    IMPLEMENT_REFCOUNTING(SimpleApp);
};
