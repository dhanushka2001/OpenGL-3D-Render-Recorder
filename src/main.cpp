// glad & GLFW
// -----------
#include <glad/glad.h>  // glad
#include <GLFW/glfw3.h> // GLFW (includes stdint.h)

// Custom headers
// --------------
#include <learnopengl/shader_s.h>           // Shader class
#include <learnopengl/camera.h>             // Camera class
#include <learnopengl/encoder.h>            // FFmpeg Encoder class
#include <learnopengl/fontmanager.h>        // Font Manager
#include <learnopengl/textrenderer.h>       // Text Renderer
#include <learnopengl/timer.h>              // Timer
#include <learnopengl/gui.h>                // GUI
#include <learnopengl/Settings.h>           // Settings
#include <learnopengl/ultralight_browser.h> // Ultralight Browser
#include <learnopengl/cef_app.h>            // Cef App
#include <learnopengl/cef_browser.h>        // Cef Browser
#include <learnopengl/input_callbacks.h>    // Cef Browser
#include <learnopengl/app_state.h>          // App State

// #include <gtk/gtk.h>

// I/O and filesystem
// ------------------
#include <iostream>   // for std::cin/cout/cerr
#include <filesystem> // for std::filesystem

// stb
// ---
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
// #define  STB_IMAGE_WRITE_IMPLEMENTATION
// #include <stb/stb_image_write.h>
// #define  GL_GLEXT_PROTOTYPES 1

// Memory buffer
// -------------
#include <array>
// #include <vector>

// glm
// ---
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef PATH_MAX
    #define PATH_MAX 4096
#endif /* PATH_MAX */

// Function declaration
// --------------------
std::string GetFPSText(float fps, float ms, float crntTime);
void UpdateFPS(float &fps, float &ms, float crntTime, float &lastTime,
               int &frameCountFPS);
void RenderCrate(Shader &ourShader, GLuint VAO, const glm::vec3 &trans,
                 GLuint crateTexture, GLuint awesomeTexture,
                 const std::array<glm::vec3, 10> &cubePositions,
                 glm::vec3 &lightPos, float crntTime);

#define IMGUI 1

