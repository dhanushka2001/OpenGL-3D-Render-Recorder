#include <learnopengl/Settings.h>
#include <atomic>

namespace Settings {
    // possibly add sub-namespaces like Screen, Camera, etc.

    unsigned int            SCR_WIDTH       =  0;
    unsigned int            SCR_HEIGHT      =  0;
    unsigned int            framerate       = 60;  // recording 30fps, 60fps, etc.
    bool                    fullscreen      =  1;
    int                     vsync           =  0;  // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
    bool                    msaa            =  1;  // | 0 = no anti-aliasing | 1 = 4xMSAA |
    std::atomic<bool>       recording       =  0;
    std::atomic<bool>       encoder_thread  =  0;
    bool                    pbo             =  0;
    bool                    paused          =  0;    
    bool                    flip_shader     =  0;
    bool                    wireframe       =  0;
    bool                    imgui           =  1;
    TextTriState            currentTextMode =  TextTriState::TextAndAtlasON;
    bool                    recording_once  =  0;
    bool                    laptop_mode     =  0;
    bool                    libx264         =  1;  // | 0 = h264_mf | 1 = libx264 | libx264 is a software encoder optimized for size and quality (can produce same as h264_mf for half the size). h264_mf is a hardware encoder designed for speed and low CPU use.
    // h264_mf
    int64_t                 g_bit_rate      =  30'000'000;  // 30 Mbps
    int                     g_gop_size      =  5;
    int                     g_max_b_frames  =  3;
    // libx264
    std::string             g_preset        = "ultrafast";
    std::string             g_crf           = "23";
    std::mutex 		        coutMutex;


    // uniform variables
    // -----------------
    float   mixValue    =   0.3f;
    float   xOffset     =   0.0f;
    float   yOffset     =   0.0f;
    float   zOffset     =   0.0f;
    float   fov         =  45.0f;
    float   camX        =   0.0f;
    float   camY        =   0.0f;
    float   camZ        =  -4.5f;

    // camera
    // ------
    Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
    float   lastX       = 0;
    float   lastY       = 0;
    double lastXpos     = 0;
    double lastYpos     = 0;
    bool    firstMouse  = true;

    // button press
    // ------------
    bool pboPressed         = false;
    bool flipPressed        = false;
    bool pausePressed       = false;
    bool vsyncPressed       = false;
    bool f11Pressed         = false;
    bool wireframePressed   = false;
    bool imguiPressed       = false;
    bool atlasPressed       = false;
    bool recordPressed      = false;
    bool encoderPressed     = false;

    // window
    // ------
    int window_xPos, window_yPos = 0;
    int window_width, window_height;
    int left, top, right, bottom;

    // viewport
    // --------
    int lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY = 0;

}
