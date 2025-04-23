#ifndef CONFIG_H    // #include guard
#define CONFIG_H

namespace Config {
    void SetScreenResolution(int width, int height);
    unsigned int        GetScreenWidth();
    unsigned int        GetScreenHeight();
    unsigned int        GetFramerate();
    bool                GetFullscreen();
    int                 GetVsync();
    bool                GetPBO();
    bool                GetPaused();
    unsigned int        GetMSAA();
    bool                GetRecording();
}

#endif /* CONFIG_H */