int main(int argc, char **argv) {
    // gtk_disable_setlocale(); // call as early as possible

    // CEF: initialization
    // -------------------
    // CEF main args
    CefMainArgs main_args(argc, argv);

    // Create app instance
    CefRefPtr<SimpleApp> app = new SimpleApp();

    // CRITICAL: subprocess handling
    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0) {
        return exit_code; // child process exits here
    }

    // CEF Settings
    CefSettings settings;
    auto exeDir =
        std::filesystem::canonical("/proc/self/exe").parent_path();

    settings.no_sandbox = true; // required on Linux unless sandbox setup
    settings.windowless_rendering_enabled = true; // Optional but useful

    // Tell CEF to use our exe as the subprocess helper
    CefString(&settings.root_cache_path) = (exeDir / "cache").string();
    CefString(&settings.cache_path) = (exeDir / "cache/cache").string();
    CefString(&settings.resources_dir_path) = exeDir.string();
    CefString(&settings.locales_dir_path) = (exeDir / "locales").string();
    // Initialize CEF
    if (!CefInitialize(main_args, settings, app, nullptr)) {
        std::cerr << "CEF init failed\n";
        return -1;
    }

    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                   3); // OpenGL 4.3 => major=4,minor=3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Set window size (needs to be fixed when recording)
    // --------------------------------------------------
    using namespace Settings;
    using namespace Input;
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    SetScreenResolution(mode->width, mode->height);
    // SetScreenResolution(1600, 900); // if the resolution is too low,
    // FPS could get too high (~1000fps) and program will crash
    // SetScreenResolution(800, 450);

    lastX = SCR_WIDTH / 2;
    lastY = SCR_HEIGHT / 2;
    window_width = SCR_WIDTH;
    window_height = SCR_HEIGHT;

    // pbo settings
    // ------------
    GLuint firstIndex = 0;
    GLuint nextIndex = 1; //(firstIndex + 1) % PBO_COUNT;
    // unsigned int frameCounter = 0;
    GLuint pboIds[PBO_COUNT];
    // GLsync pboFences[PBO_COUNT] = { nullptr };
    const size_t DATA_SIZE = SCR_WIDTH * SCR_HEIGHT * 3;

    // glfw window creation
    // --------------------
    GLFWwindow *window;
    std::cout << "[main] Creating window with size: " << SCR_WIDTH << "x"
              << SCR_HEIGHT << "\n";
    if (!fullscreen)
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL",
                                  NULL, NULL); // windowed
    else
        window =
            glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL",
                             glfwGetPrimaryMonitor(), NULL); // fullscreen

    if (window == NULL) {
        std::cout << "[main] ERROR: Failed to create GLFW window"
                  << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetCursorPos(window, lastX,
                     lastY); // set cursor in centre of screen to remove
                             // whiplash cursor jump
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, char_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "[main] ERROR: Failed to initialize GLAD"
                  << std::endl;
        return -1;
    }

    // build and compile our shader program to render the crates
    // ---------------------------------------------------------
    Shader ourShader(
        "crates.vert",
        "crates.frag"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // 3D cube without EBO (6 faces * 2 triangles * 3 vertices each = 36
    // vertices. 8 unique vertices. 78% redundancy) (each face has its own
    // unique texture coords)
    //      3------2
    //     /|  Y  /|
    //    7------6 | X
    //    | 0----|-1
    //    |/ Z   |/
    //    4------5
    float vertices[] = {
        // positions            normal             texture coords
        // -Z face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // (-,-,-) 0
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // (+,-,-) 1
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // (+,+,-) 2
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // (+,+,-) 2
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // (-,+,-) 3
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // (-,-,-) 0

        // +Z face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // (-,-,+) 4
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // (+,-,+) 5
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // (+,+,+) 6
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // (+,+,+) 6
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // (-,+,+) 7
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // (-,-,+) 4

        // -X face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // (-,+,+) 7
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // (-,+,-) 3
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // (-,-,-) 0
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // (-,-,-) 0
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // (-,-,+) 4
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // (-,+,+) 7

        // +X face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // (+,+,+) 6
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // (+,+,-) 2
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // (+,-,-) 1
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // (+,-,-) 1
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // (+,-,+) 5
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // (+,+,+) 6

        // -Y face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // (-,-,-) 0
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // (+,-,-) 1
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // (+,-,+) 5
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // (+,-,+) 5
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // (-,-,+) 4
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // (-,-,-) 0

        // +Y face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // (-,+,-) 3
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // (+,+,-) 2
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // (+,+,+) 6
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // (+,+,+) 6
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // (-,+,+) 7
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // (-,+,-) 3
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex
    // buffer(s), and then configure vertex attributes(s). once a VAO is
    // bound, any subsequent vertex attribute calls will be stored inside
    // that VAO. (only have to make the calls once)
    glBindVertexArray(VAO);

    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // You can unbind the VAO afterwards so other VAO calls won't
    // accidentally modify this VAO, but this rarely happens. Modifying
    // other VAOs requires a call to glBindVertexArray anyways so we
    // generally don't unbind VAOs (nor VBOs) when it's not directly
    // necessary.
    glBindVertexArray(0);

    // load and create textures
    // ------------------------
    GLuint crateTexture, awesomeTexture;

    // texture 1
    // ---------
    glGenTextures(1, &crateTexture);
    glBindTexture(GL_TEXTURE_2D, crateTexture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT
                                // (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(
        true); // tell stb_image.h to flip loaded texture's on the y-axis.
    namespace fs = std::filesystem;
    const char *containerfilePath = "../assets/textures/container.jpg";
    unsigned char *data =
        stbi_load(containerfilePath, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "[main] WARNING: Failed to load texture: "
                  << fs::path(containerfilePath).filename() << '\n';
    }
    stbi_image_free(data);

    // texture 2
    // ---------
    glGenTextures(1, &awesomeTexture);
    glBindTexture(GL_TEXTURE_2D, awesomeTexture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT
                                // (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    const char *awesomefacefilePath =
        "../assets/textures/awesomeface.png";
    data =
        stbi_load(awesomefacefilePath, &width, &height, &nrChannels, 0);
    if (data) {
        // note that the awesomeface.png has transparency and thus an
        // alpha channel, so make sure to tell OpenGL the data type is of
        // GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "[main] WARNING: Failed to load texture: "
                  << fs::path(awesomefacefilePath).filename() << '\n';
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to
    // (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before
                     // setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the shader class:
    ourShader.setInt("texture2", 1);

    // MODEL
    // -----
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    ourShader.setMat4("model", model);

    // VIEW
    // ----
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of
    // where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    ourShader.setMat4("view", view);

    // PROJECTION
    // ----------
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,
        100.0f);
    // glm::mat4 projection = glm::ortho(0.0f,
    // static_cast<float>(SCR_WIDTH), 0.0f,
    // static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D
    // rendering
    ourShader.setMat4("projection", projection);

    glm::vec3 lightPos(8.0f, 0.0f, 0.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
    ourShader.setVec3("lightColor", lightColor);
    ourShader.setVec3("lightPos", lightPos);
    ourShader.setVec3("objectColor", objectColor);

    ourShader.setVec3("viewPos", camera.Position);

    // bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crateTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, awesomeTexture);

    std::array<glm::vec3, 10> cubePositions = {{{0.0f, 0.0f, -2.0f},
                                                {2.0f, 5.0f, -15.0f},
                                                {-1.5f, -2.2f, -2.5f},
                                                {-3.8f, -2.0f, -12.3f},
                                                {2.4f, -0.4f, -3.5f},
                                                {-1.7f, 3.0f, -7.5f},
                                                {1.3f, -2.0f, -2.5f},
                                                {1.5f, 2.0f, -2.5f},
                                                {1.5f, 0.2f, -1.5f},
                                                {-1.3f, 1.0f, -1.5f}}};

    // fbo settings
    // ------------
    GLuint fboMsaaId, rboMsaaColorId, rboMsaaDepthId;
    GLuint fboId;
    // GLuint rboId;
    GLuint fboTexture;

    std::array<std::unique_ptr<uint8_t[]>, BUFFER_COUNT> frameBuffers;
    int currentWriteIndex = 0;

    std::array<GLubyte *, BUFFER_COUNT> pboBuffers;

    // if (recording)
    {
        // PBO OFF
        // -------
        for (int i = 0; i < static_cast<int>(BUFFER_COUNT); ++i) {
            frameBuffers[i] = std::make_unique<uint8_t[]>(DATA_SIZE);
        }

        // PBO ON
        // ------
        // create 2 pixel buffer objects, you need to delete them when
        // program exits. glBufferData() with NULL pointer reserves only
        // memory space.
        glGenBuffers(PBO_COUNT, pboIds);
        for (int i = 0; i < static_cast<int>(PBO_COUNT); ++i) {
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[i]);
            glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, nullptr,
                         GL_STREAM_READ); // STREAM_READ is better here
        }
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        // Somewhere at initialization
        /*  Framebuffer */
        glGenFramebuffers(1, &fboMsaaId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

        /* 4x MSAA renderbuffer object for colorbuffer */
        unsigned int MSAA = 0;
        if (msaa) {
            MSAA = 4;
        }
        glGenRenderbuffers(1, &rboMsaaColorId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboMsaaColorId);
        /* Storage must be one of: */
        /* GL_RGBA4, GL_RGB565, GL_RGB5_A1, GL_DEPTH_COMPONENT16,
         * GL_STENCIL_INDEX8. */
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_RGBA4,
                                         SCR_WIDTH, SCR_HEIGHT);
        // attach colorbuffer image to FBO
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
            GL_COLOR_ATTACHMENT0, // 2. color attachment point
            GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
            rboMsaaColorId);      // 4. rbo ID
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        /* 4x MSAA renderbuffer object for depthbuffer */
        glGenRenderbuffers(1, &rboMsaaDepthId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboMsaaDepthId);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA,
                                         GL_DEPTH_COMPONENT, SCR_WIDTH,
                                         SCR_HEIGHT);

        // attach depthbuffer image to FBO
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
            GL_DEPTH_ATTACHMENT, // 2. depth attachment point
            GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
            rboMsaaDepthId);     // 4. rbo ID
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Check if the MSAA FBO is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "[main] ERROR::FRAMEBUFFER:: Framebuffer "
                         "fboMsaaId is not complete!\n"
                      << std::endl;
            return -1;
        }

        // create a normal (no MSAA) FBO to hold a render-to-texture
        glGenFramebuffers(1, &fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        // create a texture object
        glGenTextures(1, &fboTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, fboTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);

        // Before drawing
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        // attach the texture to FBO color attachment point
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
            GL_COLOR_ATTACHMENT0, // 2. attachment point
            GL_TEXTURE_2D,        // 3. tex target: GL_TEXTURE_2D
            fboTexture,           // 4. tex ID
            0);                   // 5. mipmap level: 0(base)

        // Set the list of draw buffers.
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

        // glGenRenderbuffers(1, &rboId);
        // glBindRenderbuffer(GL_RENDERBUFFER, rboId);
        // glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, SCR_WIDTH,
        // SCR_HEIGHT); glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        // GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboId);
        // glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // attach the renderbuffer to depth attachment point
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo
        // target: GL_FRAMEBUFFER
        //                           GL_DEPTH_ATTACHMENT, // 2. attachment
        //                           point GL_RENDERBUFFER,     // 3. rbo
        //                           target: GL_RENDERBUFFER rboId); // 4.
        //                           rbo ID

        // Check if the FBO is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "[main] ERROR::FRAMEBUFFER:: Framebuffer fboId "
                         "is not complete!\n"
                      << std::endl;
            return -1;
        }
    }

    // light source
    // ------------
    Shader lightShader("light.vert", "light.frag");
    lightShader.use();
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already
    // contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube), light_cube,
    // GL_STATIC_DRAW); position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // scale by 0.2
    lightShader.setMat4("model", model);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);

    // flip shader
    // -----------
    Shader flipShader("flip.vert", "flip.frag");
    GLuint fboFlip, fboFlipTexture, dummyVAO; // quadVBO;
    glGenFramebuffers(1, &fboFlip);
    glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
    glGenVertexArrays(1, &dummyVAO);
    // Create the texture to attach to fboFlip
    glGenTextures(1, &fboFlipTexture);
    glBindTexture(GL_TEXTURE_2D, fboFlipTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, fboFlipTexture, 0);
    // Check framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
        GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "[main] ERROR: fboFlip is not complete!\n";
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

