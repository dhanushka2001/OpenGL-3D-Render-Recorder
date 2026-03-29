#pragma once

#include <glm/glm.hpp>
#include <cef_base.h>
#include <learnopengl/camera.h>

class UltralightBrowser;


struct AppState {
    // Ultralight
    UltralightBrowser* ultralightBrowser;
    glm::vec3 planePoint;
    glm::vec3 planeNormal;
    float quadWidth;
    float quadHeight;
    
    // CEF
    CefRefPtr<SimpleClient> cefClient;
    CefRefPtr<CefBrowser> cefBrowser;
    glm::vec3 cefPlanePoint;
    glm::vec3 cefPlaneNormal;
    float cefQuadWidth;
    float cefQuadHeight;

    // Shared
    glm::mat4* view;
    glm::mat4* projection;

};
