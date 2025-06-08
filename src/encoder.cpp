#include <learnopengl/encoder.h>
#include <learnopengl/Settings.h>
#include <iostream>

#include <iomanip>
#include <thread>
// glad & GLFW
// -----------
#include <glad/glad.h>                  // glad
#include <GLFW/glfw3.h>                 // GLFW (includes stdint.h)
#include <learnopengl/timer.h>
#include <cstring>

using namespace Settings;

Encoder::Encoder() {
    av_log_set_level(AV_LOG_INFO);
}

Encoder::~Encoder() {
    finalize();  // auto-cleanup if not already called
}

const AVCodec* Encoder::chooseEncoder() {
    const AVCodec* codec = nullptr;

    if (libx264) {
        codec = avcodec_find_encoder_by_name("libx264");
        if (!codec) {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cerr << "[Encoder] ERROR: libx264 encoder not found\n";
        }
    } else {
        codec = avcodec_find_encoder_by_name("h264_mf");
        if (!codec) {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cerr << "[Encoder] ERROR: h264_mf encoder not found\n";
            std::cout << "[Encoder] Checking for libx264 instead...\n";
            codec = avcodec_find_encoder(AV_CODEC_ID_H264);
            if (codec) libx264 = 1;
        }
    }

    return codec;
}

bool Encoder::initialize(const char* filename, double recordingStartTime) {
    // std::lock_guard<std::mutex> lock(encoderMutex);
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
        av_dict_set(&opts, "crf", g_crf.c_str(), 0);
        av_dict_set(&opts, "tune", "zerolatency", 0);
        
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << "[Encoder] using preset: '" << g_preset << "'\n";
        std::cout << "[Encoder] using crf: '" << g_crf << "'\n";
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

bool Encoder::encodeFrame(const uint8_t* rgbData, float currentTime) {
    // std::lock_guard<std::mutex> lock(encoderMutex); // use a mutex just in case as encodeFrame() can be called from main thread and encoder thread (but shouldn't be at the same time)
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

void Encoder::finalize() {
    // std::lock_guard<std::mutex> lock(encoderMutex);
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

void Encoder::start(GLFWwindow *window) {
    Timer::init();
    // std::chrono::high_resolution_clock::time_point t;

    encoderThread = std::thread([this, window]() {
        std::chrono::high_resolution_clock::time_point t;
        GLFWwindow* sharedContextWindow = glfwCreateWindow(1, 1, "", nullptr, window); // Pass window as 5th param = share context
        glfwMakeContextCurrent(sharedContextWindow);  // Make encoder's context current here
        gladLoadGL();  // Needed again in this thread!

        try {
            // keep encoder thread running even if shutting down if frame still in queue
            while (!shuttingDown || !frameQueue.empty()) {
                std::unique_lock<std::mutex> qlock(queueMutex);

                // Let encoder thread sleep, wait(lock, predicate), wait until predicate returns true
                queueCond.wait(qlock, [&]() {
                    return recording || !frameQueue.empty() || shuttingDown;
                });

                // exit cleanly only if shutting down, recording stopped and queue empty
                if (shuttingDown && !recording && frameQueue.empty()) break;

                // go back to top of while loop if not recording and queue empty (e.g. recording turned off)
                if (!recording && frameQueue.empty()) continue;

                // Step 1: If recording is ON and not currently encoding, then start encoding
                if (recording && !isEncoding) {
                    // if (encoder_thread) {
                    if (1) {
                        std::string filename = getTimestampedFilename();

                        while (!frameQueue.empty()) frameQueue.pop();  // Clear any old frames from previous instance

                        // Local instance for this session only
                        // auto encoder = std::make_unique<FFmpegEncoder>();
                        if (!this->initialize(filename.c_str(), glfwGetTime())) {
                            std::cerr << "[encoderThread] ERROR: Failed to initialize encoder\n";
                            continue;
                        }

                        isEncoding = true;
                    }
                }

                // Step 2: While recording or queue still has frames, encode frames
                while (recording || !frameQueue.empty()) {
                    qlock.unlock();  // Unlock while processing
                    if (!frameQueue.empty()) {
                        FrameData frame;
                        {
                            std::lock_guard<std::mutex> qlock(queueMutex);
                            frame = std::move(frameQueue.front());
                            frameQueue.pop();
                        }
                        Timer::startTimer(t);
                        this->encodeFrame(frame.frame, frame.pts);
                        {
                            std::lock_guard<std::mutex> coutLock(coutMutex);
                            Timer::endTimer(Timer::ENCODE, t);
                        }
                    }
                    qlock.lock();  // Relock for condition checks
                }

                // Step 3: Finalize when done recording and queue empty
                this->finalize();
                isEncoding = false;
            }
        } catch (const std::exception& e) {
            std::cerr << "[encoderThread] ERROR: Encoder thread crashed: " << e.what() << std::endl;
        }
    });
}

void Encoder::pushFrame(unsigned char* frame, double timestamp) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        frameQueue.push(FrameData{frame, timestamp});
    }
    queueCond.notify_one();
}

void Encoder::stop() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        shuttingDown = true;
    }
    queueCond.notify_one();  // Wake up the thread so it can exit
    {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << "Exiting program, waiting for encoder to finish...\n";
    }
    if (encoderThread.joinable()) {
        encoderThread.join();
    }
}


std::string Encoder::getTimestampedFilename() {
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    #ifdef _WIN32
    localtime_s(&localTime, &nowTime);
    #else
    localtime_r(&nowTime, &localTime);
    #endif

    std::ostringstream oss;
    oss << "../output/"
        << std::put_time(&localTime, "%Y.%m.%d - %H.%M.%S")
        << ".mp4";
    return oss.str();
}

// Flip the frame vertically
void Encoder::flipFrameVertically(unsigned char* frame) {
    using namespace Settings;
    
    const size_t rowSize = SCR_WIDTH * 3;
    std::vector<unsigned char> tempRow(rowSize);

    for (unsigned int y = 0; y < SCR_HEIGHT / 2; ++y) {
        unsigned char* row = frame + y * rowSize;
        unsigned char* oppositeRow = frame + (SCR_HEIGHT - 1 - y) * rowSize;

        // Swap rows
        std::memcpy(tempRow.data(), row, rowSize);
        std::memcpy(row, oppositeRow, rowSize);
        std::memcpy(oppositeRow, tempRow.data(), rowSize);
    }
}