// IMGUI
// -----
#if IMGUI == 1
    GUI::Init(window);
#endif

    glEnable(GL_BLEND); // enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // TEXT
    // ----
    FT_UInt fontSize = 48;
    // Render FPS text at the top-left corner
    float scale = static_cast<float>(SCR_WIDTH) * 0.146f / 800.0f;
    // Position on the screen
    float x = lowerLeftCornerOfViewportX;
    float y =
        lowerLeftCornerOfViewportY + static_cast<float>(SCR_HEIGHT) -
        35.0f * scale * fontSize /
            48.0f; // Invert Y-axis since OpenGL origin is bottom-left
    glm::vec3 color(1.0f, 1.0f, 1.0f); // White text
    std::string font = "Arial";
    FontManager fontManager;
    fontManager.loadFont("Arial", 48);
    TextRenderer textRenderer(fontManager);

    Timer::init();
    std::chrono::high_resolution_clock::time_point t;

    // create output directory
    fs::path outputDir = "../output";
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    std::unique_ptr<Encoder> encoder = std::make_unique<Encoder>();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Hidden window
    GLFWwindow *sharedContextWindow = glfwCreateWindow(
        1, 1, "", nullptr,
        window); // Pass window as 5th param = share context

    encoder->start(window, sharedContextWindow); // encoder thread

    // timings
    // -------
    unsigned int counter = 0; // tracks amount of frames for window title
    int frameCountFPS = 0;    // tracks amount of frames for FPS text
    float fps = 0.0f;         // for FPS text
    float ms = 0.0f;          // for FPS text
    float timeDiff, prevTime = 0.0f; // for movement (time between frames)
    float deltaTime, oldTime = 0.0f; // for window title FPS counter
    float lastTime = 0.0f;           // for FPS text
    float encodeDiff, encodeTime = 0.0f; // for encoding frames
    float crntTime = 0.0f;               // current time (used by all)

    // Generic floating quad
    // ---------------------
    float quadVertices[] = {
        // positions          // texCoords
        -1.0f, 1.0f, 0.0f, 0.0f,  1.0f, -1.0f, -1.0f, 0.0f,
        0.0f,  0.0f, 1.0f, -1.0f, 0.0f, 1.0f,  0.0f,

        -1.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  -1.0f, 0.0f,
        1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,  1.0f};

    GLuint quadVAO, quadVBO;

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
                 GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // texCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    // ---------------------------------------------------

    Shader browserShader("browser.vert", "browser.frag");

    // ULTRALIGHT BROWSER
    // ------------------
    GLuint browserTex;
    glGenTextures(1, &browserTex);
    glBindTexture(GL_TEXTURE_2D, browserTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 768, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // --- Ultralight Platform Setup (DO THIS ONCE) ---
    {
        Config config;
        config.user_stylesheet = "body { background: white; }";

        Platform::instance().set_config(config);

        Platform::instance().set_font_loader(GetPlatformFontLoader());
        Platform::instance().set_file_system(GetPlatformFileSystem("./"));
        Platform::instance().set_logger(
            GetDefaultLogger("ultralight.log"));
    }

    UltralightBrowser browser;
    browser.init(1024, 768, "https://google.com");

    glm::mat4 model_browser = glm::mat4(1.0f);

    glm::vec3 planePoint = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 planeNormal = glm::vec3(0.0f, 0.0f, 1.0f); // facing camera

    // Move in front of camera
    model_browser = glm::translate(model_browser, planePoint);

    // Scale to window-like shape
    float browser_width = 3.6f; // 3.2f;
    float browser_height = 2.0f;
    model_browser =
        glm::scale(model_browser, glm::vec3(browser_width / 2,
                                            browser_height / 2, 1.0f));

    AppState state;

    state.ultralightBrowser = &browser;
    state.view = &view;
    state.projection = &projection;

    state.planePoint = planePoint;
    state.planeNormal = planeNormal;

    state.quadWidth = browser_width;
    state.quadHeight = browser_height;

    // attach app state to window
    glfwSetWindowUserPointer(window, &state);
    // ---------------------------------------------------

    // CEF BROWSER
    // ---------------------------------------------------
    GLuint cefTex;
    glGenTextures(1, &cefTex);
    glBindTexture(GL_TEXTURE_2D, cefTex);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 768, 0, GL_BGRA,
    // GL_UNSIGNED_BYTE, nullptr);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create client
    // CefRefPtr<SimpleClient> client = new SimpleClient(1024, 768);
    CefRefPtr<SimpleClient> client =
        new SimpleClient(1280, 720, encoder.get());
    CefWindowInfo window_info; // Create browser
    window_info.SetAsWindowless(
        0); // 0 = no parent window, correct for offscreen
    CefBrowserSettings browser_settings;
    // CefBrowserHost::CreateBrowser(
    //     window_info,
    //     client,                      // CEF SimpleClient
    //     "https://www.google.com",    // starting URL
    //     browser_settings,
    //     nullptr,                     // no extra info
    //     nullptr                      // no request context, uses global
    // );

    // INCOGNITO MODE
    // --------------
    // 1. Setup request context settings for incognito (in-memory)
    CefRequestContextSettings context_settings;
    // Ensure cache_path is empty to enable incognito mode
    // CefString(&context_settings.cache_path).FromASCII("");

    // 2. Create the custom request context
    CefRefPtr<CefRequestContext> request_context =
        CefRequestContext::CreateContext(context_settings, nullptr);

    // 3. Create the browser with the new context
    CefBrowserHost::CreateBrowser(
        window_info, client, "https://www.google.com", browser_settings,
        nullptr,
        request_context // Use our incognito context instead of nullptr
    );

    // Separate transform (different pos to Ultralight)
    glm::vec3 cefPlanePoint =
        glm::vec3(4.0f, 0.0f, 3.0f); // offset so they don't overlap
    glm::vec3 cefPlaneNormal =
        glm::vec3(0.0f, 0.0f, 1.0f); // facing camera
    glm::mat4 model_cef = glm::mat4(1.0f);
    model_cef = glm::translate(model_cef, cefPlanePoint); // different X
    model_cef =
        glm::scale(model_cef, glm::vec3(browser_width / 2,
                                        browser_height / 2, 1.0f));

    state.cefClient = client;
    state.cefPlanePoint = cefPlanePoint;
    state.cefPlaneNormal = cefPlaneNormal;
    // state.cefBrowser is set async via OnAfterCreated
    state.cefQuadWidth = browser_width;
    state.cefQuadHeight = browser_height;
    // ---------------------------------------------------

    // vsync
    int lastVsync = vsync;
    glfwSwapInterval(vsync); // vsync

    // RENDER LOOP
    // -----------
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents(); // take care of all GLFW events

        // Only update vsync when needed
        if (vsync != lastVsync) {
            glfwSwapInterval(vsync); // vsync
            lastVsync = vsync;
        }

        CefDoMessageLoopWork();

        crntTime = static_cast<float>(
            glfwGetTime()); // Updates counter and times
        timeDiff =
            crntTime - prevTime; // for movement (time between frames)
        prevTime = crntTime;

        // input
        // -----
        processInput(window, timeDiff, crntTime, encoder);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "[main] OpenGL Error: " << error << std::endl;
            break;
        }

        // clear buffer
        // ------------
        // Set the clear color to the background color of choice
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // cyan
        // Clear the current framebuffer
        glClear(GL_COLOR_BUFFER_BIT);

