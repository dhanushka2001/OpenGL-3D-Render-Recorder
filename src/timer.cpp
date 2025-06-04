#include <algorithm>
#include <chrono>
#include <array>
#include <iostream>                     // for std::cin/cout/cerr
#ifdef _WIN32
#include <Windows.h>
#endif /* _WIN32 */
#include <learnopengl/timer.h>

namespace Timer {
    namespace {     // anonymous namespace (encapsulation)
        constexpr int NUM_TIMERS = 14;

        const char* names[NUM_TIMERS] = {
            "encodeFrame",
            "Rendering scene",
            "Blitting MSAA to non-MSAA FBO",
            "Rendering text",
            "Rendering ImGui/ImPlot",
            "Blitting non-MSAA FBO to screen",
            "Flip shader",
            "flipFrameVertically",
            "glClientWaitSync",
            "glMapBufferRange",
            "glReadPixels (PBO ON)",
            "glReadPixels (PBO OFF)",
            "queue push+wait (PBO ON)",
            "queue push+wait (PBO OFF)"
        };

        std::array<double, NUM_TIMERS> totalTimes = {};
        std::array<double, NUM_TIMERS> minTimes = {};
        std::array<double, NUM_TIMERS> minTimeCount = {};
        std::array<double, NUM_TIMERS> maxTimes = {};
        std::array<double, NUM_TIMERS> maxTimeCount = {};
        std::array<int, NUM_TIMERS> counts = {};
    }

    void init() {
        minTimes.fill(1e6); // sets every value in the array to 1e6
    }

    void startTimer(std::chrono::high_resolution_clock::time_point& tp) {
        tp = std::chrono::high_resolution_clock::now();
    }
    
    void endTimer(TimerType type, const std::chrono::high_resolution_clock::time_point& start) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        totalTimes[type] += elapsed.count();
        counts[type]++;
    
        constexpr int WARMUP_SAMPLES = 5; // avoid sampling at the beginning when encoding is slow
        if (counts[type] >= WARMUP_SAMPLES) {
            if (elapsed.count() < minTimes[type]) {
                minTimes[type] = elapsed.count();
                minTimeCount[type] = counts[type];
            }
            if (elapsed.count() > maxTimes[type]) {
                maxTimes[type] = elapsed.count();
                maxTimeCount[type] = counts[type];
            }   
        }
        // print timings every cycle (heavily drops fps and spams terminal, only for debugging)
        // if (type == ENCODE) {
        //     std::cout << names[type] << " took " << elapsed.count() << " ms\n";
        // }
    }
    
    void printAverages() {
        std::cout << "[Timer] Printing timings...\n";
        for (int i = 0; i < NUM_TIMERS; ++i) {
            if (counts[i] > 0) {
                #ifdef _WIN32
                SetConsoleOutputCP(CP_UTF8); // needed for ± symbol
                #endif /* _WIN32 */
                if (i == ENCODE) {
                    std::cout << "[Timer] " << names[i] << " took max: " << maxTimes[i] << "ms (" << maxTimeCount[i] << ") min: " << minTimes[i] << "ms (" << minTimeCount[i] << ") (" << counts[i] << " samples)\n";
                }
                // std::cout << names[i] << " avg: " << (totalTimes[i] / counts[i]) <<  " ms ± maxtime: " << maxTimes[i] << " (" << maxTimeCount[i] << ") mintime: " << minTimes[i] << " (" << minTimeCount[i] << ") (" << counts[i] << " samples)\n";
                std::cout << "[Timer] " << names[i] << " took avg: " << (totalTimes[i] / counts[i]) <<  " ms ± " << (maxTimes[i] - minTimes[i]) / 2.0 << "ms (" << counts[i] << " samples)\n";
            }
        }
    }
}