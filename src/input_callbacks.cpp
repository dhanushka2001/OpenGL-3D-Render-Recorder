#include <learnopengl/input_callbacks.h>
#include <learnopengl/ultralight_browser.h>
#include <learnopengl/cef_browser.h>
#include <learnopengl/app_state.h>
#include <learnopengl/encoder.h>
#include <learnopengl/Settings.h>
#include <glm/glm.hpp>
#include <iostream>

using namespace Settings;

namespace Input {
    // anonymous namespace (encapsulation)
    namespace {
        enum InputMode { INPUT_GAME, INPUT_ULTRALIGHT, INPUT_CEF };

        InputMode inputMode = INPUT_GAME;

        glm::vec3 getMouseRay(float mouseX, float mouseY, glm::mat4 projection,
                              glm::mat4 view) {
            using namespace Settings;

            float x = (2.0f * mouseX) / SCR_WIDTH - 1.0f;
            float y = 1.0f - (2.0f * mouseY) / SCR_HEIGHT;

            glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);
            glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
            ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

            glm::vec3 ray_world =
                glm::normalize(glm::vec3(glm::inverse(view) * ray_eye));

            return ray_world;
        }

        bool intersectRayPlane(glm::vec3 rayOrigin, glm::vec3 rayDir,
                               glm::vec3 planePoint, glm::vec3 planeNormal,
                               glm::vec3 &hitPoint) {
            float denom = glm::dot(rayDir, planeNormal);

            if (abs(denom) < 0.0001f)
                return false;

            float t = glm::dot(planePoint - rayOrigin, planeNormal) / denom;

            if (t < 0)
                return false;

            hitPoint = rayOrigin + t * rayDir;
            return true;
        }