// IMGUI
// -----
#if IMGUI == 1
        if (imgui) {
            GUI::NewFrame();
        }
#endif

        // FPS Counter: https://www.youtube.com/watch?v=BA6aR_5C_BM
        deltaTime = crntTime - oldTime;
        counter++;
        if (deltaTime >= 60.0 / 60.0) {
            // Creates new title
            std::string FPS = std::to_string((1.0 / deltaTime) * counter);
            std::string ms = std::to_string((deltaTime / counter) * 1000);
            std::string newTitle =
                "LearnOpenGL - " + FPS + "FPS / " + ms + "ms";
            glfwSetWindowTitle(window, newTitle.c_str());

            // Resets times and counter
            oldTime = crntTime;
            counter = 0;
        }

        // Update timing info for FPS text
        UpdateFPS(fps, ms, crntTime, lastTime, frameCountFPS);
        std::string fpsText = GetFPSText(fps, ms, crntTime);

        // recording ON
        if (recording) {
            // Step 1: Render the scene to the MSAA FBO
            // ----------------------------------------
            {
                glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
                glEnable(GL_DEPTH_TEST); // Needed for 3D rendering
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                // Enable blending
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                Timer::startTimer(t);
                // static crate
                glm::vec3 translatenew = glm::vec3(0.0f, 0.0f, 0.0f);
                RenderCrate(ourShader, VAO, translatenew, crateTexture,
                            awesomeTexture, cubePositions, lightPos,
                            crntTime);

                // controllable crate
                glm::vec3 translate =
                    glm::vec3(xOffset, yOffset, zOffset);
                RenderCrate(ourShader, VAO, translate, crateTexture,
                            awesomeTexture, cubePositions, lightPos,
                            crntTime);

                // ULTRALIGHT
                // --------------------------------------------
                browser.update();
                browser.render();

                unsigned char *pixels = browser.getPixels();

                if (pixels) {
                    glBindTexture(GL_TEXTURE_2D, browserTex);

                    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                                    browser.getWidth(),
                                    browser.getHeight(), GL_BGRA,
                                    GL_UNSIGNED_BYTE, pixels);

                    browser.unlockPixels();
                }

                // Render quad
                browserShader.use();

                browserShader.setMat4("model", model_browser);
                browserShader.setMat4("view", view);
                browserShader.setMat4("projection", projection);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, browserTex);
                browserShader.setInt("browserTexture", 0);

                glBindVertexArray(quadVAO);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
                // ----------------------------------------------

                // --- CEF (same VAO, different tex + transform) ---
                auto &rh = client->renderHandler;
                if (rh->dirty) {
                    rh->dirty = false;
                    glBindTexture(GL_TEXTURE_2D, cefTex);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rh->width,
                                    rh->height, GL_BGRA, GL_UNSIGNED_BYTE,
                                    rh->pixelBuffer.data());
                }
                browserShader.use(); // same shader works fine too
                browserShader.setMat4("model", model_cef);
                browserShader.setMat4("view", view);
                browserShader.setMat4("projection", projection);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cefTex);
                browserShader.setInt("browserTexture", 0);
                glBindVertexArray(quadVAO); // reused again
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
                // -------------------------------------------------

                // draw the light cube object
                lightShader.use();
                lightShader.setMat4("projection", projection);
                view = camera.GetViewMatrix();
                lightShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                model = glm::translate(model, lightPos);
                model =
                    glm::scale(model, glm::vec3(0.2f)); // a smaller cube
                lightShader.setMat4("model", model);
                glBindVertexArray(lightVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                {
                    // std::lock_guard<std::mutex> lock(coutMutex);
                    Timer::endTimer(Timer::RENDER_SCENE, t);
                }
            }

            // Step 2: Resolve MSAA FBO to standard non-MSAA FBO
            // -------------------------------------------------
            {
                Timer::startTimer(t);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
                glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, // src rect
                                  0, 0, SCR_WIDTH,
                                  SCR_HEIGHT,          // dest rect
                                  GL_COLOR_BUFFER_BIT, // buffer mask
                                  GL_LINEAR);          // scale filter
                {
                    // std::lock_guard<std::mutex> lock(coutMutex);
                    Timer::endTimer(Timer::BLIT_MSAA, t);
                }

                // FPS TEXT AND ATLAS
                // ----------------------------------------------
                Timer::startTimer(t);
                if (currentTextMode == TextTriState::TextAndAtlasON ||
                    currentTextMode == TextTriState::TextONAtlasOFF)
                    // textRenderer.renderText(fpsText, x, y, scale,
                    // color, font);
                    textRenderer.renderTextFast(fpsText, x, y, scale,
                                                color, font);
                if (currentTextMode == TextTriState::TextAndAtlasON)
                    textRenderer.renderAtlas(font);
                {
                    // std::lock_guard<std::mutex> lock(coutMutex);
                    Timer::endTimer(Timer::RENDER_TEXT, t);
                }
