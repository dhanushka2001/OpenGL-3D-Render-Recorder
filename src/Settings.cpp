#include <learnopengl/Settings.h>

namespace Settings {
    // possibly add sub-namespaces like Screen, Camera, etc.

    unsigned int            SCR_WIDTH       =  0;
    unsigned int            SCR_HEIGHT      =  0;
    constexpr unsigned int  CHANNEL_COUNT   =  3;
    unsigned int            framerate       = 60;  // recording 30fps, 60fps, etc.
    bool                    fullscreen      =  0;
    int                     vsync           =  0;  // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
    bool                    pbo             =  0;
    constexpr unsigned int  PBO_COUNT       =  2;
    bool                    paused          =  0;
    bool                    msaa            =  1;  // | 0 = no anti-aliasing | 1 = 4xMSAA |
    bool                    recording       =  1;
    bool                    flip_shader     =  0;
    bool                    wireframe       =  0;
    bool                    imgui           =  1;
    TextTriState            currentTextMode = TextTriState::TextAndAtlasON;
    bool                    libx264         =  1;  // | 0 = h264_mf | 1 = libx264 | libx264 is a software encoder optimized for size and quality (can produce same as h264_mf for half the size). h264_mf is a hardware encoder designed for speed and low CPU use.
    // h264_mf
    const int64_t           g_bit_rate      =  30'000'000;  // 30 Mbps
    const int               g_gop_size      =  5;
    const int               g_max_b_frames  =  3;
    // libx264
    const std::string       g_preset        = "ultrafast";
    const std::string       g_crf           = "23";
}
