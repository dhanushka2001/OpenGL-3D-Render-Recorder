#ifndef ENCODER_H
#define ENCODER_H

#include <mutex>
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

#include <queue>
#include <condition_variable>
// glad & GLFW
// -----------
#include <glad/glad.h>                  // glad
#include <GLFW/glfw3.h>                 // GLFW (includes stdint.h)
#include <learnopengl/Settings.h>
#include <thread>


class Encoder {
public:
    Encoder();
    ~Encoder();

    bool initialize(const char* filename, double recordingStartTime);
    bool encodeFrame(const uint8_t* rgbData, float currentTime);
    void finalize();

    void start(GLFWwindow *window);   // start encoder thread
    void pushFrame(unsigned char* frame, double timestamp);
    void stop();    // signals shutdown and joins thread

private:
    // std::mutex encoderMutex;
    std::mutex coutMutex;

    AVFormatContext* formatCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    AVStream* videoStream = nullptr;
    SwsContext* swsCtx = nullptr;
    AVFrame* frameX = nullptr;
    AVPacket pkt = {};

    double startTime = 0.0;

    // Helper
    const AVCodec* chooseEncoder();

    // No copying
    Encoder(const Encoder&) = delete;
    Encoder& operator=(const Encoder&) = delete;


    
    std::string getTimestampedFilename();
    void flipFrameVertically(unsigned char* frame);

    // FrameData holds a copy of the frame and its timestamp
    struct FrameData {
        unsigned char *frame;       // raw pixel data
        double pts;                 // presentation timestamp
    };

    std::queue<FrameData> frameQueue;
    std::mutex queueMutex;
    std::condition_variable queueCond;
    std::atomic<bool> shuttingDown = false;  // To stop the thread on app exit
    std::atomic<bool> isEncoding = false;
    const size_t MAX_QUEUE_SIZE = 8;

    // std::shared_ptr<Encoder> encoder = std::make_shared<Encoder>();

    // pbo settings
    // ------------
    GLuint firstIndex = 0;
    GLuint nextIndex = 1;//(firstIndex + 1) % PBO_COUNT;
    // unsigned int frameCounter = 0;
    GLuint pboIds[Settings::PBO_COUNT];
    GLsync pboFences[Settings::PBO_COUNT] = { nullptr };
    unsigned int DATA_SIZE;

    std::thread encoderThread;
};

#endif // ENCODER_H