// -----------------------------------------------

// IMGUI (visible in screen recording, messes up when window is resized)
// ---------------------------------------------------------------------
#if IMGUI == 1
                if (imgui) {
                    Timer::startTimer(t);
                    GUI::Render(encoder.get());
                    {
                        // std::lock_guard<std::mutex> lock(coutMutex);
                        Timer::endTimer(Timer::RENDER_GUI, t);
                    }
                }
#endif /* IMGUI==1 */
                // ---------------------------------------------------------------------
            }

            // Step 3: Render the scene on-screen using Blitting:
            // https://stackoverflow.com/a/31487085
            // ---------------------------------------------------------------------------------------
            {
                // Bind the target FBO to read
                glBindFramebuffer(GL_FRAMEBUFFER, fboId);
                Timer::startTimer(t);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
                glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, // src rect
                                  0, 0, SCR_WIDTH,
                                  SCR_HEIGHT,          // dest rect
                                  GL_COLOR_BUFFER_BIT, // buffer mask
                                  GL_NEAREST);         // scale filter
                {
                    // std::lock_guard<std::mutex> lock(coutMutex);
                    Timer::endTimer(Timer::BLIT_TO_SCREEN, t);
                }
            }

            // Step 4: Read pixels from the resolved FBO for off-screen
            // encoding
            // -----------------------------------------------------------------
            {
                encodeDiff = crntTime - encodeTime;
                if (vsync || (encodeDiff >= 1.0 / framerate)) {
                    // fbo first needs to be flipped for encoding
                    if (flip_shader) {
                        if (wireframe)
                            glPolygonMode(GL_FRONT_AND_BACK,
                                          GL_FILL); // Disable wireframe
                                                    // for this pass
                        Timer::startTimer(t);
                        glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
                        flipShader.use();
                        glBindVertexArray(dummyVAO);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, fboTexture);
                        flipShader.setInt("screenTexture", 0);
                        glDrawArrays(GL_TRIANGLES, 0, 3);
                        glBindVertexArray(0);
                        glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
                        {
                            // std::lock_guard<std::mutex>
                            // lock(coutMutex);
                            Timer::endTimer(Timer::FLIP_SHADER, t);
                        }
                        if (wireframe)
                            glPolygonMode(GL_FRONT_AND_BACK,
                                          GL_LINE); // Restore wireframe
                    }
                    if (!pbo) { // PBO off
                        uint8_t *buffer = frameBuffers[currentWriteIndex]
                                              .get(); // get raw pointer
                        Timer::startTimer(t);
                        glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB,
                                     GL_UNSIGNED_BYTE, buffer);
                        {
                            // std::lock_guard<std::mutex>
                            // lock(coutMutex);
                            Timer::endTimer(Timer::GLREADPIXELS_PBO_OFF,
                                            t);
                        }
                        if (!flip_shader) {
                            Timer::startTimer(t);
                            encoder->flipFrameVertically(buffer);
                            {
                                std::lock_guard<std::mutex> coutLock(
                                    coutMutex);
                                Timer::endTimer(Timer::FLIP_FUNCTION, t);
                            }
                        }
                        if (encoder_thread) {
                            encoder->pushFrame(buffer, crntTime,
                                               DATA_SIZE);
                        } else {
                            Timer::startTimer(t);
                            encoder->encodeFrame(buffer, crntTime);
                            {
                                std::lock_guard<std::mutex> coutLock(
                                    coutMutex);
                                Timer::endTimer(Timer::ENCODE, t);
                            }
                        }
                    } else { // PBO on
                        Timer::startTimer(t);
                        glBindBuffer(GL_PIXEL_PACK_BUFFER,
                                     pboIds[firstIndex]);
                        glBufferData(
                            GL_PIXEL_PACK_BUFFER, DATA_SIZE, nullptr,
                            GL_STREAM_READ); // Orphan first to ensure a
                                             // new backing store is
                                             // created
                        glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB,
                                     GL_UNSIGNED_BYTE, 0);
                        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
                        {
                            // std::lock_guard<std::mutex>
                            // lock(coutMutex);
                            Timer::endTimer(Timer::GLREADPIXELS_PBO_ON,
                                            t);
                        }

                        glBindBuffer(GL_PIXEL_PACK_BUFFER,
                                     pboIds[nextIndex]);
                        GLubyte *ptr = pboBuffers[currentWriteIndex];
                        ptr = (GLubyte *)glMapBufferRange(
                            GL_PIXEL_PACK_BUFFER, 0, DATA_SIZE,
                            GL_MAP_READ_BIT);

                        if (ptr) {
                            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
                            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

                            if (!flip_shader) {
                                Timer::startTimer(t);
                                encoder->flipFrameVertically(ptr);
                                {
                                    std::lock_guard<std::mutex> coutLock(
                                        coutMutex);
                                    Timer::endTimer(Timer::FLIP_FUNCTION,
                                                    t);
                                }
                            }
                            if (encoder_thread) {
                                encoder->pushFrame(ptr, crntTime,
                                                   DATA_SIZE);
                            } else {
                                Timer::startTimer(t);
                                encoder->encodeFrame(ptr, crntTime);
                                {
                                    std::lock_guard<std::mutex> coutLock(
                                        coutMutex);
                                    Timer::endTimer(Timer::ENCODE, t);
                                }
                            }
                        }
                        // Rotate indices
                        firstIndex = (firstIndex + 1) % PBO_COUNT;
                        nextIndex = (firstIndex + 1) % PBO_COUNT;
                    }
                    currentWriteIndex =
                        (currentWriteIndex + 1) % BUFFER_COUNT;
                    encodeTime = crntTime;
                }
            }
        }

        // recording OFF
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glEnable(GL_DEPTH_TEST); // Needed for 3D rendering
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            Timer::startTimer(t);
            // static crate
            glm::vec3 translatenew = glm::vec3(0.0f, 0.0f, 0.0f);
            RenderCrate(ourShader, VAO, translatenew, crateTexture,
                        awesomeTexture, cubePositions, lightPos,
                        crntTime);

            // controllable crate
            glm::vec3 translate = glm::vec3(xOffset, yOffset, zOffset);
            RenderCrate(ourShader, VAO, translate, crateTexture,
                        awesomeTexture, cubePositions, lightPos,
                        crntTime);

            // draw the light cube object
            lightShader.use();
            lightShader.setMat4("projection", projection);
            view = camera.GetViewMatrix();
            lightShader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
            lightShader.setMat4("model", model);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            {
                // std::lock_guard<std::mutex> lock(coutMutex);
                Timer::endTimer(Timer::RENDER_SCENE, t);
            }

            // ULTRALIGHT
            // --------------------------------------------
            browser.update();
            browser.render();

            unsigned char *pixels = browser.getPixels();

            if (pixels) {
                glBindTexture(GL_TEXTURE_2D, browserTex);

                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                                browser.getWidth(), browser.getHeight(),
                                GL_BGRA, GL_UNSIGNED_BYTE, pixels);

                browser.unlockPixels();
            }

            // Render quad
            browserShader.use();

            browserShader.setMat4("model", model_browser);
            browserShader.setMat4("view", view);
            browserShader.setMat4("projection", projection);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, browserTex);
            browserShader.setInt("browserTexture", 0);

            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            // ----------------------------------------------

            // --- CEF (same VAO, different tex + transform) ---
            auto &rh = client->renderHandler;
            if (rh->dirty) {
                rh->dirty = false;
                glBindTexture(GL_TEXTURE_2D, cefTex);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rh->width,
                                rh->height, GL_BGRA, GL_UNSIGNED_BYTE,
                                rh->pixelBuffer.data());
            }
            browserShader.use(); // same shader works fine too
            browserShader.setMat4("model", model_cef);
            browserShader.setMat4("view", view);
            browserShader.setMat4("projection", projection);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cefTex);
            browserShader.setInt("browserTexture", 0);
            glBindVertexArray(quadVAO); // reused again
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            // -------------------------------------------------

            // Render text in front: https://stackoverflow.com/a/5527249
            // glClear(GL_DEPTH_BUFFER_BIT);
            if (currentTextMode == TextTriState::TextAndAtlasON ||
                currentTextMode == TextTriState::TextONAtlasOFF)
                // textRenderer.renderText(fpsText, x, y, scale, color,
                // font);
                textRenderer.renderTextFast(fpsText, x, y, scale, color,
                                            font);
            if (currentTextMode == TextTriState::TextAndAtlasON)
                textRenderer.renderAtlas(font);

