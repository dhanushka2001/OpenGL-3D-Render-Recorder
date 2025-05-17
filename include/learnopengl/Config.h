#ifndef CONFIG_H    // #include guard
#define CONFIG_H

namespace Config {
    void                SetScreenResolution(int width, int height);
    void                TogglePBO();
    void                ToggleFlipShader();
    void                ToggleVsync();
    unsigned int        GetScreenWidth();
    unsigned int        GetScreenHeight();
    const unsigned int  GetFramerate();
    bool                GetFullscreen();
    int                 GetVsync();
    bool                GetPBO();
    const unsigned int  GetPBO_COUNT();
    bool                GetPaused();
    bool                GetMSAA();
    bool                GetRecording();
    bool                GetFlipShader();
}

#endif /* CONFIG_H */