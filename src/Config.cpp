#include <learnopengl/Config.h>

namespace {     // anonymous namespace (encapsulation)
    unsigned int        SCR_WIDTH       =  0;
    unsigned int        SCR_HEIGHT      =  0;
    unsigned int        framerate       = 60;
    bool                fullscreen      =  0;
    int                 vsync           =  0;   // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
    bool                pbo             =  0;
    const unsigned int  PBO_COUNT       =  2;
    bool                paused          =  0;
    bool                msaa            =  1;    // | 0 = no anti-aliasing | 1 = 4xMSAA |
    bool                recording       =  1;
    bool                flip_shader     =  0;
}

namespace Config {
    void SetScreenResolution(int width, int height) {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
    }
    void TogglePBO() {
        if (pbo)    { pbo = 0; }
        else        { pbo = 1; }
    }
    void ToggleFlipShader() {
        if (flip_shader)    { flip_shader = 0; }
        else                { flip_shader = 1; }
    }
    void ToggleVsync() {
        if (vsync)    { vsync = 0; }
        else          { vsync = 1; }
    }

    unsigned int        GetScreenWidth() { return SCR_WIDTH; }
    unsigned int        GetScreenHeight() { return SCR_HEIGHT; }
    const unsigned int  GetFramerate() { return framerate; }
    bool                GetFullscreen() { return fullscreen; }
    int                 GetVsync() { return vsync; }
    bool                GetPBO() { return pbo; }
    const unsigned int  GetPBO_COUNT() { return PBO_COUNT; }
    bool                GetPaused() { return paused; }
    bool                GetMSAA() { return msaa; }
    bool                GetRecording() { return recording; }
    bool                GetFlipShader() { return flip_shader; }
}