        int glfwToVirtualKey(int key) {
            switch (key) {
            case GLFW_KEY_BACKSPACE:
                return 0x08; // VK_BACK
            case GLFW_KEY_TAB:
                return 0x09; // VK_TAB
            case GLFW_KEY_ENTER:
                return 0x0D; // VK_RETURN
            case GLFW_KEY_ESCAPE:
                return 0x1B; // VK_ESCAPE
            case GLFW_KEY_SPACE:
                return 0x20; // VK_SPACE

            case GLFW_KEY_LEFT:
                return 0x25; // VK_LEFT
            case GLFW_KEY_UP:
                return 0x26; // VK_UP
            case GLFW_KEY_RIGHT:
                return 0x27; // VK_RIGHT
            case GLFW_KEY_DOWN:
                return 0x28; // VK_DOWN

            case GLFW_KEY_DELETE:
                return 0x2E; // VK_DELETE

            case GLFW_KEY_PERIOD:
                return 0xBE; // VK_OEM_PERIOD

            case GLFW_KEY_APOSTROPHE:
                return 0xDE; // VK_OEM_7 (' / @ on UK)
            case GLFW_KEY_2:
                return 0x32; // VK_2 (" on UK)

            default:
                return key; // works for A-Z, 0-9
            }
        }
    } // namespace

    // process all input: query GLFW whether relevant keys are pressed/released
    // this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window, float timeDiff, float crntTime,
                      std::unique_ptr<Encoder> &encoder) {

        // disable game controls (exc. pause) when browser active
        if (inputMode != INPUT_GAME)
            return;

        using namespace Settings; // compile-time instruction (no runtime
                                  // overhead)

        std::ostringstream oss;

        // Exit
        // ----
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
            std::cout << "[main] Esc key pressed. Exiting from program. Time: "
                      << crntTime << "\n";
        }
        // Control crate
        // -------------
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            yOffset += timeDiff * 1.0f;
            if (yOffset >= 200.0f)
                yOffset = 200.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            yOffset -= timeDiff * 1.0f;
            if (yOffset <= -200.0f)
                yOffset = -200.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            xOffset -= timeDiff * 1.0f;
            if (xOffset <= -200.0f)
                xOffset = -200.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            xOffset += timeDiff * 1.0f;
            if (xOffset >= 200.0f)
                xOffset = 200.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            zOffset -= timeDiff * 1.0f;
            if (zOffset <= -200.0f)
                zOffset = -200.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            zOffset += timeDiff * 1.0f;
            if (zOffset >= 200.0f)
                zOffset = 200.0f;
        }
        // Control camera
        // --------------
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            // camY += timeDiff * 5.0f;
            // if(camY >= 20.0f)
            //     camY = 20.0f;
            camera.ProcessKeyboard(DOWN, timeDiff);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            // camY -= timeDiff * 5.0f;
            // if (camY <= -20.0f)
            //     camY = -20.0f;
            camera.ProcessKeyboard(UP, timeDiff);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // camX -= timeDiff * 5.0f;
            // if(camX <= -20.0f)
            //     camX = -20.0f;
            camera.ProcessKeyboard(RIGHT, timeDiff);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // camX += timeDiff * 5.0f;
            // if(camX >= 20.0f)
            //     camX = 20.0f;
            camera.ProcessKeyboard(LEFT, timeDiff);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // camZ -= timeDiff * 5.0f;
            // if(camZ <= -20.0f)
            //     camZ = -20.0f;
            camera.ProcessKeyboard(BACKWARD, timeDiff);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            // camZ += timeDiff * 5.0f;
            // if(camZ >= 20.0f)
            //     camZ = 20.0f;
            camera.ProcessKeyboard(FORWARD, timeDiff);
        }
        // Mix
        // ---
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            mixValue -= timeDiff;
            if (mixValue <= 0.0f)
                mixValue = 0.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            mixValue += timeDiff;
            if (mixValue >= 1.0f)
                mixValue = 1.0f;
        }
        // FOV
        // ---
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
            fov -= timeDiff * 100.0f;
            if (fov <= 0.0f)
                fov = 0.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            fov += timeDiff * 100.0f;
            if (fov >= 180.0f)
                fov = 180.0f;
        }
        // Fullscreen
        // ----------
        if ((glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) && !(f11Pressed)) {
            f11Pressed = true;
            bool last_pause_state = paused;
            float lastYaw = camera.Yaw;
            float lastPitch = camera.Pitch;

            // if not paused, pause screen (to stop mouse drift)
            if (!paused) {
                // lock the mouse (stop YAW drift)
                glfwSetCursorPos(window, SCR_WIDTH / 2.0f,
                                 SCR_HEIGHT /
                                     2.0f); // set cursor in centre of screen to
                                            // remove whiplash cursor jump
                // tell GLFW to uncapture our mouse
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                last_pause_state = 0;
                paused = 1;
            }
            // fullscreen->windowed
            if (fullscreen) {
                // int adjusted_width  = window_width;
                // int adjusted_height = window_height + top;
                glfwSetWindowMonitor(window, NULL, window_xPos, window_yPos,
                                     //  adjusted_width, adjusted_height,
                                     //  window_width, window_height,
                                     SCR_WIDTH, SCR_HEIGHT, GLFW_DONT_CARE);
                fullscreen = 0;
            }
            // windowed->fullscreen
            else {
                glfwGetWindowPos(window, &window_xPos, &window_yPos);
                glfwGetWindowSize(window, &window_width, &window_height);
                glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
                // GLFWmonitor *monitor = glfwGetPrimaryMonitor();
                // const GLFWvidmode *mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0,
                                     //  mode->width, mode->height,      //
                                     //  viewport size accurate to window size
                                     SCR_WIDTH,
                                     SCR_HEIGHT, // viewport fills window
                                     GLFW_DONT_CARE);
                fullscreen = 1;
            }
            // unpause
            if (last_pause_state == 0) {
                // unlock the mouse
                glfwSetCursorPos(window, lastX,
                                 lastY); // set cursor in centre of screen to
                                         // remove whiplash cursor jump
                // tell GLFW to capture our mouse
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                paused = 0;
                camera.Yaw = lastYaw;
                camera.Pitch = lastPitch;
            }
        }
        if ((glfwGetKey(window, GLFW_KEY_F11) == GLFW_RELEASE) &&
            (f11Pressed)) {
            f11Pressed = false;
        }
        // Pause
        // -----
        if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) &&
            (!pausePressed)) {
            pausePressed = 1;

            // pause -> unpause
            if (paused) {
                firstMouse = true;
                lastXpos = static_cast<double>(lastX);
                lastYpos = static_cast<double>(lastY);
                glfwSetCursorPos(window, lastXpos,
                                 lastYpos); // remove whiplash cursor jump
                // tell GLFW to capture our mouse
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                paused = 0;
            }
            // unpause -> pause
            else {
                // tell GLFW to uncapture our mouse
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                paused = 1;
            }
        }
        if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE) &&
            (pausePressed)) {
            pausePressed = 0;
        }
        // PBO
        // ---
        if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && (!pboPressed)) {
            pboPressed = true;
            TogglePBO();
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] P key pressed. Press = " << pboPressed
                      << " Time: " << crntTime << "\n";
            // std::cout << "P key pressed: " << !pbo << "->" << pbo << " Press:
            // " << !pboPressed << "->" << pboPressed << " Time: " << crntTime
            // << "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) && (pboPressed)) {
            pboPressed = false;
            std::cout << "[main] P key released. Press = " << pboPressed
                      << " Time: " << crntTime << "\n";
        }
        // Flip Shader
        // -----------
        if ((glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) && (!flipPressed)) {
            flipPressed = true;
            ToggleFlipShader();
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] F key pressed. Press = " << flipPressed
                      << " Time: " << crntTime << "\n";
            // std::cout << "Toggled PBO: " << !pbo << "->" << pbo << "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) && (flipPressed)) {
            flipPressed = false;
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] F key released. Press = " << flipPressed
                      << " Time: " << crntTime << "\n";
        }
        // VSYNC
        // -----
        if ((glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) && (!vsyncPressed)) {
            vsyncPressed = true;
            ToggleVsync();
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] V key pressed. Press = " << vsyncPressed
                      << " Time: " << crntTime << "\n";
            // std::cout << "Toggled Vsync: " << !vsync << "->" << vsync <<
            // "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) &&
            (vsyncPressed)) {
            vsyncPressed = false;
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] V key released. Press = " << vsyncPressed
                      << " Time: " << crntTime << "\n";
        }
        // Wireframe
        // ---------
        if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) &&
            (!wireframePressed)) {
            wireframePressed = true;
            ToggleWireframe();
            if (wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                // glLineWidth(2.0f); // Only affects non-core-profile backends
            } else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] M key pressed. Press = " << wireframePressed
                      << " Time: " << crntTime << "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) &&
            (wireframePressed)) {
            wireframePressed = false;
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] M key released. Press = " << wireframePressed
                      << " Time: " << crntTime << "\n";
        }
        // ImGui
        // -----
        if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) && (!imguiPressed)) {
            imguiPressed = true;
            ToggleImGui();
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] N key pressed. Press = " << imguiPressed
                      << " Time: " << crntTime << "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) &&
            (imguiPressed)) {
            imguiPressed = false;
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] N key released. Press = " << imguiPressed
                      << " Time: " << crntTime << "\n";
        }
        // Atlas
        // -----
        if ((glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) && (!atlasPressed)) {
            atlasPressed = true;
            cycleTriState(currentTextMode);
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] B key pressed. Press = " << atlasPressed
                      << " Time: " << crntTime << "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) &&
            (atlasPressed)) {
            atlasPressed = false;
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] B key released. Press = " << atlasPressed
                      << " Time: " << crntTime << "\n";
        }
        // Recording
        // ---------
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) &&
            (!recordPressed)) {
            recordPressed = true;
            recording_once = true;

            // if encoding and recording turned off, wait for encoding to turn
            // off before turning recording again
            if (encoder->isEncoding.load(std::memory_order_acquire) &&
                !recording.load(std::memory_order_acquire)) {
                oss << "[main] R key pressed. WARNING: Trying to turn on "
                       "recording when encoding hasn't finished!\n";
            } else {
                recording.store(!recording.load(std::memory_order_acquire),
                                std::memory_order_release);
                encoder->queueCond
                    .notify_all(); // Wake up encoder thread if asleep
                oss << "[main] R key pressed. Press = " << recordPressed
                    << " Time: " << crntTime << "\n";
            }
        }
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) &&
            (recordPressed)) {
            recordPressed = false;
            std::lock_guard<std::mutex> coutLock(coutMutex);
            oss << "[main] R key released. Press = " << recordPressed
                << " Time: " << crntTime << "\n";
        }
        // Encoder thread
        // --------------
        if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) &&
            (!encoderPressed)) {
            encoderPressed = true;
            encoder_thread.store(
                !encoder_thread.load(std::memory_order_acquire),
                std::memory_order_release);
            encoder->queueCond.notify_all(); // Wake encoder thread if asleep
            std::lock_guard<std::mutex> coutLock(coutMutex);
            oss << "[main] T key pressed. Press = " << encoderPressed
                << " Time: " << crntTime << "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) &&
            (encoderPressed)) {
            encoderPressed = false;
            std::lock_guard<std::mutex> coutLock(coutMutex);
            oss << "[main] T key released. Press = " << encoderPressed
                << " Time: " << crntTime << "\n";
        }
        // Print to terminal
        // -----------------
        if (!oss.str().empty()) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << oss.str();
        }
    }

    // glfw: whenever window size changes (by OS or user) this callback function
    // executes
    // ----------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        using namespace Settings;
        // make sure the viewport matches the new window dimensions; note that
        // width and height will be significantly larger than specified on
        // retina displays.

        // keep viewport size fixed (recording doesn't get messed up)
        // ----------------------------------------------------------
        // lowerLeftCornerOfViewportX = 0.5*(width -
        // static_cast<int>(SCR_WIDTH));    // need to convert SCR_WIDTH/HEIGHT
        // from unsigned int->int! lowerLeftCornerOfViewportY = 0.5*(height -
        // static_cast<int>(SCR_HEIGHT)); lowerLeftCornerOfViewportX =
        // std::max(lowerLeftCornerOfViewportX, 0); lowerLeftCornerOfViewportY =
        // std::max(lowerLeftCornerOfViewportY, 0);

        window_width = width;
        window_height = height;
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        // Update ImGui's projection and screen space alignment
        // UpdateImGuiProjection(lowerLeftCornerOfViewportX,
        // lowerLeftCornerOfViewportY, SCR_WIDTH, SCR_HEIGHT);
        // ImGui::SetNextWindowPos(ImVec2(lowerLeftCornerOfViewportX + 10,
        // lowerLeftCornerOfViewportY + 10)); UpdateImGuiProjection(0, 0,
        // SCR_WIDTH, SCR_HEIGHT);

        // viewport stretches (recording gets messed up)
        // ---------------------------------------------
        // glViewport(0, 0, width, height);

        // scale viewport to always be inside window with correct aspect ratio
        // (works nice when SCR_WIDTH and SCR_HEIGHT = monitor resolution)
        // -----------------------------------------------------------------------------------------------------------------------------------
        // if ((float)height/(float)width > (float)SCR_HEIGHT/(float)SCR_WIDTH)
        //     glViewport(0, 0.5*((float)height -
        //     (float)SCR_HEIGHT*(float)width/(float)SCR_WIDTH), width,
        //     width*(float)SCR_HEIGHT/(float)SCR_WIDTH);
        // else
        //     glViewport(0.5*((float)width -
        //     (float)SCR_WIDTH*(float)height/(float)SCR_HEIGHT), 0,
        //     height*(float)SCR_WIDTH/(float)SCR_HEIGHT, height);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void cursor_pos_callback(GLFWwindow *window, double xposIn, double yposIn) {
        using namespace Settings;
        AppState *state = (AppState *)glfwGetWindowUserPointer(window);
        // mouse pos
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        // only update camera when unpaused
        if (!paused) {
            if (firstMouse) {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xoffset = xpos - lastX;
            float yoffset =
                lastY -
                ypos; // reversed since y-coordinates go from bottom to top

            lastX = xpos;
            lastY = ypos;

            camera.ProcessMouseMovement(xoffset, yoffset);
        }

        // only update cursor pos for browser when paused
        if (paused) {
            // --- Ultralight ---
            if (inputMode == INPUT_ULTRALIGHT) {
                if (!state->ultralightBrowser)
                    return;

                glm::vec3 rayDir =
                    getMouseRay(xpos, ypos, *state->projection, *state->view);

                glm::vec3 hitPoint;
                if (intersectRayPlane(camera.Position, rayDir,
                                      state->planePoint, state->planeNormal,
                                      hitPoint)) {
                    glm::vec3 local = hitPoint - state->planePoint;

                    float u = (local.x / state->quadWidth) + 0.5f;
                    float v = (local.y / state->quadHeight) + 0.5f;
                    v = 1.0f - v;

                    if (u >= 0 && u <= 1 && v >= 0 && v <= 1) {
                        int px =
                            (int)(u * state->ultralightBrowser->getWidth());
                        int py =
                            (int)(v * state->ultralightBrowser->getHeight());

                        ultralight::MouseEvent evt;
                        evt.type = ultralight::MouseEvent::kType_MouseMoved;
                        evt.x = px;
                        evt.y = py;
                        evt.button = ultralight::MouseEvent::kButton_None;

                        state->ultralightBrowser->fireMouseEvent(evt);
                    }
                }
            }

            // --- CEF ---
            if (inputMode == INPUT_CEF) {
                if (!state->cefClient->browser)
                    return;

                glm::vec3 rayDir =
                    getMouseRay(xpos, ypos, *state->projection, *state->view);
                glm::vec3 hitPoint;

                if (intersectRayPlane(camera.Position, rayDir,
                                      state->cefPlanePoint,
                                      state->cefPlaneNormal, hitPoint)) {
                    glm::vec3 local = hitPoint - state->cefPlanePoint;
                    float u = (local.x / state->cefQuadWidth) + 0.5f;
                    float v = 1.0f - ((local.y / state->cefQuadHeight) + 0.5f);
                    if (u >= 0 && u <= 1 && v >= 0 && v <= 1) {
                        int px =
                            (int)(u * state->cefClient->renderHandler->width);
                        int py =
                            (int)(v * state->cefClient->renderHandler->height);
                        CefMouseEvent evt;
                        evt.x = px;
                        evt.y = py;
                        evt.modifiers = 0;
                        state->cefClient->browser->GetHost()
                            ->SendMouseMoveEvent(evt, false);
                    }
                }
            }
        }
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
        // camera
        if (inputMode == INPUT_GAME)
            camera.ProcessMouseScroll(static_cast<float>(yoffset));

        AppState *state = (AppState *)glfwGetWindowUserPointer(window);

        // ULTRALIGHT
        if (inputMode == INPUT_ULTRALIGHT) {
            if (!state->ultralightBrowser)
                return;

            ultralight::ScrollEvent evt;
            evt.type = ultralight::ScrollEvent::kType_ScrollByPixel;
            evt.delta_x = (int)xoffset * 50; // tweak sensitivity
            evt.delta_y = (int)yoffset * 50;

            state->ultralightBrowser->fireScrollEvent(evt);
        }

        // CEF
        if (inputMode == INPUT_CEF) {
            if (!state->cefClient->browser)
                return;

            CefMouseEvent evt;
            evt.x = 0;
            evt.y = 0;
            evt.modifiers = 0; // position doesn't matter for scroll
            state->cefClient->browser->GetHost()->SendMouseWheelEvent(
                evt, (int)(xoffset * 50), (int)(yoffset * 50));
        }
    }

    void mouse_button_callback(GLFWwindow *window, int button, int action,
                               int mods) {
        AppState *state = (AppState *)glfwGetWindowUserPointer(window);

        glm::mat4 &view = *state->view;
        glm::mat4 &projection = *state->projection;

        glm::vec3 planePoint = state->planePoint;
        glm::vec3 planeNormal = state->planeNormal;

        float quadWidth = state->quadWidth;
        float quadHeight = state->quadHeight;

        // --- Ultralight ---
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            glm::vec3 rayDir = getMouseRay(mouseX, mouseY, projection, view);
            glm::vec3 hitPoint;

            if (intersectRayPlane(camera.Position, rayDir, planePoint,
                                  planeNormal, hitPoint)) {
                // Convert to local quad space
                glm::vec3 local = hitPoint - planePoint;

                float u = (local.x / quadWidth) + 0.5f;
                float v = (local.y / quadHeight) + 0.5f;

                // Flip Y for OpenGL
                v = 1.0f - v;

                // Check inside quad
                if (u >= 0 && u <= 1 && v >= 0 && v <= 1) {
                    int px = (int)(u * state->ultralightBrowser->getWidth());
                    int py = (int)(v * state->ultralightBrowser->getHeight());

                    ultralight::MouseEvent evt;
                    evt.x = px;
                    evt.y = py;
                    evt.button = ultralight::MouseEvent::kButton_Left;

                    if (action == GLFW_PRESS)
                        evt.type = ultralight::MouseEvent::kType_MouseDown;
                    if (action == GLFW_RELEASE)
                        evt.type = ultralight::MouseEvent::kType_MouseUp;

                    state->ultralightBrowser->fireMouseEvent(evt);
                }
            }
        }

        // --- CEF ---
        // if (inputMode == INPUT_CEF) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (!state->cefClient->browser)
                return;

            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            glm::vec3 rayDir =
                getMouseRay(mouseX, mouseY, *state->projection, *state->view);
            glm::vec3 hitPoint;

            if (intersectRayPlane(camera.Position, rayDir, state->cefPlanePoint,
                                  state->cefPlaneNormal, hitPoint)) {
                // Convert to local quad space
                glm::vec3 local = hitPoint - state->cefPlanePoint;

                float u = (local.x / state->cefQuadWidth) + 0.5f;
                float v = (local.y / state->cefQuadHeight) + 0.5f;

                // Flip Y for OpenGL
                v = 1.0f - v;

                // Check inside quad
                if (u >= 0 && u <= 1 && v >= 0 && v <= 1) {
                    int px = (int)(u * state->cefClient->renderHandler->width);
                    int py = (int)(v * state->cefClient->renderHandler->height);

                    CefMouseEvent evt;
                    evt.x = px;
                    evt.y = py;
                    evt.modifiers = 0;

                    CefBrowserHost::MouseButtonType btnType = MBT_LEFT;
                    bool mouseUp = (action == GLFW_RELEASE);
                    state->cefClient->browser->GetHost()->SendMouseClickEvent(
                        evt, btnType, mouseUp, 1);
                }
            }
        }
    }

    void key_callback(GLFWwindow *window, int key, int scancode, int action,
                      int mods) {
        using namespace Settings;

        AppState *state = (AppState *)glfwGetWindowUserPointer(window);

        // --- GLOBAL KEYS (always work) ---
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
            return;
        }

        // Cycle input mode: GAME -> ULTRALIGHT -> CEF -> GAME
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
            if (inputMode == INPUT_GAME)
                inputMode = INPUT_ULTRALIGHT;
            else if (inputMode == INPUT_ULTRALIGHT)
                inputMode = INPUT_CEF;
            else
                inputMode = INPUT_GAME;
            return;
        }

        // Pause
        // -----
        if (key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS &&
            !pausePressed) {
            pausePressed = 1;

            // pause -> unpause
            if (paused) {
                firstMouse = true;
                lastXpos = static_cast<double>(lastX);
                lastYpos = static_cast<double>(lastY);
                glfwSetCursorPos(window, lastXpos,
                                 lastYpos); // remove whiplash cursor jump
                // tell GLFW to capture our mouse
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                paused = 0;
            }
            // unpause -> pause
            else {
                // tell GLFW to uncapture our mouse
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                paused = 1;
            }
        }
        if (key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_RELEASE &&
            pausePressed) {
            pausePressed = 0;
        }

        // --- ROUTE INPUT ---
        if (inputMode == INPUT_ULTRALIGHT) {
            // Send to Ultralight
            ultralight::KeyEvent evt;

            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                evt.type = ultralight::KeyEvent::kType_RawKeyDown;
            } else if (action == GLFW_RELEASE) {
                evt.type = ultralight::KeyEvent::kType_KeyUp;
            } else
                return;

            evt.virtual_key_code = glfwToVirtualKey(key);
            evt.native_key_code = scancode;
            evt.modifiers = 0;

            ultralight::GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code,
                                                           evt.key_identifier);

            if (mods & GLFW_MOD_SHIFT)
                evt.modifiers |= ultralight::KeyEvent::kMod_ShiftKey;
            if (mods & GLFW_MOD_CONTROL)
                evt.modifiers |= ultralight::KeyEvent::kMod_CtrlKey;
            if (mods & GLFW_MOD_ALT)
                evt.modifiers |= ultralight::KeyEvent::kMod_AltKey;
            if (key == GLFW_KEY_LEFT && (mods & GLFW_MOD_ALT) &&
                action != GLFW_RELEASE) {
                state->ultralightBrowser->GoBack();
            }
            if (key == GLFW_KEY_RIGHT && (mods & GLFW_MOD_ALT) &&
                action != GLFW_RELEASE) {
                state->ultralightBrowser->GoForward();
            }

            state->ultralightBrowser->fireKeyEvent(evt);
        }

        if (inputMode == INPUT_CEF) {
            if (!state->cefClient->browser)
                return;
            if (action == GLFW_REPEAT && key != GLFW_KEY_BACKSPACE)
                return;

            CefKeyEvent evt;
            evt.windows_key_code = glfwToVirtualKey(key);
            evt.native_key_code = scancode;
            evt.modifiers = 0;
            if (mods & GLFW_MOD_SHIFT)
                evt.modifiers |= EVENTFLAG_SHIFT_DOWN;
            if (mods & GLFW_MOD_CONTROL)
                evt.modifiers |= EVENTFLAG_CONTROL_DOWN;
            if (mods & GLFW_MOD_ALT)
                evt.modifiers |= EVENTFLAG_ALT_DOWN;

            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                evt.type = KEYEVENT_RAWKEYDOWN;
                state->cefClient->browser->GetHost()->SendKeyEvent(evt);
            } else if (action == GLFW_RELEASE) {
                evt.type = KEYEVENT_KEYUP;
                state->cefClient->browser->GetHost()->SendKeyEvent(evt);
            }

            if (key == GLFW_KEY_LEFT && (mods & GLFW_MOD_ALT) &&
                action != GLFW_RELEASE) {
                state->cefClient->browser->GoBack();
            }
            if (key == GLFW_KEY_RIGHT && (mods & GLFW_MOD_ALT) &&
                action != GLFW_RELEASE) {
                state->cefClient->browser->GoForward();
            }
        }
    }

    void char_callback(GLFWwindow *window, unsigned int codepoint) {
        AppState *state = (AppState *)glfwGetWindowUserPointer(window);

        if (inputMode == INPUT_ULTRALIGHT) {
            if (!state->ultralightBrowser)
                return;

            char utf8[5] = {0};

            if (codepoint <= 0x7F) {
                utf8[0] = (char)codepoint;
            }

            ultralight::KeyEvent evt;
            evt.type = ultralight::KeyEvent::kType_Char;
            evt.text = ultralight::String(utf8);
            evt.unmodified_text = ultralight::String(utf8);

            state->ultralightBrowser->fireKeyEvent(evt);
        }

        if (inputMode == INPUT_CEF) {
            if (!state->cefClient->browser)
                return;

            CefKeyEvent evt;
            evt.type = KEYEVENT_CHAR;
            evt.character = (char16_t)codepoint;
            evt.unmodified_character = (char16_t)codepoint;
            evt.windows_key_code = 0; // Do NOT set this to codepoint
            evt.modifiers = 0;
            state->cefClient->browser->GetHost()->SendKeyEvent(evt);
        }
    }
} // namespace Input
