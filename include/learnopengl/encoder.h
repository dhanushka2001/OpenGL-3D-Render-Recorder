#ifndef ENCODER_H
#define ENCODER_H

#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <cstdint>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavutil/log.h>
#ifdef _WIN32
#include <x264/x264.h>
#endif
#ifdef __linux
#include <x264.h>
#endif
}

// glad & GLFW
// -----------
#include <glad/glad.h>                  // glad
#include <GLFW/glfw3.h>                 // GLFW (includes stdint.h)


class Encoder {
public:
    Encoder();
    ~Encoder();

    bool initialize(const char* filename, double recordingStartTime);
    bool encodeFrame(const uint8_t* rgbData, float currentTime);
    void finalize();
    
    void start(GLFWwindow *window);   // start encoder thread
    void pushFrame(uint8_t* frame, double timestamp, size_t DATA_SIZE = 0);
    void stop();    // signals shutdown and joins thread

    void flipFrameVertically(unsigned char* frame);

    std::condition_variable queueCond;
    std::atomic<bool> isEncoding = false;

private:
    AVFormatContext* formatCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    AVStream* videoStream = nullptr;
    SwsContext* swsCtx = nullptr;
    AVFrame* frameX = nullptr;
    AVPacket pkt = {};

    // Helper
    const AVCodec* chooseEncoder();

    // No copying
    Encoder(const Encoder&) = delete;
    Encoder& operator=(const Encoder&) = delete;

    // FrameData holds a copy of the frame and its timestamp
    struct FrameData {
        std::vector<uint8_t> frameVec;   // deep-copied frame
        uint8_t *framePtr = nullptr;     // pointer to RGB pixel data
        double pts;                      // presentation timestamp
        
        // Default constructor (need in order to declare struct instance before assigning values)
        FrameData() = default;

        // Constructor with pointer and pts
        FrameData(uint8_t *framePtr_val, double pts_val)
            : framePtr(framePtr_val), pts(pts_val) {}

        // Move constructor with vector (rvalue reference) and pts
        FrameData(const std::vector<uint8_t> &&frameVec_val, double pts_val)
            : frameVec(std::move(frameVec_val)), pts(pts_val) {}

    };

    double startTime = 0.0;
    std::string getTimestampedFilename();
    std::queue<FrameData> frameQueue;
    std::mutex queueMutex;
    std::atomic<bool> shuttingDown = false;  // To stop the thread on app exit
    const size_t MAX_QUEUE_SIZE = 8;

    std::thread encoderThread;
};

#endif // ENCODER_H
