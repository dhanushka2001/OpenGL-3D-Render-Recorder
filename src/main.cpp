// ImGui
// -----
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
//ImPlot
//------
#include <implot/implot.h>
// glad & GLFW
// -----------
#include <glad/glad.h>                  // glad
#include <GLFW/glfw3.h>                 // GLFW (includes stdint.h)

#ifdef _WIN32
#ifndef  NOMINMAX
#define  NOMINMAX                       // disable Windows min/max macro (interferes with C++ std::min/max)
#endif  /* NOMINMAX */
#include <Windows.h>                    // For V-Sync (unfortunately tied to Windows :( )
#include <wglext.h>                     // For V-Sync
#endif /* _WIN32 */

#include <learnopengl/shader_s.h>       // Shader class
#include <learnopengl/camera.h>         // Camera class
#include <learnopengl/encoder.h>        // FFmpeg functions
#include <learnopengl/fontmanager.h>    // Font Manager
#include <learnopengl/textrenderer.h>   // Text Renderer
#include <learnopengl/Config.h>         // Config
#include <learnopengl/timer.h>          // Timer

#include <iostream>                     // for std::cin/cout/cerr
#include <filesystem>                   // for std::filesystem
#include <iomanip>                      // for std::precision(3)              
#include <cstdio>                       // For sprintf
#define  M_PI           3.14159265358979323846
#include <cmath>
#define  STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define  STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define  GL_GLEXT_PROTOTYPES 1
#include <array>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#ifndef  PATH_MAX
#define  PATH_MAX 4096
#endif  /* PATH_MAX */

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
std::string GetFPSText(float fps, float ms);
void UpdateFPS(float crntTime);
#ifdef _WIN32
bool WGLExtensionSupported(const char *extension_name);
#endif
void RenderFullscreenQuad(Shader &quadShader, GLuint &quadTexture);
void RenderCrate(Shader &ourShader, glm::vec3 &trans);
// FFmpeg
void flipFrameVertically(unsigned char* frame);

// settings
// --------
namespace {     // anonymous namespace (encapsulation)
    unsigned int        SCR_WIDTH;
    unsigned int        SCR_HEIGHT;
    unsigned int        framerate;
    bool                fullscreen;
    int                 vsync;
    bool                pbo;
    bool                paused;
    bool                msaa;
    bool                recording;
    const unsigned int  CHANNEL_COUNT   =  3;
    unsigned int        PBO_COUNT;
    bool                flip_shader;
}

#define             RENDER_3D           1   // make sure to change in the shaders too
#define             RENDER_EBO          0   // only matters when RENDER_3D=1
#define             IMGUI               1

// fbo settings
// ------------
GLuint fboMsaaId, rboMsaaColorId, rboMsaaDepthId;
GLuint fboId, rboId;
GLuint fboTexture;

// textures
// --------
GLuint crateTexture, awesomeTexture;

// uniform variables
// -----------------
float   mixValue    =   0.3f;
float   xOffset     =   0.0f;
float   yOffset     =   0.0f;
float   zOffset     =   0.0f;
float   fov         =  45.0f;
float   camX        =   0.0f;
float   camY        =   0.0f;
float   camZ        =  -4.5f;

// camera
// ------
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float   lastX       = 0;
float   lastY       = 0;
double lastXpos     = 0;
double lastYpos     = 0;
bool    firstMouse  = true;

// timing
// ------
float encodeDiff, encodeTime = 0.0f;
float deltaTime, oldTime = 0.0f;
float timeDiff, prevTime = 0.0f;
float dt, old_t = 0.0f;
float crntTime = 0.0f;
// Keeps track of the amount of frames in timeDiff
unsigned int counter = 0;
// Variables for timing
float lastTime = 0.0f;
int frameCountFPS = 0;
float fps = 0.0f;
float msPerFrame = 0.0f;

// button press
// ------------
bool pboPressed = false;
bool flipPressed = false;
bool pausePressed = false;
bool vsyncPressed = false;

// window
// ------
int window_xPos, window_yPos = 0;
int window_width, window_height;
int left, top, right, bottom;

// viewport
// --------
int lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY = 0;

// IMGUI
// -----
// ImVec2 imgui_pos;

