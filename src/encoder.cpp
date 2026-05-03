#include <iomanip>
#include <iostream>
#include <learnopengl/Settings.h>
#include <learnopengl/encoder.h>

#include <thread>

// glad & GLFW
// -----------
#include <GLFW/glfw3.h> // GLFW (includes stdint.h)
#include <cstring>
#include <glad/glad.h> // glad
#include <learnopengl/timer.h>

Encoder::Encoder() { av_log_set_level(AV_LOG_INFO); }

std::string ffmpegErrStr(int err) {
    char buf[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(err, buf, sizeof(buf));
    return std::string(buf);
}

const AVCodec *Encoder::chooseEncoder() {
    using namespace Settings;
    const AVCodec *codec = nullptr;

    if (libx264) {
        codec = avcodec_find_encoder_by_name("libx264");
        if (!codec) {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cerr << "[Encoder] ERROR: libx264 encoder not "
                         "found\n";
        }
    } else {
        codec = avcodec_find_encoder_by_name("h264_mf");
        if (!codec) {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cerr << "[Encoder] ERROR: h264_mf encoder not "
                         "found\n";
            std::cout << "[Encoder] Checking for libx264 instead...\n";
            codec = avcodec_find_encoder(AV_CODEC_ID_H264);
            if (codec)
                libx264 = 1;
        }
    }

    return codec;
}

bool Encoder::initialize(const char *filename, double recordingStartTime) {
    using namespace Settings;
    // std::lock_guard<std::mutex> lock(encoderMutex);
    startTime = recordingStartTime;

    // 1. Create format context
    avformat_alloc_output_context2(&formatCtx, nullptr, "mp4", filename);
    if (!formatCtx)
        return false;

    const AVCodec *codec = chooseEncoder();
    if (!codec)
        return false;

    // 2. Create VIDEO stream
    videoStream = avformat_new_stream(formatCtx, codec);
    if (!videoStream)
        return false;

    // 3. Setup video codec
    codecCtx = avcodec_alloc_context3(codec);
    codecCtx->width = SCR_WIDTH;
    codecCtx->height = SCR_HEIGHT;
    int FPS = static_cast<int>(framerate);
    codecCtx->time_base = {1, FPS * 1000};
    codecCtx->framerate = {FPS, 1};
    codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    // add global header
    if (formatCtx->oformat->flags & AVFMT_GLOBALHEADER)
        codecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    AVDictionary *opts = nullptr;

    if (strcmp(codec->name, "libx264") == 0) {
        codecCtx->bit_rate = 0;
        av_dict_set(&opts, "preset", g_preset.c_str(), 0);
        av_dict_set(&opts, "crf", g_crf.c_str(), 0);
        av_dict_set(&opts, "tune", "zerolatency", 0);

        std::ostringstream oss, oss2;
        oss << "[Encoder] using preset: '" << g_preset << "'\n";
        oss2 << "[Encoder] using crf: '" << g_crf << "'\n";
        {
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << oss.str();
            std::cout << oss2.str();
        }
    } else if (strcmp(codec->name, "h264_mf") == 0) {
        codecCtx->bit_rate = g_bit_rate;
        codecCtx->gop_size = g_gop_size;
        codecCtx->max_b_frames = g_max_b_frames;
    }

    if (avcodec_open2(codecCtx, codec, &opts) < 0)
        return false;

    avcodec_parameters_from_context(videoStream->codecpar, codecCtx);
    videoStream->time_base = codecCtx->time_base;

    // 4. Setup audio codec
    const AVCodec *audioCodec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if (!audioCodec) {
        std::cerr << "AAC encoder not found\n";
        return false;
    }

    // 5. Create AUDIO stream
    audioStream = avformat_new_stream(formatCtx, nullptr);
    if (!audioStream)
        return false;

    audioCodecCtx = avcodec_alloc_context3(audioCodec);
    if (!audioCodecCtx)
        return false;

    // add global header
    if (formatCtx->oformat->flags & AVFMT_GLOBALHEADER)
        audioCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    audioPts = 0;
    audioBuffer.clear();
    audioCodecCtx->sample_rate = 48000;
    av_channel_layout_default(&audioCodecCtx->ch_layout, 2);
    // audioCodecCtx->channels = 2;
    audioCodecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP; // AAC expects this usually
    // audioCodecCtx->sample_fmt = audioCodec->sample_fmts[0];
    audioCodecCtx->bit_rate = 128000;
    audioStream->time_base = {1, audioCodecCtx->sample_rate};

    if (avcodec_open2(audioCodecCtx, audioCodec, nullptr) < 0) {
        std::cerr << "Failed to open audio codec\n";
        return false;
    }
    avcodec_parameters_from_context(audioStream->codecpar, audioCodecCtx);

    // allocate frame
    audioFrame = av_frame_alloc();
    if (!audioFrame)
        return false;
    audioFrame->format = audioCodecCtx->sample_fmt;
    // audioFrame->channel_layout =
    // audioCodecCtx->channel_layout;
    av_channel_layout_copy(&audioFrame->ch_layout, &audioCodecCtx->ch_layout);
    audioFrame->sample_rate = audioCodecCtx->sample_rate;
    // audioFrame->nb_samples = audioCodecCtx->frame_size;
    audioFrame->nb_samples =
        audioCodecCtx->frame_size > 0 ? audioCodecCtx->frame_size : 1024;

    if (av_frame_get_buffer(audioFrame, 0) < 0) {
        std::cerr << "Failed to av_frame_get_buffer(audioFrame, 0)\n";
        return false;
    }

    if (!(formatCtx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&formatCtx->pb, filename, AVIO_FLAG_WRITE) < 0) {
            std::cerr << "Failed to avio_open(...)\n";
            return false;
        }
    }

    if (avformat_write_header(formatCtx, nullptr) < 0) {
        std::cerr << "Failed to write header\n";
        return false;
    }

    frameX = av_frame_alloc();
    frameX->format = AV_PIX_FMT_YUV420P;
    frameX->width = SCR_WIDTH;
    frameX->height = SCR_HEIGHT;

    if (av_frame_get_buffer(frameX, 32) < 0 || !frameX->data[0]) {
        std::cerr << "Failed to av_frame_get_buffer(frameX, 32)\n";
        return false;
    }

    swsCtx = sws_getContext(SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_RGB24, SCR_WIDTH,
                            SCR_HEIGHT, AV_PIX_FMT_YUV420P, SWS_BILINEAR,
                            nullptr, nullptr, nullptr);

    return swsCtx != nullptr;
}

