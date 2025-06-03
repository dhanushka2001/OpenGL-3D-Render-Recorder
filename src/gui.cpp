// ImGui
// -----
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
// ImPlot
// ------
#include <implot/implot.h>
// Settings
// --------
#include <learnopengl/Settings.h>

namespace GUI {
    namespace { // anonymous namespace (encapsulation) evaluated once at program startup
        constexpr const char* Title = "My name is window, ImGUI window";
        const std::string MainBody = 
            "Hello there adventurer!\n"
            "Click the RIGHT SHIFT key to free the mouse cursor.\n"
            "Use WASD and the mouse to move the camera.\n"
            "Use the SCROLL wheel to zoom in and out.\n"
            "Use the arrow keys and Z/X to move the objects.\n"
            "Hold Q/E to change the opacity of the objects.\n"
            "Click M to toggle wireframe mode.\n"
            "Click N to toggle ImGui/ImPlot.\n"
            "Click B to cycle between fullscreen font texture\n"
            "atlas and text, text only, or all OFF.\n"
            "\nClick P when recording to see the boost in FPS with\n"
            "multithreading and asynchronous read-back PBOs!\n"
            "Click V to toggle Vsync.\n"
            "\nPress R to turn recording ON/OFF.\n"
            "Screen recording is ";
        constexpr const char* HeaderText = "\nEncoder selected:\n";
        constexpr const char* encoderTypes[] = { "h264_mf", "libx264" };
        constexpr const char* framerates[] = { "30", "60" };
        int currentEncoderIdx = Settings::libx264 ? 1 : 0;
        int selectedFramerateIdx = (Settings::framerate == 60) ? 1 : 0;
        constexpr const int num_encoders = IM_ARRAYSIZE(encoderTypes);
        constexpr const char* EncoderInfo = 
            "\nlibx264 is a software encoder optimized for size and\n"
            "quality (can produce same quality as h264_mf for half\n"
            "the size).\nh264_mf is a hardware encoder designed for speed and\n"
            "low CPU use.";
        constexpr const char* ExitText = "\nPress ESC to exit.";
        constexpr const int xpos = 30;
        constexpr const int ypos = 30;
        constexpr const int width = 420;
        bool encoderChanged = false;
        int presetIdx = 0;
    }

    void Init(GLFWwindow *window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    // hot function, used in render loop
    void Render() {
        using namespace Settings;
        int height = Settings::libx264 ? 528 : 550; // new line = +20 height. libx264 height = h264_mf height - 16.
        ImGui::SetNextWindowPos(ImVec2(xpos, ypos), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Once);
        if (encoderChanged) {
            ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
            encoderChanged = false;
        }
        ImGui::Begin(Title);
        ImGui::PushTextWrapPos(); // wrap at window edge
        
        const std::string isRecording = Settings::recording ?
            "ON. You can find the recordings in /build/output/." :
            "OFF. ";
        std::string MainText = MainBody + isRecording;
        ImGui::Text(MainText.c_str());

        ImGui::Text("%s", HeaderText);
        if (recording) ImGui::BeginDisabled();
        {
            // --- Encoder selection ---
            // currentEncoderIdx = libx264 ? 1 : 0;
            // if (ImGui::Combo("Encoder", &currentEncoderIdx, encoderTypes, num_encoders)) {
            //     // User changed selection, update the external flag accordingly
            //     libx264 = (currentEncoderIdx == 1);
            //     // If you want, update other encoder settings or flags here too
            // }

            bool isLibx264 = Settings::libx264;
            bool isH264mf = !Settings::libx264;
            // Custom checkbox logic for h264_mf
            ImGui::Bullet();
            ImGui::SameLine();
            bool h264mfClicked = ImGui::Checkbox("h264_mf", &isH264mf);
            if (h264mfClicked && !Settings::recording && Settings::libx264) {
                Settings::libx264 = false;
                encoderChanged = true;
            }
            // Custom checkbox logic for libx264
            ImGui::Bullet();
            ImGui::SameLine();
            bool libx264Clicked = ImGui::Checkbox("libx264", &isLibx264);
            if (libx264Clicked && !Settings::recording && !Settings::libx264) {
                Settings::libx264 = true;
                encoderChanged = true;
            }

            // --- Encoder-specific settings ---
            if (Settings::libx264) {
                // --- libx264 settings ---
                const char* presets[] = {
                    "ultrafast", "superfast", "veryfast", "faster",
                    "fast", "medium", "slow", "slower"
                };
                // Match g_preset to index
                // static int presetIdx = 0;
                for (int i = 0; i < IM_ARRAYSIZE(presets); ++i) {
                    if (Settings::g_preset == presets[i]) {
                        presetIdx = i;
                        break;
                    }
                }

                ImGui::PushItemWidth(100.0f); // Set desired width in pixels
                if (ImGui::Combo("Preset", &presetIdx, presets, IM_ARRAYSIZE(presets))) {
                    Settings::g_preset = std::string(presets[presetIdx]);
                }

                // Convert crf string to int (only once unless changed)
                static int crf = std::stoi(Settings::g_crf);
                if (ImGui::SliderInt("CRF", &crf, 17, 30)) {
                    Settings::g_crf = std::to_string(crf);
                }

            } else {
                // h264_mf settings
                static int bitRate = Settings::g_bit_rate; // e.g. 10-100 Mbps
                static int gopSize = Settings::g_gop_size; // e.g. 10–120 // smaller GOP=>more I-frames: better seeking/error recovery, faster previewing, but larger file size. for real-time/streaming: GOP = 2 x framerate.
                static int maxBFrames = Settings::g_max_b_frames; // 0–5
                
                ImGui::PushItemWidth(100.0f); // Set desired width in pixels
                int mbps = static_cast<int>(Settings::g_bit_rate / 1'000'000); // Convert to Mbps
                if (ImGui::SliderInt("Bitrate (Mbps)", &mbps, 10, 100, "%d", ImGuiSliderFlags_AlwaysClamp)) {
                    Settings::g_bit_rate = static_cast<int64_t>(mbps) * 1'000'000; // Convert back to bps
                }

                if (ImGui::SliderInt("GOP Size", &gopSize, 10, 120)) {
                    Settings::g_gop_size = gopSize;
                }

                if (ImGui::SliderInt("Max B-frames", &maxBFrames, 0, 5)) {
                    Settings::g_max_b_frames = maxBFrames;
                }
            }

            // --- Framerate dropdown (shared between encoders) ---
            if (ImGui::Combo("Framerate", &selectedFramerateIdx, framerates, IM_ARRAYSIZE(framerates))) {
                Settings::framerate = (selectedFramerateIdx == 0) ? 30 : 60;
            }
        }
        if (recording) ImGui::EndDisabled();

        ImGui::Text("%s", EncoderInfo);
        ImGui::Text("%s", ExitText);
        ImGui::End();

        
        // Show the ImPlot demo window
        #ifdef _WIN32
        ImGui::SetNextWindowPos(ImVec2(1056, 28), ImGuiCond_Once);
        #endif /* _WIN32 */
        #ifdef __linux__
        ImGui::SetNextWindowPos(ImVec2(956, 580), ImGuiCond_Once);
        #endif /* __linux__ */
        ImGui::SetNextWindowSize(ImVec2(544, 556), ImGuiCond_Once);
        if (ImGui::Begin("ImPlot Demo")) {
            ImPlot::ShowDemoWindow();
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Exit() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
    }
}