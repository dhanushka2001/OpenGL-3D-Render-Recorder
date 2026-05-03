#pragma once

#include <cef_client.h>
#include <cef_render_handler.h>
// #include <cef_command_line.h>
#include <learnopengl/encoder.h>
#if defined(_WIN32)
CefEnableHighDPISupport();
#endif
#include <cstdint>
#include <vector>
#include <cstring>
#include <miniaudio/miniaudio.h>

class SimpleRenderHandler : public CefRenderHandler {
  public:
    int width, height;
    std::vector<uint8_t> pixelBuffer;
    bool dirty = false;

    SimpleRenderHandler(int w, int h)
        : width(w), height(h), pixelBuffer(w * h * 4, 0) {}

    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override {
        rect = CefRect(0, 0, width, height);
    }

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                 const RectList &dirtyRects, const void *buffer, int w,
                 int h) override {
        // Resize buffer if CEF gives us different dimensions than
        // expected
        if (w != width || h != height) {
            width = w;
            height = h;
            pixelBuffer.resize(w * h * 4);
        }
        memcpy(pixelBuffer.data(), buffer, w * h * 4);
        dirty = true;
    }

    IMPLEMENT_REFCOUNTING(SimpleRenderHandler);
};

// class AudioHandler : public CefAudioHandler {
//   public:
//     AudioHandler(Encoder *enc) : encoder(enc) {}
//
//     bool GetAudioParameters(CefRefPtr<CefBrowser>,
//                             CefAudioParameters &params) override {
//         params.channel_layout = CEF_CHANNEL_LAYOUT_STEREO;
//         params.sample_rate = 48000;
//         params.frames_per_buffer = 480;
//         return true;
//     }
//
//     void OnAudioStreamPacket(CefRefPtr<CefBrowser>, const float **data,
//                              int frames, int64_t pts) override {
//
//         const int channels = 2;
//         // ring buffer (swap for std::deque?)
//         std::vector<float> interleaved(frames * channels);
//
//         for (int f = 0; f < frames; ++f) {
//             interleaved[f * 2 + 0] = data[0][f];
//             interleaved[f * 2 + 1] = data[1][f];
//         }
//
//         encoder->pushAudio(interleaved.data(), frames, pts);
//     }
//
//     void OnAudioStreamStarted(CefRefPtr<CefBrowser>, const CefAudioParameters
//     &,
//                               int) override {}
//
//     void OnAudioStreamStopped(CefRefPtr<CefBrowser>) override {}
//
//     void OnAudioStreamError(CefRefPtr<CefBrowser>, const CefString &)
//     override {
//     }
//
//   private:
//     Encoder *encoder;
//
//     IMPLEMENT_REFCOUNTING(AudioHandler);
// };

class AudioHandler : public CefAudioHandler {
  public:
    AudioHandler(Encoder *enc) : encoder(enc) {
        ma_device_config config =
            ma_device_config_init(ma_device_type_playback);
        config.playback.format = ma_format_f32;
        config.playback.channels = 2;
        config.sampleRate = 48000;
        config.dataCallback = AudioHandler::audioCallback;
        config.pUserData = this;

        if (ma_device_init(nullptr, &config, &device) != MA_SUCCESS) {
            fprintf(stderr, "[AudioHandler] Failed to init playback device\n");
            return;
        }
        ma_device_start(&device);
    }

    ~AudioHandler() { ma_device_uninit(&device); }

    bool GetAudioParameters(CefRefPtr<CefBrowser>,
                            CefAudioParameters &params) override {
        params.channel_layout = CEF_CHANNEL_LAYOUT_STEREO;
        params.sample_rate = 48000;
        params.frames_per_buffer = 480;
        return true;
    }

    void OnAudioStreamPacket(CefRefPtr<CefBrowser>, const float **data,
                             int frames, int64_t pts) override {
        const int channels = 2;

        // Interleave once, use for both paths
        std::vector<float> interleaved(frames * channels);
        for (int f = 0; f < frames; ++f) {
            interleaved[f * 2 + 0] = data[0][f];
            interleaved[f * 2 + 1] = data[1][f];
        }

        // 1. Send to encoder
        encoder->pushAudio(interleaved.data(), frames, pts);

        // 2. Send to speakers via ring buffer
        {
            std::lock_guard<std::mutex> lock(bufferMutex);
            playbackBuffer.insert(playbackBuffer.end(), interleaved.begin(),
                                  interleaved.end());
        }
    }

    void OnAudioStreamStarted(CefRefPtr<CefBrowser>, const CefAudioParameters &,
                              int) override {}
    void OnAudioStreamStopped(CefRefPtr<CefBrowser>) override {}
    void OnAudioStreamError(CefRefPtr<CefBrowser>, const CefString &) override {
    }

  private:
    static void audioCallback(ma_device *device, void *output,
                              const void *input, ma_uint32 frameCount) {
        AudioHandler *self = (AudioHandler *)device->pUserData;
        float *out = (float *)output;
        const int channels = 2;
        const int samplesNeeded = frameCount * channels;

        std::lock_guard<std::mutex> lock(self->bufferMutex);

        int available = (int)self->playbackBuffer.size();
        if (available >= samplesNeeded) {
            // Enough data — copy and consume
            memcpy(out, self->playbackBuffer.data(),
                   samplesNeeded * sizeof(float));
            self->playbackBuffer.erase(self->playbackBuffer.begin(),
                                       self->playbackBuffer.begin() +
                                           samplesNeeded);
        } else {
            // Underrun — output silence to avoid glitches
            memcpy(out, self->playbackBuffer.data(), available * sizeof(float));
            memset(out + available, 0,
                   (samplesNeeded - available) * sizeof(float));
            self->playbackBuffer.clear();
        }
    }

    Encoder *encoder;
    ma_device device;
    std::vector<float> playbackBuffer;
    std::mutex bufferMutex;

    IMPLEMENT_REFCOUNTING(AudioHandler);
};

class SimpleClient : public CefClient, public CefLifeSpanHandler {
  public:
    CefRefPtr<SimpleRenderHandler> renderHandler;
    CefRefPtr<CefBrowser> browser; // set when ready

    // SimpleClient(int w, int h) {
    //     renderHandler = new SimpleRenderHandler(w, h);
    // }

    Encoder *encoder = nullptr;
    CefRefPtr<AudioHandler> audioHandler;

    SimpleClient(int w, int h, Encoder *enc) : encoder(enc) {
        renderHandler = new SimpleRenderHandler(w, h);
        audioHandler = new AudioHandler(encoder);
    }

    CefRefPtr<CefAudioHandler> GetAudioHandler() override {
        return audioHandler;
    }

    CefRefPtr<CefRenderHandler> GetRenderHandler() override {
        return renderHandler;
    }
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

    void OnAfterCreated(CefRefPtr<CefBrowser> b) override {
        browser = b; // now safe to send input
    }

    IMPLEMENT_REFCOUNTING(SimpleClient);
};