// light source
// ------------
glm::vec3 lightPos(3.2f, 5.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(1.0f, 0.5f, 0.31f);

GLuint VAO, VBO, EBO;

unsigned char* frame = NULL;
unsigned char* frame_old = NULL;
unsigned char* buffer = NULL;

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  -2.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    // Initialise GLFW
    if( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // OpenGL 4.3 => major=4,minor=3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); 

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Set window size (needs to be fixed when recording)
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    // Config::SetScreenResolution(mode->width, mode->height);
    Config::SetScreenResolution(1600, 900); // if the resolution is too low, FPS could get too high (~1000fps) and program will crash
    // Config::SetScreenResolution(1920, 1080);

    // Set our local cached values
    SCR_WIDTH   = Config::GetScreenWidth();
    SCR_HEIGHT  = Config::GetScreenHeight();
    framerate   = Config::GetFramerate();
    fullscreen  = Config::GetFullscreen();
    pbo         = Config::GetPBO();
    paused      = Config::GetPaused();
    msaa        = Config::GetMSAA();
    recording   = Config::GetRecording();
    flip_shader = Config::GetFlipShader();
    vsync       = Config::GetVsync();

    lastX = SCR_WIDTH / 2;
    lastY = SCR_HEIGHT / 2;
    window_width = SCR_WIDTH;
    window_height = SCR_HEIGHT;

    // pbo settings
    // ------------
    GLuint firstIndex = 0;
    GLuint nextIndex = 1;//(firstIndex + 1) % PBO_COUNT;
    unsigned int frameCounter = 0;
    PBO_COUNT = Config::GetPBO_COUNT();
    GLuint pboIds[PBO_COUNT];
    GLsync pboFences[PBO_COUNT] = { nullptr };
    unsigned int DATA_SIZE;

    // glfw window creation
    // --------------------
    GLFWwindow* window;

    if (!fullscreen)
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); // windowed
    else
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", glfwGetPrimaryMonitor(), NULL); // fullscreen
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetCursorPos(window, lastX,lastY);  // set cursor in centre of screen to remove whiplash cursor jump
    glfwSetScrollCallback(window, scroll_callback);

    
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);  // Hidden window
    GLFWwindow* sharedContextWindow = glfwCreateWindow(1, 1, "", nullptr, window); // Pass mainWindow as 5th param = share context
    

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // V-SYNC: Initialize function pointers
    // ------------------------------------
    // https://stackoverflow.com/a/589232
    #ifdef _WIN32
    PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT    = NULL;
    PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;

    if (WGLExtensionSupported("WGL_EXT_swap_control"))
    {
        // Extension is supported, init pointers.
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");

        // this is another function from WGL_EXT_swap_control extension
        wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");
    }
    // 0 = V-Sync Off, 1 = V-Sync On, -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz)
    // https://www.khronos.org/opengl/wiki/Swap_Interval
    // wglSwapIntervalEXT(vsync);
    std::cout << "\nwglSwapIntervalEXT: " << wglGetSwapIntervalEXT() << std::endl;
    #endif /* _WIN32 */

    // build and compile our shader program to render the crates
    // ---------------------------------------------------------
    Shader ourShader("crates.vert", "crates.frag"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    #if RENDER_3D==0
    float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    #endif  /* RENDER_3D==0 */
    
    #if RENDER_3D==1
    // 3D cube without EBO (6 faces * 2 triangles * 3 vertices each = 36 vertices. 8 unique vertices. 78% redundancy) (each face has its own unique texture coords)
    #if RENDER_EBO==0
    float vertices[] = {
      // positions            normal             texture coords
        // -Z face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,   // (-,-,-) 0
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,   // (+,-,-) 1
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,   // (+,+,-) 2
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,   // (+,+,-) 2
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,   // (-,+,-) 3
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,   // (-,-,-) 0

        // +Z face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,   // (-,-,+) 4
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,   // (+,-,+) 5
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,   // (+,+,+) 6
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,   // (+,+,+) 6
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,   // (-,+,+) 7
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,   // (-,-,+) 4

        // -X face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   // (-,+,+) 7
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,   // (-,+,-) 3
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   // (-,-,-) 0
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   // (-,-,-) 0
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,   // (-,-,+) 4
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   // (-,+,+) 7

        // +X face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   // (+,+,+) 6
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,   // (+,+,-) 2
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   // (+,-,-) 1
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   // (+,-,-) 1
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,   // (+,-,+) 5
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   // (+,+,+) 6

        // -Y face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,   // (-,-,-) 0
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,   // (+,-,-) 1
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,   // (+,-,+) 5
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,   // (+,-,+) 5
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,   // (-,-,+) 4
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,   // (-,-,-) 0

        // +Y face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,   // (-,+,-) 3
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,   // (+,+,-) 2
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,   // (+,+,+) 6
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,   // (+,+,+) 6
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,   // (-,+,+) 7
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f    // (-,+,-) 3
    };
    #endif  /* RENDER_EBO==0 */
    // 3D cube with EBO (8 unique vertices. Each vertex is shared by 3 faces. One vertex's texture coords for one face are being shared for the two other neighboring faces, rather than them having their own unique texture)
    #if RENDER_EBO==1
    float vertices[] = {
        // positions          // texture coords
        // -Z face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // (-,-,-) 0
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // (+,-,-) 1
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   // (-,+,-) 3

        // +Z face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // (+,-,+) 5
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // (+,+,+) 6
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   // (-,+,+) 7
    };
    unsigned int indices[] = {  
        // -Z face
        0, 1, 2,
        2, 3, 0,
        
        // +Z face
        4, 5, 6,
        6, 7, 4,

        // -X face
        7, 3, 0,
        0, 4, 7,

        // +X face
        6, 2, 1,
        1, 5, 6,

        // -Y face
        0, 1, 5,
        5, 4, 0,

        // +Y face
        3, 2, 6,
        6, 7, 3
    };
    #endif  /* RENDER_EBO==1 */
    #endif  /* RENDER_3D==1 */

    // unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    #if RENDER_EBO==1 || RENDER_3D==0
    glGenBuffers(1, &EBO);
    #endif
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // once a VAO is bound, any subsequent vertex attribute calls will be stored inside that VAO. (only have to make the calls once)
    glBindVertexArray(VAO);

    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // The last element buffer object (EBO) that gets bound while a VAO is bound is stored as that VAO's EBO.
    // Binding to a VAO then also automatically binds that EBO.
    #if RENDER_EBO==1 || RENDER_3D==0
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    #endif

    #if RENDER_3D==0
    // 2D
    // --
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    #endif /* RENDER_3D==0 */

    #if RENDER_3D==1
    // 3D
    // --
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    #endif /* RENDER_3D==1 */

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // load and create a texture 
    // -------------------------

    // texture 1
    // ---------
    glGenTextures(1, &crateTexture);
    glBindTexture(GL_TEXTURE_2D, crateTexture); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("../assets/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 2
    // ---------
    glGenTextures(1, &awesomeTexture);
    glBindTexture(GL_TEXTURE_2D, awesomeTexture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("../assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the shader class:
    ourShader.setInt("texture2", 1);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
    ourShader.setMat4("model", model);
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
    ourShader.setMat4("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
    // glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
    ourShader.setMat4("projection", projection);
    ourShader.setVec3("lightColor", lightColor);
    ourShader.setVec3("objectColor", objectColor);
    ourShader.setVec3("lightPos", lightPos);
    ourShader.setVec3("viewPos", camera.Position);

    // bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crateTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, awesomeTexture);
    
    if (recording)
    {
        // PBO OFF
        // -------
        frame = new unsigned char[SCR_WIDTH * SCR_HEIGHT * 3];

        // PBO ON
        // ------
        // create 2 pixel buffer objects, you need to delete them when program exits.
        // glBufferData() with NULL pointer reserves only memory space.
        DATA_SIZE = SCR_WIDTH * SCR_HEIGHT * CHANNEL_COUNT;
        glGenBuffers(PBO_COUNT, pboIds);
        for (int i = 0; i < PBO_COUNT; ++i) {
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[i]);
            glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, nullptr, GL_STREAM_READ); // STREAM_READ is better here
        }
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        //Somewhere at initialization
        /*  Framebuffer */
        glGenFramebuffers(1,&fboMsaaId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

        /* 4x MSAA renderbuffer object for colorbuffer */
        unsigned int MSAA = 0;
        if (msaa) {
            MSAA = 4;
        }
        glGenRenderbuffers(1,&rboMsaaColorId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboMsaaColorId);
        /* Storage must be one of: */
        /* GL_RGBA4, GL_RGB565, GL_RGB5_A1, GL_DEPTH_COMPONENT16, GL_STENCIL_INDEX8. */
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_RGBA4, SCR_WIDTH, SCR_HEIGHT);
        // attach colorbuffer image to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
                                  GL_COLOR_ATTACHMENT0, // 2. color attachment point
                                  GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
                                  rboMsaaColorId);      // 4. rbo ID
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        
        /* 4x MSAA renderbuffer object for depthbuffer */
        glGenRenderbuffers(1, &rboMsaaDepthId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboMsaaDepthId);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

        // attach depthbuffer image to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
                                  GL_DEPTH_ATTACHMENT,  // 2. depth attachment point
                                  GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
                                  rboMsaaDepthId);      // 4. rbo ID
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Check if the MSAA FBO is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer fboMsaaId is not complete!\n" << std::endl;
        }


        // create a normal (no MSAA) FBO to hold a render-to-texture
        glGenFramebuffers(1, &fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        // create a texture object
        glGenTextures(1, &fboTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, fboTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Before drawing
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        // attach the texture to FBO color attachment point
        glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                               GL_COLOR_ATTACHMENT0,  // 2. attachment point
                               GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                               fboTexture,            // 4. tex ID
                               0);                    // 5. mipmap level: 0(base)

        // Set the list of draw buffers.
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers                       

        // glGenRenderbuffers(1, &rboId);
        // glBindRenderbuffer(GL_RENDERBUFFER, rboId);
        // glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, SCR_WIDTH, SCR_HEIGHT);
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboId);
        // glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // attach the renderbuffer to depth attachment point
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
        //                           GL_DEPTH_ATTACHMENT, // 2. attachment point
        //                           GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
        //                           rboId);              // 4. rbo ID

        // Check if the FBO is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer fboId is not complete!\n" << std::endl;
        }
    }

    // light source
    // ------------
    Shader lightShader("light.vert", "light.frag");
    lightShader.use();
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube), light_cube, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // scale by 0.2
    lightShader.setMat4("model", model);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);

    // flip shader
    // -----------
    GLuint fboFlip, fboFlipTexture, quadVAO, quadVBO;
    glGenFramebuffers(1, &fboFlip);
    glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);

    glGenVertexArrays(1, &quadVAO);

    // Create the texture to attach to fboFlip
    glGenTextures(1, &fboFlipTexture);
    glBindTexture(GL_TEXTURE_2D, fboFlipTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboFlipTexture, 0);

    // Check framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR: fboFlip is not complete!\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader flipShader("flip.vert", "flip.frag");

    // IMGUI
    // -----
    #if IMGUI==1
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    #endif
    
    glEnable(GL_BLEND); // enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // TEXT
    // ----
    FT_UInt fontSize = 48;
    // Render FPS text at the top-left corner
    float scale = static_cast<float>(SCR_WIDTH)*0.16f/800.0f;
    // Position on the screen
    float x = lowerLeftCornerOfViewportX;
    float y = lowerLeftCornerOfViewportY + static_cast<float>(SCR_HEIGHT) - 35.0f * scale * fontSize/48.0f; // Invert Y-axis since OpenGL origin is bottom-left
    glm::vec3 color(1.0f, 1.0f, 1.0f); // White text

    FontManager fontManager;
    fontManager.loadFont("Arial", 48);

    TextRenderer textRenderer(fontManager);
    
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


    // FrameData holds a copy of the frame and its timestamp
    struct FrameData {
        // std::vector<uint8_t> pixels;  // Make a deep copy of PBO data
        unsigned char *frame;
        double pts;
        GLuint pboIndex;
    };

    // A lock-free queue is overkill here — simple mutex + condition_variable works well
    std::queue<FrameData> frameQueue;
    std::mutex queueMutex;
    std::condition_variable queueCond;
    std::atomic<bool> recordingNew = true;
    const size_t MAX_QUEUE_SIZE = 8;

    // std::mutex encoderMutex;
    std::mutex coutMutex;
    Timer::init();
    std::chrono::high_resolution_clock::time_point t;

    // create output directory
    namespace fs = std::filesystem;
    fs::path outputDir = "../output";
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    // encoder thread
    std::thread encoderThread([&]() {
        glfwMakeContextCurrent(sharedContextWindow);  // Make encoder's context current here
        gladLoadGL(); // Needed again in this thread!
        try {
            // Step 1: Initialize encoder
            if (!Encoder::initializeEncoder("../output/output.mp4")) {
                std::cerr << "Failed to initialize encoder in thread\n";
                return;
            }
    
            // Step 2: Process frame queue
            while (recordingNew || !frameQueue.empty()) {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCond.wait(lock, [&]() {
                    return !frameQueue.empty() || !recordingNew;
                });
    
                while (!frameQueue.empty()) {
                    FrameData frame = std::move(frameQueue.front());
                    frameQueue.pop();
                    
                    if (!pbo) {
                        lock.unlock();  // unlock while processing
                        if (!flip_shader) {
                            Timer::startTimer(t);
                            flipFrameVertically(frame.frame);
                            {
                                std::lock_guard<std::mutex> lock(coutMutex);
                                Timer::endTimer(Timer::FLIP_FUNCTION, t);
                            }
                        }
                        // lock.lock(); // relock for logging
                        {
                            std::lock_guard<std::mutex> encoderLock(Encoder::encoderMutex);
                            Timer::startTimer(t);
                            // Encoder::encodeFrame(frame.pixels.data(), frame.pts);
                            Encoder::encodeFrame(frame.frame, frame.pts);
                            {
                                std::lock_guard<std::mutex> lock(coutMutex);
                                Timer::endTimer(Timer::ENCODE, t);
                            }
                        }
                        // lock.unlock();  // unlock
                        lock.lock(); // relock for queue
                    }

                    if (pbo) {
                        lock.unlock();  // unlock while processing
                        // Wait for sync to be ready
                        int index = frame.pboIndex;
                        if (pboFences[index]) {
                            // GLenum waitReturn = glClientWaitSync(pboFences[index], 0, GL_TIMEOUT_IGNORED);
                            GLenum waitReturn = glClientWaitSync(pboFences[index], GL_SYNC_FLUSH_COMMANDS_BIT, 1'000'000'000); // 1s timeout
                            if (waitReturn == GL_ALREADY_SIGNALED || waitReturn == GL_CONDITION_SATISFIED) {
                                glDeleteSync(pboFences[index]);
                                pboFences[index] = nullptr;
                            } else {
                                std::cerr << "[encoderThread] glClientWaitSync timeout or error\n";
                                continue;
                            }
                        }

                        // Map and copy pixels from PBO
                        glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
                        GLubyte* ptr = (GLubyte*)glMapBufferRange(
                            GL_PIXEL_PACK_BUFFER, 0, DATA_SIZE,
                            GL_MAP_READ_BIT
                        );

                        if (ptr) {
                            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
                            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

                            if (!flip_shader) {
                                Timer::startTimer(t);
                                flipFrameVertically(ptr);
                                {
                                    std::lock_guard<std::mutex> lock(coutMutex);
                                    Timer::endTimer(Timer::FLIP_FUNCTION, t);
                                }
                            }
                            // lock.lock(); // relock for logging
                            {
                                std::lock_guard<std::mutex> encoderLock(Encoder::encoderMutex);
                                Timer::startTimer(t);
                                // Encoder::encodeFrame(frame.pixels.data(), frame.pts);
                                Encoder::encodeFrame(ptr, frame.pts);
                                {
                                    std::lock_guard<std::mutex> lock(coutMutex);
                                    Timer::endTimer(Timer::ENCODE, t);
                                }
                            }
                            // lock.unlock();  // unlock
                        }
                        lock.lock(); // relock for queue
                    }
                }
            }

            {
                std::lock_guard<std::mutex> encoderLock(Encoder::encoderMutex);
                Encoder::finalizeEncoder();
            }
        } catch (const std::exception& e) {
            std::cerr << "Encoder thread crashed: " << e.what() << std::endl;
        }
    });

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glfwMakeContextCurrent(window);
        // input
        // -----
        // glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        processInput(window);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << error << std::endl;
            break;
        }

        // render
        // ------
        // Set the clear color to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Clear the current framebuffer
        // At this point the entire framebuffer is black
        glClear(GL_COLOR_BUFFER_BIT);
        // Set the clear color to the background color of choice
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // background color
        
        // IMGUI
        // -----
        #if IMGUI==1
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        #endif

        // FPS Counter: https://www.youtube.com/watch?v=BA6aR_5C_BM
        // Updates counter and times
		crntTime = static_cast<float>(glfwGetTime());

        // for movement (time between frames)
        timeDiff = crntTime - prevTime;
        prevTime = crntTime;

        deltaTime = crntTime - oldTime;
		counter++;
		if (deltaTime >= 60.0 / 60.0)
		{
			// Creates new title
			std::string FPS = std::to_string((1.0 / deltaTime) * counter);
            // float FPSs = (1.0 / timeDiff) * counter;
			std::string ms = std::to_string((deltaTime / counter) * 1000);
            // float mss = (timeDiff / counter) * 1000;
			std::string newTitle = "LearnOpenGL - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			oldTime = crntTime;
            counter = 0;

			// Use this if you have disabled VSync
			//camera.Inputs(window);
		}

        // Update timing info
        UpdateFPS(crntTime);
        std::string fpsText = GetFPSText(fps, msPerFrame);

        // recording ON
        if (recording)
        {
            // Step 1: Render the scene to the MSAA FBO
            // ----------------------------------------
            glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
            glEnable(GL_DEPTH_TEST); // Needed for 3D rendering
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // static crate
            Timer::startTimer(t);
            glm::vec3 translatenew = glm::vec3(0.0f, 0.0f, 0.0f);
            RenderCrate(ourShader, translatenew);

            // controllable crate
            glm::vec3 translate = glm::vec3(xOffset, yOffset, zOffset);
            RenderCrate(ourShader, translate);

            // draw the light cube object
            // also draw the lamp object
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
                std::lock_guard<std::mutex> lock(coutMutex);
                Timer::endTimer(Timer::RENDER_SCENE, t);
            }
            
            // Step 2: Resolve MSAA FBO to standard non-MSAA FBO
            // -------------------------------------------------
            Timer::startTimer(t);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
            glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                              0, 0, SCR_WIDTH, SCR_HEIGHT,           // dest rect
                                      GL_COLOR_BUFFER_BIT,           // buffer mask
                                               GL_LINEAR);           // scale filter
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                Timer::endTimer(Timer::BLIT_MSAA, t);
            }

            Timer::startTimer(t);
            // textRenderer.renderText(fpsText, x, y, scale, color, "Arial");
            textRenderer.renderTextFast(fpsText, x, y, scale, color, "Arial");
            textRenderer.renderAtlas("Arial");
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                Timer::endTimer(Timer::RENDER_TEXT, t);
            }
        
            // IMGUI (visible in screen recording, messes up when window is resized)
            // ---------------------------------------------------------------------
            #if IMGUI==1
            Timer::startTimer(t);
            ImGui::Begin("My name is window, ImGUI window");
            ImGui::Text("Hello there adventurer!");
            ImGui::End();
            // Show the ImPlot demo window
            if (ImGui::Begin("ImPlot Demo")) {
                ImPlot::ShowDemoWindow();
            }
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                Timer::endTimer(Timer::RENDER_GUI, t);
            }
            #endif

            // Step 3: Render the scene on-screen using Blitting: https://stackoverflow.com/a/31487085
            // ---------------------------------------------------------------------------------------
            // Bind the target FBO to read
            glBindFramebuffer(GL_FRAMEBUFFER, fboId);
            Timer::startTimer(t);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                              0, 0, SCR_WIDTH, SCR_HEIGHT,           // dest rect
                                      GL_COLOR_BUFFER_BIT,           // buffer mask
                                              GL_NEAREST);           // scale filter
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                Timer::endTimer(Timer::BLIT_TO_SCREEN, t);
            }

            if (flip_shader) {
                Timer::startTimer(t);
                glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
                flipShader.use();
                glBindVertexArray(quadVAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, fboTexture);
                flipShader.setInt("screenTexture", 0);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                glBindVertexArray(0);
                glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
                {
                    std::lock_guard<std::mutex> lock(coutMutex);
                    Timer::endTimer(Timer::FLIP_SHADER, t);
                }
            }

            // PBO off
            if (!pbo)
            {
                // Step 4: Read pixels from the resolved FBO for off-screen encoding (without PBOs)
                // --------------------------------------------------------------------------------
                // glBindFramebuffer(GL_FRAMEBUFFER, fboId);
                Timer::startTimer(t);
                glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame);
                {
                    std::lock_guard<std::mutex> lock(coutMutex);
                    Timer::endTimer(Timer::GLREADPIXELS_PBO_OFF, t);
                }
                GLenum err = glGetError();
                if (err != GL_NO_ERROR)
                    std::cerr << "OpenGL error: " << std::hex << err << std::endl;


                encodeDiff = crntTime - encodeTime;
                if (encodeDiff >= 1.0 / framerate) {
                    // FrameData data;
                    // data.frame = frame;
                    // data.pts = crntTime;

                    // {
                    //     std::unique_lock<std::mutex> lock(queueMutex);

                    //     if (frameQueue.size() < MAX_QUEUE_SIZE) {
                    //         Timer::startTimer(t);
                    //         frameQueue.push(data);
                    //         queueCond.notify_one();
                    //         {
                    //             std::lock_guard<std::mutex> lock(coutMutex);
                    //             // std::cout << "[main] Queue size during push+wait: " << frameQueue.size() - 1 << "/" << MAX_QUEUE_SIZE << "\n";
                    //             Timer::endTimer(Timer::QUEUE_PUSH_WAIT_PBO_OFF, t);
                    //         }
                    //     } else {
                    //         {
                    //             std::lock_guard<std::mutex> lock(coutMutex);
                    //             // std::cout << "[main] Skipping frame due to full queue: " << frameQueue.size() << "/" << MAX_QUEUE_SIZE << "\n";
                    //         }
                    //     }
                    // }

                    // Timer::startTimer(t);
                    if (!flip_shader) {
                        flipFrameVertically(frame);
                    }
                    // {
                    //     std::lock_guard<std::mutex> lock(coutMutex);
                    //     Timer::endTimer(Timer::FLIP_FUNCTION, t);
                    // }
                    Encoder::encodeFrame(frame, crntTime);
                    // Resets times and counter
			        encodeTime = crntTime;
                }
            }
            // PBO on
            else
            {
                encodeDiff = crntTime - encodeTime;
                if (encodeDiff >= 1.0 / framerate) {
                    // float t0, t1;
                    // Step 4: Read pixels from FBO into PBO (glReadPixels() should return immediately)
                    // --------------------------------------------------------------------------------
                    Timer::startTimer(t);
                    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[firstIndex]);
                    glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, nullptr, GL_STREAM_READ); // Orphan first to ensure a new backing store is created
                    glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, 0);
                    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
                    {
                        std::lock_guard<std::mutex> lock(coutMutex);
                        Timer::endTimer(Timer::GLREADPIXELS_PBO_ON, t);
                    }
                    
                    // Insert a fence for the current PBO
                    if (pboFences[firstIndex]) {
                        glDeleteSync(pboFences[firstIndex]);
                    }
                    pboFences[firstIndex] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

                    // Push nextIndex (the one rendered last frame) to queue
                    FrameData data;
                    data.pboIndex = nextIndex;
                    data.pts = crntTime;

                    {
                        std::unique_lock<std::mutex> lock(queueMutex);

                        if (frameQueue.size() < MAX_QUEUE_SIZE) {
                            Timer::startTimer(t);
                            frameQueue.push(data);
                            queueCond.notify_one();
                            {
                                std::lock_guard<std::mutex> lock(coutMutex);
                                // std::cout << "[main] Queue size during push+wait: " << frameQueue.size() - 1 << "/" << MAX_QUEUE_SIZE << "\n";
                                Timer::endTimer(Timer::QUEUE_PUSH_WAIT_PBO_ON, t);
                            }
                        } else {
                            {
                                std::lock_guard<std::mutex> lock(coutMutex);
                                // std::cout << "[main] Skipping frame due to full queue: " << frameQueue.size() << "/" << MAX_QUEUE_SIZE << "\n";
                            }
                        }
                    }

                    // Rotate indices
                    firstIndex = (firstIndex + 1) % PBO_COUNT;
                    nextIndex = (firstIndex + 1) % PBO_COUNT;

                    // Resets times and counter
			        encodeTime = crntTime;
                }
            }
        }
        
        // recording OFF
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glEnable(GL_DEPTH_TEST); // Needed for 3D rendering
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // static crate
            glm::vec3 translatenew = glm::vec3(0.0f, 0.0f, 0.0f);
            RenderCrate(ourShader, translatenew);

            // controllable crate
            glm::vec3 translate = glm::vec3(xOffset, yOffset, zOffset);
            RenderCrate(ourShader, translate);

            // draw the light cube object
            // also draw the lamp object
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

            // Render text in front: https://stackoverflow.com/a/5527249
            // glClear(GL_DEPTH_BUFFER_BIT);
            textRenderer.renderText(fpsText, x, y, scale, color, "Arial");
            textRenderer.renderAtlas("Arial");

            // IMGUI
            // -----
            #if IMGUI==1
            ImGui::Begin("My name is window, ImGUI window");
            ImGui::Text("Hello there adventurer!");
            ImGui::End();
            // Show the ImPlot demo window
            if (ImGui::Begin("ImPlot Demo")) {
                ImPlot::ShowDemoWindow();
            }
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            #endif
        }

        // IMGUI (won't be visible in screen recording, but doesn't mess up when window is resized)
        // ----------------------------------------------------------------------------------------
        // #if IMGUI==1
        // ImGui::Begin("My name is window, ImGUI window");
        // ImGui::Text("Hello there adventurer!");
        // ImGui::End();
        // ImGui::Render();
        // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // #endif

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);    // swap the BACK buffer with the FRONT buffer
        glfwPollEvents();           // take care of all GLFW events
        glfwSwapInterval(vsync);    // vsync
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    #if RENDER_EBO==1 || RENDER_3D==0
    glDeleteBuffers(1, &EBO);
    #endif
    // De-allocate frame buffer (deletes all items in the array)
    delete[] frame;
    // Delete all the shader programs we've created
	ourShader.Delete();
    lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);

    Timer::printAverages();

    if (recording)
    {
        glDeleteBuffers(PBO_COUNT, pboIds);
        glDeleteFramebuffers(1,&fboMsaaId);
        glDeleteFramebuffers(1,&fboId);
        glDeleteRenderbuffers(1,&rboMsaaColorId);
        glDeleteRenderbuffers(1,&rboMsaaDepthId);
        // glDeleteRenderbuffers(1,&rboId);

        // Stop ffmpeg
        // -----------
        // Encoder::finalizeEncoder();

        recordingNew = false;
        queueCond.notify_all();
        encoderThread.join();
    }
    // IMGUI
    // -----
    #if IMGUI==1
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    #endif

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    // Exit
    // ----
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Control crate
    // -------------
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        yOffset += timeDiff * 1.0f;
        if(yOffset >= 200.0f)
            yOffset = 200.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        yOffset -= timeDiff * 1.0f;
        if (yOffset <= -200.0f)
            yOffset = -200.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        xOffset -= timeDiff * 1.0f;
        if(xOffset <= -200.0f)
            xOffset = -200.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        xOffset += timeDiff * 1.0f;
        if(xOffset >= 200.0f)
            xOffset = 200.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        zOffset -= timeDiff * 1.0f;
        if(zOffset <= -200.0f)
            zOffset = -200.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        zOffset += timeDiff * 1.0f;
        if(zOffset >= 200.0f)
            zOffset = 200.0f;
    }
    // Control camera
    // --------------
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        // camY += timeDiff * 5.0f;
        // if(camY >= 20.0f)
        //     camY = 20.0f;
        camera.ProcessKeyboard(DOWN, timeDiff);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        // camY -= timeDiff * 5.0f;
        // if (camY <= -20.0f)
        //     camY = -20.0f;
        camera.ProcessKeyboard(UP, timeDiff);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // camX -= timeDiff * 5.0f;
        // if(camX <= -20.0f)
        //     camX = -20.0f;
        camera.ProcessKeyboard(RIGHT, timeDiff);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // camX += timeDiff * 5.0f;
        // if(camX >= 20.0f)
        //     camX = 20.0f;
        camera.ProcessKeyboard(LEFT, timeDiff);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        // camZ -= timeDiff * 5.0f;
        // if(camZ <= -20.0f)
        //     camZ = -20.0f;
        camera.ProcessKeyboard(BACKWARD, timeDiff);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        // camZ += timeDiff * 5.0f;
        // if(camZ >= 20.0f)
        //     camZ = 20.0f;
        camera.ProcessKeyboard(FORWARD, timeDiff);
    }
    // Mix
    // ---
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        mixValue -= timeDiff;
        if(mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        mixValue += timeDiff;
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    // FOV
    // ---
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        fov -= timeDiff * 100.0f;
        if(fov <= 0.0f)
            fov = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    {
        fov += timeDiff * 100.0f;
        if(fov >= 180.0f)
            fov = 180.0f;
    }
    // Fullscreen
    // ----------
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {
        bool last_pause_state = paused;
        float lastYaw = camera.Yaw;
        float lastPitch = camera.Pitch;
        
        // if not paused, pause screen (to stop mouse drift)
        if (!paused)
        {
            // lock the mouse (stop YAW drift)
            glfwSetCursorPos(window, SCR_WIDTH/2.0f, SCR_HEIGHT/2.0f);  // set cursor in centre of screen to remove whiplash cursor jump
            // tell GLFW to uncapture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            last_pause_state = 0;
            paused = 1;
        }
        // fullscreen->windowed
        if (fullscreen)
        {
            int adjusted_width  = window_width;
            int adjusted_height = window_height + top;
            glfwSetWindowMonitor(window,
                                 NULL,
                                 window_xPos, window_yPos,
                                //  adjusted_width, adjusted_height,
                                //  window_width, window_height,
                                 SCR_WIDTH, SCR_HEIGHT,
                                 GLFW_DONT_CARE
            );
            fullscreen = 0;
        }
        // windowed->fullscreen
        else
        {
            glfwGetWindowPos(window, &window_xPos, &window_yPos);
            glfwGetWindowSize(window, &window_width, &window_height);
            glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window,
                                 glfwGetPrimaryMonitor(),
                                 0, 0,
                                //  mode->width, mode->height,      // viewport size accurate to window size
                                 SCR_WIDTH, SCR_HEIGHT,             // viewport fills window
                                 GLFW_DONT_CARE
            );
            fullscreen = 1;
        }
        // unpause
        if (last_pause_state==0)
        {
            // unlock the mouse
            glfwSetCursorPos(window, lastX, lastY);  // set cursor in centre of screen to remove whiplash cursor jump
            // tell GLFW to capture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            paused = 0;
            camera.Yaw = lastYaw;
            camera.Pitch = lastPitch;
        }
    }
    // Pause
    // -----
    if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) && (!pausePressed))
    {
        pausePressed = 1;

        // pause -> unpause
        if (paused)
        {
            firstMouse = true;
            lastXpos = static_cast<double>(lastX);
            lastYpos = static_cast<double>(lastY);
            glfwSetCursorPos(window, lastXpos, lastYpos);  // remove whiplash cursor jump
            // tell GLFW to capture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            paused = 0;
        }
        // unpause -> pause
        else
        {
            // tell GLFW to uncapture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            paused = 1;
        }
    }
    if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE) && (pausePressed)) {
        pausePressed = 0;
    }
    // PBO
    // ---
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && (!pboPressed)) {
        pboPressed = true;
        Config::TogglePBO();
        pbo = Config::GetPBO();
        std::cout << "P key pressed. Press = " << pboPressed << " Time: " << crntTime << std::endl;
        // std::cout << "P key pressed: " << !pbo << "->" << pbo << " Press: " << !pboPressed << "->" << pboPressed << " Time: " << crntTime << std::endl;
    }
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) && (pboPressed)) {
        pboPressed = false;
        std::cout << "P key released. Press = " << pboPressed << " Time: " << crntTime << std::endl;
    }
    // Flip Shader
    // -----------
    if ((glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) && (!flipPressed))
    {
        flipPressed = true;
        Config::ToggleFlipShader();
        flip_shader = Config::GetFlipShader();
        std::cout << "F key pressed. Press = " << flipPressed << " Time: " << crntTime << std::endl;
        // std::cout << "Toggled PBO: " << !pbo << "->" << pbo << std::endl;
    }
    if ((glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) && (flipPressed)) {
        flipPressed = false;
        std::cout << "F key released. Press = " << flipPressed << " Time: " << crntTime << std::endl;
    }
    // VSYNC
    // -----
    if ((glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) && (!vsyncPressed))
    {
        vsyncPressed = true;
        Config::ToggleVsync();
        vsync = Config::GetVsync();
        std::cout << "V key pressed. Press = " << vsyncPressed << " Time: " << crntTime << std::endl;
        // std::cout << "Toggled Vsync: " << !vsync << "->" << vsync << std::endl;
    }
    if ((glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) && (vsyncPressed)) {
        vsyncPressed = false;
        std::cout << "V key released. Press = " << vsyncPressed << " Time: " << crntTime << std::endl;
    }
}

