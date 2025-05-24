#include <learnopengl/Settings.h>

namespace Settings {
    // possibly add sub-namespaces like Screen, Camera, etc.

    unsigned int             SCR_WIDTH       =  0;
    unsigned int             SCR_HEIGHT      =  0;
    constexpr unsigned int   CHANNEL_COUNT   =  3;
    unsigned int             framerate       = 30;   // recording 30fps, 60fps, etc.
    bool                     fullscreen      =  0;
    int                      vsync           =  0;   // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
    bool                     pbo             =  0;
    constexpr unsigned int   PBO_COUNT       =  2;
    bool                     paused          =  0;
    bool                     msaa            =  1;   // | 0 = no anti-aliasing | 1 = 4xMSAA |
    bool                     recording       =  1;
    bool                     flip_shader     =  0;
}