bool Encoder::encodeFrame(const uint8_t *rgbData, float currentTime) {
    using namespace Settings;
    // std::lock_guard<std::mutex> lock(encoderMutex); // use a
    // mutex just in case as encodeFrame() can be called from
    // main thread and encoder thread (but shouldn't be at the
    // same time)
    if (!frameX || !frameX->data[0] || !swsCtx)
        return false;

    if (!videoStarted) {
        videoStarted = true;
    }

    uint8_t *inData[1] = {(uint8_t *)rgbData};
    int inLinesize[1] = {3 * static_cast<int>(SCR_WIDTH)};
    sws_scale(swsCtx, inData, inLinesize, 0, SCR_HEIGHT, frameX->data,
              frameX->linesize);

    frameX->pts = static_cast<int64_t>((currentTime - startTime) *
                                       videoStream->time_base.den);

    if (avcodec_send_frame(codecCtx, frameX) < 0)
        return false;

    AVPacket *pkt = av_packet_alloc();

    while (avcodec_receive_packet(codecCtx, pkt) == 0) {
        pkt->stream_index = videoStream->index;

        av_interleaved_write_frame(formatCtx, pkt);

        av_packet_unref(pkt); // reuse packet memory
    }

    av_packet_free(&pkt);

    return true;
}

