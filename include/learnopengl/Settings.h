#ifndef SETTINGS_H    // #include guard
#define SETTINGS_H

namespace Settings {
    // possibly add sub-namespaces like Screen, Camera, etc.

    inline unsigned int             SCR_WIDTH       =  0;
    inline unsigned int             SCR_HEIGHT      =  0;
    // struct Screen {
    //     unsigned int        SCR_WIDTH          =  0;
    //     unsigned int        SCR_HEIGHT         =  0;
    // };
    // inline Screen screen;
    inline constexpr unsigned int   CHANNEL_COUNT   =  3;
    inline unsigned int             framerate       = 60;
    inline bool                     fullscreen      =  0;
    inline int                      vsync           =  0;   // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
    inline bool                     pbo             =  0;
    inline constexpr unsigned int   PBO_COUNT       =  2;
    inline bool                     paused          =  0;
    inline bool                     msaa            =  0;   // | 0 = no anti-aliasing | 1 = 4xMSAA |
    inline bool                     recording       =  1;
    inline bool                     flip_shader     =  0;

    inline void TogglePBO() { pbo = !pbo; }
    inline void ToggleFlipShader() { flip_shader = !flip_shader; }
    inline void ToggleVsync() { vsync = vsync ? 0 : 1; }
    inline void SetScreenResolution(unsigned int width, unsigned int height) {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
    }
}

#endif /* SETTINGS_H */