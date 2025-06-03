#include <learnopengl/encoder.h>
#include <learnopengl/Settings.h>
#include <iostream>

using namespace Settings;

FFmpegEncoder::FFmpegEncoder() {
    av_log_set_level(AV_LOG_INFO);
}

FFmpegEncoder::~FFmpegEncoder() {
    finalize();  // auto-cleanup if not already called
}

const AVCodec* FFmpegEncoder::chooseEncoder() {
    const AVCodec* codec = nullptr;

    if (libx264) {
        codec = avcodec_find_encoder_by_name("libx264");
        if (!codec)
            std::cerr << "[Encoder] ERROR: libx264 encoder not found\n";
    } else {
        codec = avcodec_find_encoder_by_name("h264_mf");
        if (!codec) {
            std::cerr << "[Encoder] ERROR: h264_mf encoder not found\n";
            std::cout << "[Encoder] Checking for libx264 instead...\n";
            codec = avcodec_find_encoder(AV_CODEC_ID_H264);
            if (codec) libx264 = 1;
        }
    }

    return codec;
}

bool FFmpegEncoder::initialize(const char* filename, double recordingStartTime) {
    std::lock_guard<std::mutex> lock(encoderMutex);
    startTime = recordingStartTime;

    avformat_alloc_output_context2(&formatCtx, nullptr, "mp4", filename);
    if (!formatCtx) return false;

    const AVCodec* codec = chooseEncoder();
    if (!codec) return false;

    videoStream = avformat_new_stream(formatCtx, codec);
    if (!videoStream) return false;

    codecCtx = avcodec_alloc_context3(codec);
    codecCtx->width = SCR_WIDTH;
    codecCtx->height = SCR_HEIGHT;
    int FPS = static_cast<int>(framerate);
    codecCtx->time_base = {1, FPS * 1000};
    codecCtx->framerate = {FPS, 1};
    codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    AVDictionary* opts = nullptr;

    if (strcmp(codec->name, "libx264") == 0) {
        codecCtx->bit_rate = 0;
        av_dict_set(&opts, "preset", g_preset.c_str(), 0);
        std::cout << "[ffmpeg] using preset: '" << g_preset << "'\n";
        av_dict_set(&opts, "crf", g_crf.c_str(), 0);
        std::cout << "[ffmpeg] using crf: '" << g_crf << "'\n";
        av_dict_set(&opts, "tune", "zerolatency", 0);
    } else if (strcmp(codec->name, "h264_mf") == 0) {
        codecCtx->bit_rate = g_bit_rate;
        codecCtx->gop_size = g_gop_size;
        codecCtx->max_b_frames = g_max_b_frames;
    }

    if (avcodec_open2(codecCtx, codec, &opts) < 0)
        return false;

    avcodec_parameters_from_context(videoStream->codecpar, codecCtx);
    videoStream->time_base = codecCtx->time_base;

    if (!(formatCtx->oformat->flags & AVFMT_NOFILE))
        if (avio_open(&formatCtx->pb, filename, AVIO_FLAG_WRITE) < 0)
            return false;

    if (avformat_write_header(formatCtx, nullptr) < 0)
        return false;

    frameX = av_frame_alloc();
    frameX->format = AV_PIX_FMT_YUV420P;
    frameX->width = SCR_WIDTH;
    frameX->height = SCR_HEIGHT;

    if (av_frame_get_buffer(frameX, 32) < 0 || !frameX->data[0])
        return false;

    swsCtx = sws_getContext(SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_RGB24,
                            SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_YUV420P,
                            SWS_BILINEAR, nullptr, nullptr, nullptr);

    return swsCtx != nullptr;
}

bool FFmpegEncoder::encodeFrame(const uint8_t* rgbData, float currentTime) {
    std::lock_guard<std::mutex> lock(encoderMutex);
    if (!frameX || !frameX->data[0] || !swsCtx) return false;

    uint8_t* inData[1] = {(uint8_t*)rgbData};
    int inLinesize[1] = {3 * static_cast<int>(SCR_WIDTH)};
    sws_scale(swsCtx, inData, inLinesize, 0, SCR_HEIGHT, frameX->data, frameX->linesize);

    frameX->pts = static_cast<int64_t>((currentTime - startTime) * framerate * 1000);

    if (avcodec_send_frame(codecCtx, frameX) < 0)
        return false;

    pkt.data = nullptr;
    pkt.size = 0;
    pkt.dts = frameX->pts;

    while (avcodec_receive_packet(codecCtx, &pkt) == 0) {
        pkt.stream_index = videoStream->index;
        av_interleaved_write_frame(formatCtx, &pkt);
        av_packet_unref(&pkt);
    }

    return true;
}

void FFmpegEncoder::finalize() {
    std::lock_guard<std::mutex> lock(encoderMutex);
    if (!codecCtx) return;

    avcodec_send_frame(codecCtx, nullptr);
    while (avcodec_receive_packet(codecCtx, &pkt) == 0) {
        pkt.stream_index = videoStream->index;
        av_interleaved_write_frame(formatCtx, &pkt);
        av_packet_unref(&pkt);
    }

    av_write_trailer(formatCtx);
    avcodec_free_context(&codecCtx);
    avformat_free_context(formatCtx);
    sws_freeContext(swsCtx);
    av_frame_free(&frameX);

    formatCtx = nullptr;
    codecCtx = nullptr;
    videoStream = nullptr;
    swsCtx = nullptr;
    frameX = nullptr;
}