// glfw: whenever window size changes (by OS or user) this callback function executes
// ----------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    
    // keep viewport size fixed (recording doesn't get messed up)
    // ----------------------------------------------------------
    // lowerLeftCornerOfViewportX = 0.5*(width - static_cast<int>(SCR_WIDTH));    // need to convert SCR_WIDTH/HEIGHT from unsigned int->int!
    // lowerLeftCornerOfViewportY = 0.5*(height - static_cast<int>(SCR_HEIGHT));
    // lowerLeftCornerOfViewportX = std::max(lowerLeftCornerOfViewportX, 0);
    // lowerLeftCornerOfViewportY = std::max(lowerLeftCornerOfViewportY, 0);
    
    window_width = width;
    window_height = height;
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // MAKE IT SO THE GAME PAUSES WHEN RESIZING WINDOW.

    // for debugging
    // std::cout << "viewportX: " << lowerLeftCornerOfViewportX << " "
    //           << "viewportY: " << lowerLeftCornerOfViewportY << std::endl;

    // glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    // glViewport(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, SCR_WIDTH, SCR_HEIGHT);

    #if IMGUI==1
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(100, 100+lowerLeftCornerOfViewportY*2));
    // ImGui::SetNextWindowPos(ImVec2(lowerLeftCornerOfViewportX, 100+lowerLeftCornerOfViewportY));//, ImGuiCond_FirstUseEver);
    ImGui::Begin("My name is window, ImGUI window");
    ImGui::Text("Hello there adventurer!");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    #endif

    // Update ImGui's projection and screen space alignment
    // UpdateImGuiProjection(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, SCR_WIDTH, SCR_HEIGHT);
    // ImGui::SetNextWindowPos(ImVec2(lowerLeftCornerOfViewportX + 10, lowerLeftCornerOfViewportY + 10));
    // UpdateImGuiProjection(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    // viewport stretches (recording gets messed up)
    // ---------------------------------------------
    // glViewport(0, 0, width, height);
    
    // scale viewport to always be inside window with correct aspect ratio (works nice when SCR_WIDTH and SCR_HEIGHT = monitor resolution)
    // -----------------------------------------------------------------------------------------------------------------------------------
    // if ((float)height/(float)width > (float)SCR_HEIGHT/(float)SCR_WIDTH)
    //     glViewport(0, 0.5*((float)height - (float)SCR_HEIGHT*(float)width/(float)SCR_WIDTH), width, width*(float)SCR_HEIGHT/(float)SCR_WIDTH);
    // else
    //     glViewport(0.5*((float)width - (float)SCR_WIDTH*(float)height/(float)SCR_HEIGHT), 0, height*(float)SCR_WIDTH/(float)SCR_HEIGHT, height);
    
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    // only update when unpaused
    if (!paused)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Flip the frame vertically