// IMGUI
// ----------------------------------------------------
#if IMGUI == 1
            if (imgui) {
                Timer::startTimer(t);
                GUI::Render(encoder.get());
                {
                    // std::lock_guard<std::mutex> lock(coutMutex);
                    Timer::endTimer(Timer::RENDER_GUI, t);
                }
            }
#endif /* IMGUI==1 */
            // ----------------------------------------------------
        }

        // IMGUI (won't be visible in screen recording, but doesn't mess
        // up when window is resized)
        // ----------------------------------------------------------------------------------------
        // #if IMGUI==1
        // if (imgui) {
        //     Timer::startTimer(t);
        //     GUI::Render();
        //     {
        //         // std::lock_guard<std::mutex> lock(coutMutex);
        //         Timer::endTimer(Timer::RENDER_GUI, t);
        //     }
        // }
        // #endif

        // glfw: swap buffers and poll IO events (keys pressed/released,
        // mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(
            window); // swap the BACK buffer with the FRONT buffer
    }

    // std::cout << "[main] Setting recording to false\n";
    recording = false;
    // need to stop encoder BEFORE deleting VAO/VBO/frame etc.
    // std::cout << "[main] Stopping encoder\n";
    encoder->stop();

    // optional: de-allocate all resources once they've outlived their
    // purpose:
    // ------------------------------------------------------------------------
    // std::cout << "[main] Deleting VAO, VBO, shaders\n";
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    // Delete all the shader programs we've created
    ourShader.Delete();
    lightShader.Delete();

    // std::cout << "[main] Deleting PBOs, FBOs, RBOs\n";
    glDeleteBuffers(PBO_COUNT, pboIds);
    glDeleteFramebuffers(1, &fboMsaaId);
    glDeleteFramebuffers(1, &fboId);
    glDeleteRenderbuffers(1, &rboMsaaColorId);
    glDeleteRenderbuffers(1, &rboMsaaDepthId);
    // glDeleteRenderbuffers(1,&rboId);

    Timer::printAverages();