bool Encoder::encodeAudio(const float *samples, int nb_samples) {
    if (!audioCodecCtx || !audioFrame)
        return false;

    // ONE-TIME INIT: offset audio PTS to match when audio actually started
    // relative to when video/media started
    if (!audioOffsetInitialized && mediaStartTime > 0.0 &&
        audioStartTime > 0.0) {

        double offsetSec = audioStartTime - mediaStartTime;
        double now = glfwGetTime();
        // double offsetSec = now - startTime;

        // LOG EVERYTHING
        fprintf(stderr, "[AUDIO SYNC DEBUG]\n");
        fprintf(stderr, "  startTime        = %.6f\n", startTime);
        fprintf(stderr, "  glfwGetTime()    = %.6f\n", now);
        fprintf(stderr, "  offsetSec        = %.6f\n", offsetSec);
        fprintf(stderr, "  audioStartTime   = %.6f\n", audioStartTime);
        fprintf(stderr, "  mediaStartTime   = %.6f\n", mediaStartTime);
        fprintf(stderr, "  audioPts will be = %" PRId64 " samples (%.3f sec)\n",
                (int64_t)(offsetSec * audioCodecCtx->sample_rate), offsetSec);
        fflush(stderr);

        if (offsetSec < 0.0)
            offsetSec = 0.0;

        int64_t offsetSamples =
            (int64_t)(offsetSec * audioCodecCtx->sample_rate);

        // Fill the gap with silent frames instead of just setting PTS
        int64_t silencePts = 0;
        const int frameSize =
            audioCodecCtx->frame_size > 0 ? audioCodecCtx->frame_size : 1024;

        while (silencePts + frameSize <= offsetSamples) {
            // Zero out the audio frame (silence)
            av_frame_make_writable(audioFrame);
            memset(audioFrame->data[0], 0, frameSize * sizeof(float)); // left
            memset(audioFrame->data[1], 0, frameSize * sizeof(float)); // right
            audioFrame->pts = silencePts;
            silencePts += frameSize;

            if (avcodec_send_frame(audioCodecCtx, audioFrame) < 0)
                break;

            AVPacket *pkt = av_packet_alloc();
            while (avcodec_receive_packet(audioCodecCtx, pkt) == 0) {
                pkt->stream_index = audioStream->index;
                av_packet_rescale_ts(pkt, audioCodecCtx->time_base,
                                     audioStream->time_base);
                av_interleaved_write_frame(formatCtx, pkt);
                av_packet_unref(pkt);
            }
            av_packet_free(&pkt);
        }

        audioPts = silencePts; // continue from where silence left off
        audioOffsetInitialized = true;
    }

    const int channels = 2;
    const int frameSize =
        audioCodecCtx->frame_size > 0 ? audioCodecCtx->frame_size : 1024;

    // 1. Buffer incoming interleaved samples
    audioBuffer.insert(audioBuffer.end(), samples,
                       samples + nb_samples * channels);

    // 2. Encode while we have enough for one AAC frame
    while ((int)audioBuffer.size() >= frameSize * channels) {
        float *left = (float *)audioFrame->data[0];
        float *right = (float *)audioFrame->data[1];

        // 3. Copy one full frame
        for (int i = 0; i < frameSize; ++i) {
            left[i] = audioBuffer[i * 2 + 0];
            right[i] = audioBuffer[i * 2 + 1];
        }

        // 4. Set correct PTS (in audio samples)
        // ALWAYS monotonic audio timeline
        audioFrame->pts = audioPts;
        audioPts += frameSize;

        // 5. Remove consumed samples
        audioBuffer.erase(audioBuffer.begin(),
                          audioBuffer.begin() + frameSize * channels);

        // 6. Send to encoder
        if (avcodec_send_frame(audioCodecCtx, audioFrame) < 0)
            return false;

        AVPacket *pkt = av_packet_alloc();

        while (avcodec_receive_packet(audioCodecCtx, pkt) == 0) {
            pkt->stream_index = audioStream->index;

            // Rescale from codec time_base to stream time_base explicitly
            av_packet_rescale_ts(pkt, audioCodecCtx->time_base,
                                 audioStream->time_base);

            // fprintf(stderr,
            //         "[PKT] audio pkt->pts before rescale = %" PRId64
            //         ", stream time_base = %d/%d, codec time_base = %d/%d\n",
            //         pkt->pts, audioStream->time_base.num,
            //         audioStream->time_base.den, audioCodecCtx->time_base.num,
            //         audioCodecCtx->time_base.den);
            // fflush(stderr);

            if (av_interleaved_write_frame(formatCtx, pkt) < 0) {
                av_packet_free(&pkt);
                return false;
            }
            av_packet_unref(pkt);
        }
        av_packet_free(&pkt);
    }
    return true;
}

