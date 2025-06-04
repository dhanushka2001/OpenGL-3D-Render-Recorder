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

class FFmpegEncoder {
public:
    FFmpegEncoder();
    ~FFmpegEncoder();

    bool initialize(const char* filename, double recordingStartTime);
    bool encodeFrame(const uint8_t* rgbData, float currentTime);
    void finalize();

private:
    std::mutex encoderMutex;
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
    FFmpegEncoder(const FFmpegEncoder&) = delete;
    FFmpegEncoder& operator=(const FFmpegEncoder&) = delete;
};

#endif // ENCODER_H
