#ifndef TIMER_H    // #include guard
#define TIMER_H

#include <chrono>

namespace Timer {
    enum TimerType {
        ENCODE = 0,
        RENDER_SCENE = 1,
        BLIT_MSAA = 2,
        RENDER_TEXT = 3,
        RENDER_GUI = 4,
        BLIT_TO_SCREEN = 5,
        FLIP_SHADER = 6,
        FLIP_FUNCTION = 7,
        GL_CLIENT_WAIT_SYNC = 8,
        GL_MAP_BUFFER_RANGE = 9,
        GLREADPIXELS_PBO_ON = 10,
        GLREADPIXELS_PBO_OFF = 11,
        QUEUE_PUSH_WAIT_PBO_ON = 12,
        QUEUE_PUSH_WAIT_PBO_OFF = 13,
    };

    void init();
    void startTimer(std::chrono::high_resolution_clock::time_point& tp);
    void endTimer(TimerType type, const std::chrono::high_resolution_clock::time_point& start);
    void printAverages();
}

#endif /* TIMER_H */