#include <learnopengl/Config.h>

namespace {     // anonymous namespace (encapsulation)
    unsigned int        SCR_WIDTH       =  0;
    unsigned int        SCR_HEIGHT      =  0;
    const unsigned int  framerate       = 60;
    bool                fullscreen      =  0;
    int                 vsync           =  0;   // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
    bool                pbo             =  1;   // +10FPS boost when enabled
    bool                paused          =  0;
    unsigned int        msaa            =  4;    // | 0 = no anti-aliasing | 4 = 4xMSAA |
    bool                recording       =  1;

}

namespace Config {
    void SetScreenResolution(int width, int height) {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
    }

    unsigned int        GetScreenWidth() { return SCR_WIDTH; }
    unsigned int        GetScreenHeight() { return SCR_HEIGHT; }
    unsigned int        GetFramerate() { return framerate; }
    bool                GetFullscreen() { return fullscreen; }
    int                 GetVsync() { return vsync; }
    bool                GetPBO() { return pbo; }
    bool                GetPaused() { return paused; }
    unsigned int        GetMSAA() { return msaa; }
    bool                GetRecording() { return recording; }
}