// IMGUI
// -----
#if IMGUI == 1
    GUI::Exit();
#endif

    // CEF
    // ---
    CefShutdown();

    // Delete window before ending the program
    glfwDestroyWindow(window);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    // std::cout << "Press ENTER to exit...";
    // std::cin.get(); // waits for the user to press Enter

    return 0;
}

std::string GetFPSText(float fps, float ms, float crntTime) {
    using namespace Settings;

    char buffer[250]; // small stack-allocated array (extremely fast, in
                      // CPU cache)

    bool press = pboPressed || flipPressed || pausePressed ||
                 vsyncPressed || wireframePressed || imguiPressed ||
                 atlasPressed || recordPressed || encoderPressed;

    snprintf(buffer, sizeof(buffer),
             "FPS: %u | %.3f ms | Time: %.1f s | mix(Q/E)=%.1f | "
             "FOV=%.1f | Encoder thread(T)=%s | PBO(P)=%s | Flip "
             "Shader(F)=%s | Vsync(V)=%s | Fullscreen(F11)=%s | PRESS=%s "
             "| pos=%.1f,%.1f,%.1f | cam=%.1f,%.1f,%.1f | YAW= %.1f | "
             "PITCH=%.1f | %s",
             static_cast<int>(fps), ms, crntTime, mixValue, camera.Zoom,
             encoder_thread ? "ON" : "OFF", pbo ? "ON" : "OFF",
             flip_shader ? "ON" : "OFF", vsync ? "ON" : "OFF",
             fullscreen ? "ON" : "OFF", press ? "YES" : "NO", xOffset,
             yOffset, zOffset, camera.Position.x, camera.Position.y,
             camera.Position.z, camera.Yaw, camera.Pitch,
             paused ? "PAUSED" : "");

    return std::string(buffer);
}