void flipFrameVertically(unsigned char* frame) {
    // for (unsigned int y = 0; y < SCR_HEIGHT / 2; y++) {
    //     int oppositeY = SCR_HEIGHT - 1 - y;
    //     for (unsigned int x = 0; x < SCR_WIDTH * 3; x++) {
    //         std::swap(frame[y * SCR_WIDTH * 3 + x], frame[oppositeY * SCR_WIDTH * 3 + x]);
    //     }
    // }

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

std::string GetFPSText(float fps, float ms) {
    char buffer[250];
    bool press = pboPressed || flipPressed || pausePressed || vsyncPressed;
    // "FPS: %.1f | %.1f ms"
    // "AaBbCcDdEeFfGg1!2£4$"
    // "In the dream, they took me to the light. A beautiful lie."
    snprintf(buffer, sizeof(buffer), "FPS: %u | %.3f ms | Time: %.1f s | mix(Q/E)=%.1f | FOV=%.1f | PBO(P)=%s | Flip Shader(F)=%s | Vsync(V)=%s | Fullscreen(F11)=%s | PRESS=%s | pos=%.1f,%.1f,%.1f | cam=%.1f,%.1f,%.1f | YAW= %.1f | PITCH=%.1f | %s", static_cast<int>(fps), ms, crntTime, mixValue, camera.Zoom, pbo ? "ON" : "OFF", flip_shader ? "ON" : "OFF", vsync ? "ON" : "OFF", fullscreen ? "ON" : "OFF", press ? "YES" : "NO", xOffset, yOffset, zOffset, camera.Position.x, camera.Position.y, camera.Position.z, camera.Yaw, camera.Pitch, paused ? "PAUSED" : "");
    return std::string(buffer);
}

void UpdateFPS(float crntTime) {
    frameCountFPS++;

    // Calculate FPS every second
    if (crntTime - lastTime >= 1.0f / 1.0f) {
        fps = frameCountFPS / (crntTime - lastTime);
        msPerFrame = 1000.0f / fps; // Convert to milliseconds
        frameCountFPS = 0;
        lastTime = crntTime;
    }
}

#ifdef _WIN32
bool WGLExtensionSupported(const char *extension_name)
{
    // this is pointer to function which returns pointer to string with list of all wgl extensions
    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

    // determine pointer to wglGetExtensionsStringEXT function
    _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) wglGetProcAddress("wglGetExtensionsStringEXT");

    if (strstr(_wglGetExtensionsStringEXT(), extension_name) == NULL)
    {
        // string was not found
        return false;
    }

    // extension is supported
    return true;
}
#endif