void Encoder::finalize() {
    // std::lock_guard<std::mutex> lock(encoderMutex);

    // video flush
    if (!codecCtx)
        return;

    avcodec_send_frame(codecCtx, nullptr);

    AVPacket *pkt = av_packet_alloc();

    while (avcodec_receive_packet(codecCtx, pkt) == 0) {
        pkt->stream_index = videoStream->index;

        av_interleaved_write_frame(formatCtx, pkt);

        av_packet_unref(pkt); // reuse packet memory
    }

    av_packet_free(&pkt);

    // audio flush
    if (audioCodecCtx) {
        avcodec_send_frame(audioCodecCtx, nullptr);

        AVPacket *pkt = av_packet_alloc();

        while (avcodec_receive_packet(audioCodecCtx, pkt) == 0) {
            pkt->stream_index = audioStream->index;

            av_packet_rescale_ts(pkt, audioCodecCtx->time_base,
                                 audioStream->time_base);

            av_interleaved_write_frame(formatCtx, pkt);

            av_packet_unref(pkt); // reuse packet memory
        }

        av_packet_free(&pkt);
    }

    // Reset audio sync state for next recording
    audioOffsetInitialized = false;
    audioPts = 0;
    audioBuffer.clear();
    audioStartTime = -1.0;
    mediaStartTime = -1.0;
    videoStarted = false;
    // isEncoding = false; // if not already set by caller

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

void Encoder::start(GLFWwindow *window, GLFWwindow *sharedContextWindow) {
    using namespace Settings;
    Timer::init();

    encoderThread = std::thread([this, window, sharedContextWindow]() {
        std::chrono::high_resolution_clock::time_point t;
        glfwMakeContextCurrent(sharedContextWindow); // Make encoder's context
                                                     // current here
        gladLoadGL(); // Needed again in this thread!

        try {
            // keep encoder thread running even if shutting down
            // or if frame still in queue
            while (!frameQueue.empty() || !audioQueue.empty() ||
                   !shuttingDown) {
                std::unique_lock<std::mutex> qlock(queueMutex);

                // Let encoder thread sleep, wait(lock,
                // predicate), wait until predicate returns true
                queueCond.wait(qlock, [&]() {
                    return recording || !frameQueue.empty() || shuttingDown;
                });

                // exit cleanly only if shutting down, recording
                // stopped and queue empty
                if (shuttingDown && !recording && frameQueue.empty())
                    break;

                // go back to top of while loop if not recording
                // and queue empty (e.g. recording turned off)
                if (!recording && frameQueue.empty())
                    continue;

                // Step 1: If recording is ON and not currently
                // encoding, then start encoding
                if (recording && !isEncoding) {
                    std::string filename = getTimestampedFilename();

                    // Clear any old frames from previous instance
                    while (!frameQueue.empty())
                        frameQueue.pop();
                    while (!audioQueue.empty())
                        audioQueue.pop();

                    if (!this->initialize(filename.c_str(), glfwGetTime())) {
                        std::cerr << "[encoderThread] ERROR: "
                                     "Failed to "
                                     "initialize encoder\n";
                        recording = false; // prevent infinite retry loop
                        continue;
                    }

                    isEncoding = true;
                }

                // Step 2: While recording or queue still has
                // frames, encode frames
                qlock.unlock();
                while (!frameQueue.empty() || !audioQueue.empty() ||
                       recording) {

                    AudioData audioData;
                    FrameData videoData;

                    bool doAudio = false;
                    bool doVideo = false;

                    {
                        std::lock_guard<std::mutex> vlock(queueMutex);
                        std::lock_guard<std::mutex> alock(audioMutex);

                        if (!frameQueue.empty()) {
                            videoData = frameQueue.front();
                            frameQueue.pop();
                            doVideo = true;
                        } else if (!audioQueue.empty() && videoStarted) {
                            audioData = std::move(audioQueue.front());
                            audioQueue.pop();
                            doAudio = true;
                        }

                        // if (!frameQueue.empty() && !audioQueue.empty()) {
                        //     if (frameQueue.front().pts <=
                        //         audioQueue.front().pts) {
                        //         videoData = frameQueue.front();
                        //         frameQueue.pop();
                        //         doVideo = true;
                        //     } else {
                        //         audioData = std::move(audioQueue.front());
                        //         audioQueue.pop();
                        //         doAudio = true;
                        //     }
                        // }
                    }

                    if (doAudio) {
                        encodeAudio(audioData.samples.data(),
                                    audioData.nb_samples);
                    } else if (doVideo) {
                        encodeFrame(laptop_mode ? videoData.frameVec.data()
                                                : videoData.framePtr,
                                    videoData.pts);
                    } else {
                        // CRITICAL: prevent 100% CPU spin
                        std::this_thread::sleep_for(
                            std::chrono::milliseconds(1));
                    }
                }
                // Step 3: Finalize when done recording and queue
                // empty std::cout << "[encoderThread] Finalizing
                // encoder in encoderThread inside outer while
                // loop\n";
                this->finalize();
                isEncoding = false;
            }
        } catch (const std::exception &e) {
            std::cerr << "[encoderThread] ERROR: Encoder thread "
                         "crashed: "
                      << e.what() << std::endl;
        }
        // This MUST always run
        glfwMakeContextCurrent(nullptr);
        if (sharedContextWindow) {
            // std::cout << "[encoderThread] Destroying shared
            // context window\n";
            glfwDestroyWindow(sharedContextWindow);
        }
    });
}

void Encoder::pushFrame(uint8_t *frame, double timestamp, size_t DATA_SIZE) {
    using namespace Settings;
    std::lock_guard<std::mutex> lock(queueMutex);

    if (mediaStartTime < 0.0) {
        mediaStartTime = glfwGetTime(); // video defines timeline start
        fprintf(stderr,
                "[VIDEO PUSH] First frame pushed at wallTime=%.6f, "
                "startTime=%.6f, delta=%.6f\n",
                mediaStartTime, startTime, mediaStartTime - startTime);
        fflush(stderr);
    }

    if (laptop_mode) {
        std::vector<uint8_t> copiedFrame(
            frame,
            frame + DATA_SIZE); // deep copy of original raw frame
        frameQueue.push(FrameData{std::move(copiedFrame),
                                  timestamp}); // move into FrameData so
                                               // Encoder takes ownership
    } else {
        // move into FrameData so Encoder takes ownership
        frameQueue.push(FrameData{frame, timestamp});
    }
    queueCond.notify_all();
}

void Encoder::pushAudio(const float *samples, int nb_samples, int64_t pts) {
    if (!isEncoding || !audioCodecCtx)
        return;

    // Capture when audio actually starts
    if (audioStartTime < 0.0) {
        audioStartTime = glfwGetTime();
        fprintf(stderr,
                "[AUDIO PUSH] First audio pushed at wallTime=%.6f, "
                "startTime=%.6f, delta=%.6f\n",
                audioStartTime, startTime, audioStartTime - startTime);
        fflush(stderr);
    }

    std::lock_guard<std::mutex> lock(audioMutex);

    std::vector<float> copy(samples,
                            samples + nb_samples * 2); // stereo assumed

    audioQueue.push({std::move(copy), nb_samples, pts});
}

void Encoder::stop() {
    using namespace Settings;
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        // std::cout << "[Encoder] Setting shuttingDown to
        // true\n";
        shuttingDown = true;
    }
    // std::cout << "[Encoder] Notfying all threads...\n";
    queueCond.notify_all(); // Wake up the thread so it can exit
    {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        // std::cout << "[Encoder] Exiting program, waiting for
        // encoder to finish...\n";
    }
    if (encoderThread.joinable()) {
        // std::cout << "[Encoder] Trying to join encoder thread
        // with main thread...\n";
        encoderThread.join();
        // std::cout << "[Encoder] Encoder thread joined with
        // main thread\n";
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
    oss << "../output/" << std::put_time(&localTime, "%Y.%m.%d - %H.%M.%S")
        << ".mp4";
    return oss.str();
}

// Flip the frame vertically
void Encoder::flipFrameVertically(unsigned char *frame) {
    using namespace Settings;

    const size_t rowSize = SCR_WIDTH * 3;
    std::vector<unsigned char> tempRow(rowSize);

    for (unsigned int y = 0; y < SCR_HEIGHT / 2; ++y) {
        unsigned char *row = frame + y * rowSize;
        unsigned char *oppositeRow = frame + (SCR_HEIGHT - 1 - y) * rowSize;

        // Swap rows
        std::memcpy(tempRow.data(), row, rowSize);
        std::memcpy(row, oppositeRow, rowSize);
        std::memcpy(oppositeRow, tempRow.data(), rowSize);
    }
}
