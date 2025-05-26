#include <learnopengl/encoder.h>
#include <learnopengl/Settings.h>
#include <iostream>                 // for std::cin/cout/cerr
#include <mutex>
#include <cstdint>                  // C++-friendly version of stdint.h

// FFmpeg
// ------
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavutil/log.h>
#include <x264/x264.h>
}

namespace Encoder {
    std::mutex encoderMutex;

    namespace {     // anonymous namespace (encapsulation)
        // FFmpeg
        // ------
        AVFormatContext* formatCtx = nullptr;
        AVCodecContext* codecCtx = nullptr;
        AVStream* videoStream = nullptr;
        SwsContext* swsCtx = nullptr;
        AVFrame* frameX = nullptr;
        AVPacket pkt = {};  // Zero-initialize the struct
        // std::ofstream logFile;
    }

    // Function to initialize FFmpeg encoder
    bool initializeEncoder(const char* filename) {
        using namespace Settings;
        // Set up ffmpeg's log callback
        // av_log_set_callback(custom_ffmpeg_log_callback);

        // Optional: Set log level (AV_LOG_DEBUG=full logs, AV_LOG_INFO=default, AV_LOG_WARNING=only warnings, AV_LOG_ERROR=only errors)
        // std::cout << "[Encoder] ";
        av_log_set_level(AV_LOG_INFO);

        avformat_alloc_output_context2(&formatCtx, nullptr, "mp4", filename);
        if (!formatCtx) {
            printf("[Encoder] ERROR: Could not create output context\n");
            return false;
        }

        // Find the H.264 encoder
        // void* it = nullptr;
        const AVCodec* codec = nullptr;
        // while ((codec = av_codec_iterate(&it))) {
        //     if (av_codec_is_encoder(codec))
        //         printf("[Encoder] Encoder available: %s (%s)\n", codec->name, codec->long_name ? codec->long_name : "no description");
        // }
        // codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (libx264) {
            codec = avcodec_find_encoder_by_name("libx264");
            if (!codec) {
                std::cerr << "[Encoder] ERROR: libx264 encoder not found\n";
                return -1;
            }
        } else {
            codec = avcodec_find_encoder_by_name("h264_mf");
            if (!codec) {
                std::cerr << "[Encoder] ERROR: h264_mf encoder not found\n";
                std::cout << "[Encoder] Checking for libx264 instead...\n";
                codec = avcodec_find_encoder(AV_CODEC_ID_H264);
                if (!codec) {
                    std::cerr << "[Encoder] ERROR: libx264 encoder not found\n";
                    return -1;
                }
            }
        }
        // const AVCodec* codec = avcodec_find_encoder_by_name("h264_qsv");
        // const char* preferred_encoders[] = { "h264_qsv", "h264_nvenc", "h264_mf", "libx264" };
        // const AVCodec* codec = nullptr;
        // for (const char* encName : preferred_encoders) {
        //     codec = avcodec_find_encoder_by_name(encName);
        //     if (codec) break;
        // }
        // printf("Using encoder: %s\n", codec->name);

        // Create a new stream
        videoStream = avformat_new_stream(formatCtx, codec);
        if (!videoStream) {
            printf("[Encoder] ERROR: Could not create video stream\n");
            return false;
        }

        // Set up the codec context
        codecCtx = avcodec_alloc_context3(codec);
        codecCtx->width = SCR_WIDTH;
        codecCtx->height = SCR_HEIGHT;
        // printf("Encoder init: width=%d, height=%d\n", codecCtx->width, codecCtx->height);
        int FPS = static_cast<int>(framerate);
        codecCtx->time_base = (AVRational){1, FPS * 1000};  // Frame rate
        codecCtx->framerate = (AVRational){FPS, 1};
        codecCtx->pix_fmt = AV_PIX_FMT_YUV420P; // good for libx264 (software encoder)
        // codecCtx->pix_fmt = AV_PIX_FMT_NV12; // good for h264_mf (hardware encoder)
        
        AVDictionary* opts = nullptr;
        
        // libx264: software encoder (optimized for size and quality)
        if (strcmp(codec->name, "libx264") == 0) {
            codecCtx->bit_rate = 0; // Ignore bitrate, use CRF instead
            av_dict_set(&opts, "preset", g_preset.c_str(), 0);
            av_dict_set(&opts, "crf", g_crf.c_str(), 0);
            av_dict_set(&opts, "tune", "zerolatency", 0);
            // Other libx264-specific options here
        }