void RenderFullscreenQuad(Shader &quadShader, GLuint &quadTexture) {
    quadShader.use();
    // Enable 2D rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // bind VAO
    // glBindVertexArray(quadVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    // Bind the texture (the texture atlas in this case)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, quadTexture); // atlasTextureId is the texture containing the atlas
    quadShader.setInt("screenTexture", 0);
    // Draw the quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderCrate(Shader &ourShader, glm::vec3 &trans) {
    // set the texture mix value in the shader (this needs to be in the render loop)
    ourShader.use();
    ourShader.setFloat("mixValue", mixValue);

    // bind Texture
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
    // transform_mat = glm::translate(transform_mat, glm::vec3(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, 0));
    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    ourShader.setMat4("transform", transform_mat);

    // model
    // -----
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
    ourShader.setMat4("model", model);

    // moveable camera + swivel
    glm::mat4 view = camera.GetViewMatrix();
    
    ourShader.setMat4("view", view);

    // projection
    // ----------
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    // light
    // -----
    ourShader.setVec3("lightPos", lightPos);
    ourShader.setVec3("viewPos", camera.Position);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // render the crate
    // ----------------
    #if RENDER_3D==0
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    #endif
    #if RENDER_3D==1
    for(unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i; 
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), cubePositions[i]); //glm::vec3(1.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", model);

        #if RENDER_EBO==0
        glDrawArrays(GL_TRIANGLES, 0, 36);
        #endif
        #if RENDER_EBO==1
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        #endif
    }
    #endif

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
