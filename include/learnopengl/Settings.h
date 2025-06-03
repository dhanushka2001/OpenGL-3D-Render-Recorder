#ifndef SETTINGS_H    // #include guard
#define SETTINGS_H

#include <cstdint>
#include <string>
#include <atomic>

namespace Settings {
    // possibly add sub-namespaces like Screen, Camera, etc.

    extern unsigned int             SCR_WIDTH;
    extern unsigned int             SCR_HEIGHT;
    extern unsigned int             framerate;
    extern bool                     fullscreen;
    extern int                      vsync;          // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
    extern bool                     pbo;
    constexpr unsigned int          PBO_COUNT       =  2;
    constexpr unsigned int          CHANNEL_COUNT   =  3;
    extern bool                     paused;
    extern bool                     msaa;           // | 0 = no anti-aliasing | 1 = 4xMSAA |
    extern std::atomic<bool>        recording;
    extern bool                     flip_shader;
    extern bool                     wireframe;
    extern bool                     imgui;
    extern bool                     libx264;
    // h264_mf
    extern int64_t g_bit_rate;
    extern int g_gop_size;
    extern int g_max_b_frames;
    // libx264
    extern std::string g_preset;
    extern std::string g_crf;

    inline void TogglePBO() { pbo = !pbo; }
    inline void ToggleFlipShader() { flip_shader = !flip_shader; }
    inline void ToggleVsync() { vsync = vsync ? 0 : 1; }
    inline void Togglex264() { libx264 = !libx264; }
    inline void ToggleWireframe() { wireframe = !wireframe; }
    inline void ToggleImGui() { imgui = !imgui; }
    inline void SetScreenResolution(unsigned int width, unsigned int height) {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
    }

    enum class TextTriState : uint8_t {
        TextAndAtlasOFF    = 0,
        TextAndAtlasON     = 1,
        TextONAtlasOFF     = 2
    };
    inline void cycleTriState(TextTriState& state) {
        state = static_cast<TextTriState>((static_cast<uint8_t>(state) + 1) % 3);
    }
    extern TextTriState currentTextMode;
}

#endif /* SETTINGS_H */