        // h264_mf: hardware encoder (optimized for speed and low CPU use)
        else if (strcmp(codec->name, "h264_mf") == 0) {
            codecCtx->bit_rate = g_bit_rate;  // 30 Mbps fixed bitrate
            codecCtx->gop_size = g_gop_size;
            codecCtx->max_b_frames = g_max_b_frames;
            // For hardware encoder, CRF is usually unsupported or ignored
            // You can set buffer sizes if needed:
            // codecCtx->rc_max_rate = codecCtx->bit_rate;
            // codecCtx->rc_buffer_size = codecCtx->bit_rate;
        }

        if (avcodec_open2(codecCtx, codec, &opts) < 0) {
            printf("[Encoder] ERROR: Could not open codec\n");
            return false;
        }

        // Assign codec to stream
        videoStream->codecpar->codec_id = AV_CODEC_ID_H264;
        avcodec_parameters_from_context(videoStream->codecpar, codecCtx);

        // Make sure that your AVStream has the same time_base
        videoStream->time_base = codecCtx->time_base;

        // Open output file
        if (!(formatCtx->oformat->flags & AVFMT_NOFILE)) {
            if (avio_open(&formatCtx->pb, filename, AVIO_FLAG_WRITE) < 0) {
                printf("[Encoder] ERROR: Could not open output file\n");
                return false;
            }
        }

        // Write header
        if (avformat_write_header(formatCtx, nullptr) < 0) {
            std::cerr << "[Encoder] ERROR: Error writing header to output file!" << std::endl;
            return false;  // Handle failure
        }

        // Allocate frame and conversion context
        frameX = av_frame_alloc();
        if (!frameX) {
            std::cerr << "[Encoder] ERROR: Could not allocate frame\n";
            return false;
        }
        frameX->format = AV_PIX_FMT_YUV420P;
        // frameX->format = AV_PIX_FMT_NV12;
        frameX->width = SCR_WIDTH;
        frameX->height = SCR_HEIGHT;

        // Allocate frame buffer
        if (av_frame_get_buffer(frameX, 32) < 0) {
            std::cerr << "[Encoder] ERROR: Failed to allocate frame buffer for frameX\n";
            return false;
        }
        if (!frameX->data[0]) {
            std::cerr << "[Encoder] ERROR: frameX->data[0] is still null after get_buffer!\n";
            return false;
        }

        swsCtx = sws_getContext(SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_RGB24,
                                SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_YUV420P,
                                // SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_NV12,
                                SWS_BILINEAR, nullptr, nullptr, nullptr);

        return true;
    }

    // Encode frame using FFmpeg
    bool encodeFrame(const uint8_t* rgbData, float crntTime) {
        // av_frame_unref(frameX);  // Unref previous frame
        using namespace Settings;

        // Ensure frameX->data is valid
        if (!frameX->data[0]) {
            std::cerr << "[Encoder] ERROR: frameX->data[0] is null before sws_scale!\n";
            return false;
        }

        // Convert RGB to YUV420P
        uint8_t* inData[1] = {(uint8_t*)rgbData};  // Input RGB data
        int inLinesize[1] = {3 * static_cast<int>(SCR_WIDTH)};          // Input stride
        
        if (!swsCtx) {
            std::cerr << "[Encoder] ERROR: swsCtx is null!\n";
            return false;
        }
        // Scale the image to YUV420P  
        sws_scale(swsCtx, inData, inLinesize, 0, SCR_HEIGHT, frameX->data, frameX->linesize);
        
        // Check if the frame was properly scaled
        if (frameX->data[0] == nullptr) {
            std::cerr << "[Encoder] ERROR: After sws_scale, frameX->data[0] is null!\n";
            return false;
        }

        // frameX->pts = static_cast<int64_t>(crntTime * 60 * 1000); //* AV_TIME_BASE;
	    frameX->pts = static_cast<int64_t>(crntTime * framerate * 1000);
        
        // Encode the frame
        if (avcodec_send_frame(codecCtx, frameX) < 0) {
            printf("[Encoder] ERROR: Error sending frame to encoder\n");
            return false;
        }

        // Retrieve the encoded packet
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

    // Finalize encoding and close files
    void finalizeEncoder() {
        avcodec_send_frame(codecCtx, nullptr);
        while (avcodec_receive_packet(codecCtx, &pkt) == 0) {
            pkt.stream_index = videoStream->index;
            av_interleaved_write_frame(formatCtx, &pkt);
            av_packet_unref(&pkt);
        }

        // Close log file at the end
        // if (logFile.is_open())
        // {
        //     logFile.close();
        // }

        av_write_trailer(formatCtx);
        avcodec_free_context(&codecCtx);
        avformat_free_context(formatCtx);
        sws_freeContext(swsCtx);
        av_frame_free(&frameX);
    }

}
