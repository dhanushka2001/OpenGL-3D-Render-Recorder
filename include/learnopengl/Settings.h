#ifndef SETTINGS_H    // #include guard
#define SETTINGS_H

#include <cstdint>
#include <string>
#include <atomic>
#include <mutex>
#include <learnopengl/camera.h>

namespace Settings {
    // possibly add sub-namespaces like Screen, Camera, etc.

    extern unsigned int             SCR_WIDTH;
    extern unsigned int             SCR_HEIGHT;
    extern unsigned int             framerate;
    extern bool                     fullscreen;
    extern int                      vsync;          // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
    extern bool                     pbo;
    constexpr std::size_t           PBO_COUNT       =  2;
    constexpr std::size_t           CHANNEL_COUNT   =  3;
    constexpr std::size_t           BUFFER_COUNT    =  5;
    extern bool                     paused;
    extern bool                     msaa;           // | 0 = no anti-aliasing | 1 = 4xMSAA |
    extern std::atomic<bool>        recording;
    extern std::atomic<bool>        encoder_thread;
    extern bool                     flip_shader;
    extern bool                     wireframe;
    extern bool                     imgui;
    extern bool                     recording_once;
    extern bool                     laptop_mode;
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

    // inline std::mutex coutMutex;
    extern std::mutex coutMutex;



    // uniform variables
    // -----------------
    extern float   mixValue;
    extern float   xOffset ;
    extern float   yOffset ;
    extern float   zOffset ;
    extern float   fov     ;
    extern float   camX    ;
    extern float   camY    ;
    extern float   camZ    ;

    // camera
    // ------
    extern Camera camera;
    extern float   lastX;
    extern float   lastY;
    extern double lastXpos;
    extern double lastYpos;
    extern bool    firstMouse;

    // button press
    // ------------
    extern bool pboPressed ;
    extern bool flipPressed;
    extern bool pausePressed;
    extern bool vsyncPressed;
    extern bool f11Pressed  ;
    extern bool wireframePressed;
    extern bool imguiPressed    ;
    extern bool atlasPressed    ;
    extern bool recordPressed   ;
    extern bool encoderPressed  ;

    // window
    // ------
    extern int window_xPos, window_yPos;
    extern int window_width, window_height;
    extern int left, top, right, bottom;

    // viewport
    // --------
    extern int lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY;
}

#endif /* SETTINGS_H */