void UpdateFPS(float &fps, float &ms, float crntTime, float &lastTime,
               int &frameCountFPS) {
    frameCountFPS++;

    // Calculate FPS every second
    if (crntTime - lastTime >= 1.0f / 1.0f) {
        fps = frameCountFPS / (crntTime - lastTime);
        ms = 1000.0f / fps; // Convert to milliseconds
        frameCountFPS = 0;
        lastTime = crntTime;
    }
}

void RenderCrate(Shader &ourShader, GLuint VAO, const glm::vec3 &trans,
                 GLuint crateTexture, GLuint awesomeTexture,
                 const std::array<glm::vec3, 10> &cubePositions,
                 glm::vec3 &lightPos, float crntTime) {
    using namespace Settings;

    ourShader.use();
    ourShader.setFloat("mixValue", mixValue);

    // bind Texture
    // ------------
    // need to rebind every time in render loop
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crateTexture);
    ourShader.setInt("texture1", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, awesomeTexture);
    ourShader.setInt("texture2", 1);

    // transform
    // ---------
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 transform_mat = glm::translate(identity, trans);
    // transform_mat = glm::translate(transform_mat,
    // glm::vec3(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY,
    // 0)); trans = glm::rotate(trans, (float)glfwGetTime(),
    // glm::vec3(0.0f, 0.0f, 1.0f));
    ourShader.setMat4("transform", transform_mat);

    // model
    // -----
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
                        glm::vec3(0.5f, 1.0f, 0.0f));
    ourShader.setMat4("model", model);

    // view
    // ----
    // moveable camera + swivel
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("view", view);

    // projection
    // ----------
    // pass projection matrix to shader (note that in this case it could
    // change every frame)
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom),
        static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
        0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    // light
    // -----
    float radius = 8.0f;
    float theta = crntTime * 0.5f; // slower rotation (Y-axis)
    float phi =
        glm::radians(90.0f + sin(crntTime * 0.6f) *
                                 90.0f); // varies between 0° and 180°

    lightPos.x = radius * sin(phi) * cos(theta);
    // lightPos.y = radius * cos(phi);
    lightPos.y =
        1.5f + glm::sin(crntTime * 0.7f) * 2.0f; // modulate the height
    lightPos.z = radius * sin(phi) * sin(theta);

    // simple circle in XZ plane
    // lightPos.x = glm::cos(crntTime) * 8.0f;
    // lightPos.y = 1.5f;
    // lightPos.z = glm::sin(crntTime) * 8.0f;

    ourShader.setVec3("lightPos", lightPos);
    ourShader.setVec3(
        "viewPos",
        camera.Position); // for world-space, not needed for view-space

    glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // render the crate
    // ----------------
    for (unsigned int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(
            model, crntTime * glm::radians(angle),
            cubePositions[i]); // glm::vec3(1.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
