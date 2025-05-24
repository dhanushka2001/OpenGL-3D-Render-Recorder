#ifndef SETTINGS_H    // #include guard
#define SETTINGS_H

namespace Settings {
    // possibly add sub-namespaces like Screen, Camera, etc.

    extern unsigned int             SCR_WIDTH;
    extern unsigned int             SCR_HEIGHT;
    extern const unsigned int       CHANNEL_COUNT;
    extern unsigned int             framerate;
    extern bool                     fullscreen;
    extern int                      vsync;          // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
    extern bool                     pbo;
    extern const unsigned int       PBO_COUNT;
    extern bool                     paused;
    extern bool                     msaa;           // | 0 = no anti-aliasing | 1 = 4xMSAA |
    extern bool                     recording;
    extern bool                     flip_shader;

    inline void TogglePBO() { pbo = !pbo; }
    inline void ToggleFlipShader() { flip_shader = !flip_shader; }
    inline void ToggleVsync() { vsync = vsync ? 0 : 1; }
    inline void SetScreenResolution(unsigned int width, unsigned int height) {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
    }
}

#endif /* SETTINGS_H */