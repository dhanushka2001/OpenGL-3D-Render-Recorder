// ImGui
// -----
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
// FFmpeg
// ------
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavutil/log.h>
}
#include <fstream>
#include <mutex>
// glad & GLFW
// -----------
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifndef  NOMINMAX
#define  NOMINMAX                   // disable Windows min/max macro (interferes with C++ std::min/max)
#endif
#include <Windows.h>                // For V-Sync (unfortunately tied to Windows :( )
#include <wglext.h>                 // For V-Sync
#include <learnopengl/shader_s.h>   // Shader class
#include <learnopengl/camera.h>     // Camera class
#include <iostream>
#include <filesystem>               // for std::filesystem
#include <iomanip>                  // for std::precision(3)              
#include <cstdio>                   // For sprintf
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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef  PATH_MAX
#define  PATH_MAX 4096
#endif
// FreeType
// --------
#include <ft2build.h>
#include FT_FREETYPE_H

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void startFFmpeg();
void appendBuffer(unsigned char *frame, unsigned char *frameBuffer, unsigned int frameCount);
void sendBlockBufferToFFmpeg(unsigned char* frameBuffer, int N);
void sendFrameBufferToFFmpeg(unsigned char *frame, unsigned int bufferSize);
void stopFFmpeg();
void flipFrameVertically(unsigned char* frame);
bool loadFont(FT_UInt &fontsize);
void createTextureAtlas();
void RenderText(Shader &textShader, const std::string &text, float x, float y, float scale, glm::vec3 color);
std::string GetFPSText(float fps, float ms);
void UpdateFPS(float crntTime);
bool WGLExtensionSupported(const char *extension_name);
void RenderAtlas(Shader &atlasShader, GLuint &atlasTexture);
void RenderFullscreenQuad(Shader &quadShader, GLuint &quadTexture);
void RenderCrate(Shader &ourShader, glm::vec3 &trans);
void custom_ffmpeg_log_callback(void *ptr, int level, const char *fmt, va_list args);
bool initializeEncoder(const char* filename);
bool encodeFrame(const uint8_t* rgbData);
void finalizeEncoder();

struct Glyph {
    float textureX, textureY;  // Texture coordinates in the atlas
    float width, height;       // Glyph's width and height
    float offsetX, offsetY;    // Offsets (for positioning)
    unsigned int advanceX;     // Horizontal advance (for spacing)
};

// settings
// --------
const unsigned int  SCR_WIDTH       = 800;//2560;  // 800;
const unsigned int  SCR_HEIGHT      = 600;//1440;  // 600;
const unsigned int  CHANNEL_COUNT   =  3;
const unsigned int  DATA_SIZE       = SCR_WIDTH * SCR_HEIGHT * CHANNEL_COUNT;
const unsigned int  PBO_COUNT       =  2;
const unsigned int  msaa            =  4;    // | 0 = no anti-aliasing | 4 = 4xMSAA |
const bool          recording       =  1;
bool                fullscreen      =  0;
bool                pbo             =  1;
bool                paused          =  0;
const int           vsync           = -1;   // | 0 = V-Sync Off | 1 = V-Sync On | -1 = Adaptive V-Sync (V-Sync turns off if FPS<Hz) |
unsigned int        framerate       = 60;
#define             RENDER_3D           1   // make sure to change in the shaders too
#define             RENDER_EBO          0   // only matters when RENDER_3D=1
#define             IMGUI               1

// pbo settings
// ------------
int index = 0;
int nextIndex = 1;
GLuint pboIds[PBO_COUNT];

// fbo settings
// ------------
GLuint fboMsaaId, rboMsaaColorId, rboMsaaDepthId;
GLuint fboId, rboId;
GLuint fboTexture;

// textures
// --------
GLuint textureAtlasID;
GLuint crateTexture, awesomeTexture;
// Initialize atlas dimensions
const unsigned int atlasWidth   = 512;
const unsigned int atlasHeight  = 512;

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
float   lastX       = SCR_WIDTH / 2.0f;
float   lastY       = SCR_HEIGHT / 2.0f;

// timing
// ------
float deltaTime, oldTime = 0.0f;
float timeDiff, prevTime = 0.0f;
float dt, old_t = 0.0f;
float crntTime = 0.0f;
// Keeps track of the amount of frames in timeDiff
unsigned int counter = 0;
// Variables for timing
float lastTime = 0.0f;
int frameCount = 0;
float fps = 0.0f;
float msPerFrame = 0.0f;

// window
// ------
int window_xPos, window_yPos, window_width, window_height;

// viewport
// --------
int lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY;

// IMGUI
// -----
// ImVec2 imgui_pos;

// FFmpeg
// ------
AVFormatContext* formatCtx = nullptr;
AVCodecContext* codecCtx = nullptr;
AVStream* videoStream = nullptr;
SwsContext* swsCtx = nullptr;
AVFrame* frameX = nullptr;
AVPacket pkt = {};  // Zero-initialize the struct
std::ofstream logFile;
std::mutex logMutex;


// light source
// ------------
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);


// Path to ffmpeg binary, if it's not in the system path, provide the full path. MAKE SURE THE FRAME RESOLUTION IS CORRECT!
// split strings over multiple lines in C (MAKE SURE TO ADD A SPACE AT THE END OF EACH LINE!): https://stackoverflow.com/a/797351
// Formatting char array: https://stackoverflow.com/a/10820457
// If video is not outputting correctly, check ffmpeg_log.txt, probably a fault in the FFmpegCommand.
const char* FFmpegCommand = "ffmpeg "
                            "-y "                               // Overwrites the output file if it exists.
                            "-f rawvideo "                      // Tells FFmpeg to expect raw video frames.
                            "-pixel_format rgb24 "              // The format of the raw pixel data (RGB, 8 bits per channel).
                            "-video_size %dx%d "                // The resolution of each frame (WIDTHxHEIGHT).
                            "-framerate %d "                    // The frame rate of the video.
                            "-use_wallclock_as_timestamps 1 "   // Preserves frame timestamp (variable FPS) https://video.stackexchange.com/a/25953
                            "-i - "                             // Tells FFmpeg to read input from stdin ("-").
                            "-c:v libx264 "                     // Uses the H.264 codec to compress the video.
                            "-crf 18 -preset slow "             // Constant rate factor (18=visually lossless, 23=default)
                            // "-tune animation "
                            "-pix_fmt yuv420p "                 // Sets the pixel format to YUV 4:2:0, which is widely supported by media players.
                            "output.mp4 "                       // The output file name.
                            "2> ffmpeg_log.txt";                // Saves FFmpeg’s output and error messages to a file called "ffmpeg_log.txt".

FILE* ffmpeg;

// "C:/WINDOWS/FONTS/ARIAL.TTF"
// "C:/WINDOWS/FONTS/BKANT.TTF"
const std::string fontFilepath = "C:/WINDOWS/FONTS/ARIAL.TTF";
FT_Library ft;
FT_Face face;

std::map<char, Glyph> glyphs;          // Store info about each glyph
GLuint VAO, VBO, EBO;
GLuint textVAO, textVBO;
GLuint quadVAO, quadVBO;

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
    wglSwapIntervalEXT(vsync);
    std::cout << "\nwglSwapIntervalEXT: " << wglGetSwapIntervalEXT() << std::endl;

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
        // positions          // texture coords
        // -Z face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // (-,-,-) 0
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // (+,-,-) 1
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   // (-,+,-) 3
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // (-,-,-) 0

        // +Z face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // (+,-,+) 5
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // (+,+,+) 6
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // (+,+,+) 6
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   // (-,+,+) 7
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4

        // -X face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (-,+,+) 7
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (-,+,-) 3
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (-,-,-) 0
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (-,-,-) 0
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (-,+,+) 7

        // +X face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (+,+,+) 6
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (+,-,-) 1
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (+,-,-) 1
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (+,-,+) 5
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (+,+,+) 6

        // -Y face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (-,-,-) 0
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   // (+,-,-) 1
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // (+,-,+) 5
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // (+,-,+) 5
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (-,-,-) 0

        // +Y face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   // (-,+,-) 3
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (+,+,+) 6
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (+,+,+) 6
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   // (-,+,+) 7
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f    // (-,+,-) 3
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
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
    unsigned char *data = stbi_load("../textures/container.jpg", &width, &height, &nrChannels, 0);
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
    data = stbi_load("../textures/awesomeface.png", &width, &height, &nrChannels, 0);
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

    // bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crateTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, awesomeTexture);
    
    if (recording)
    {
        // PBO off
        if (!pbo)
        {
            // Frame buffer to hold the raw frame data (RGB)
            // std::vector<unsigned char> frame(SCR_WIDTH * SCR_HEIGHT * 3);
            // std::array<unsigned char, SCR_WIDTH * SCR_HEIGHT * 3> frame;
            // Why use unsigned char? https://stackoverflow.com/a/13642984
            frame = new unsigned char[SCR_WIDTH * SCR_HEIGHT * 3];// - 2415];
            // unsigned int bufferSize = 5;
            // unsigned char* frameBuffer = (unsigned char*) malloc(SCR_WIDTH * SCR_HEIGHT * 3 * bufferSize);
            // unsigned int frameCount = 0;
        }
        // PBO on
        else
        {
            // create 2 pixel buffer objects, you need to delete them when program exits.
            // glBufferData() with NULL pointer reserves only memory space.
            glGenBuffers(PBO_COUNT, pboIds);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[0]);
            glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[1]);
            glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);

            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        }

        //Somewhere at initialization
        /*  Framebuffer */
        glGenFramebuffers(1,&fboMsaaId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

        /* 4x MSAA renderbuffer object for colorbuffer */
        glGenRenderbuffers(1,&rboMsaaColorId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboMsaaColorId);
        /* Storage must be one of: */
        /* GL_RGBA4, GL_RGB565, GL_RGB5_A1, GL_DEPTH_COMPONENT16, GL_STENCIL_INDEX8. */
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_RGBA4, SCR_WIDTH, SCR_HEIGHT);
        // attach colorbuffer image to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
                                  GL_COLOR_ATTACHMENT0, // 2. color attachment point
                                  GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
                                  rboMsaaColorId);      // 4. rbo ID
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        
        /* 4x MSAA renderbuffer object for depthbuffer */
        glGenRenderbuffers(1, &rboMsaaDepthId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboMsaaDepthId);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

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


    // load font and create texture atlas
    // ----------------------------------
    std::ifstream file(fontFilepath);
    if (!file.good()) {
        std::cerr << "ERROR: Font file not found at " << fontFilepath << std::endl;
        return 1;
    }
    FT_UInt fontsize = 48;
    loadFont(fontsize);
    createTextureAtlas();

    // build and compile our text shader program
    // -----------------------------------------
    Shader textShader("text_shader.vert", "text_shader.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);

    glBindVertexArray(textVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    
    // Position and texture attribute
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Set up text shader and projection matrix
    // ----------------------------------------
    textShader.use();
    glm::mat4 projection_text = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    textShader.setMat4("projection", projection_text);


    // build and compile our fullscreen atlas shader program
    // ----------------------------------------------------
    Shader atlasShader("atlas.vert", "atlas.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float quad_vertices[] = {
        // Positions   // Texture coordinates
        -1.0f,  1.0f,  0.0f,  1.0f,  // Top-left
        -1.0f, -1.0f,  0.0f,  0.0f,  // Bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  // Bottom-right

        -1.0f,  1.0f,  0.0f,  1.0f,  // Top-left
         1.0f, -1.0f,  1.0f,  0.0f,  // Bottom-right
         1.0f,  1.0f,  1.0f,  1.0f   // Top-right
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    atlasShader.use();
    atlasShader.setMat4("projection", projection_text);


    // build and compile our fullscreen quad shader program
    // ----------------------------------------------------
    // Shader quadShader("quad.vert", "quad.frag");

    // // Position attribute
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // // Texture coordinate attribute
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // light source
    // ------------
    Shader lightShader("light.vert", "light.frag");
    lightShader.use();
    // float light_cube[] = {
    //     // positions          // texture coords
    //     // -Z face
    //      0.5f, -0.5f, -0.5f,   // (+,-,-) 1
    //     -0.5f, -0.5f, -0.5f,   // (-,-,-) 0
    //      0.5f,  0.5f, -0.5f,   // (+,+,-) 2
    //      0.5f,  0.5f, -0.5f,   // (+,+,-) 2
    //     -0.5f,  0.5f, -0.5f,   // (-,+,-) 3
    //     -0.5f, -0.5f, -0.5f,   // (-,-,-) 0

    //     // +Z face
    //     -0.5f, -0.5f,  0.5f,   // (-,-,+) 4
    //      0.5f, -0.5f,  0.5f,   // (+,-,+) 5
    //      0.5f,  0.5f,  0.5f,   // (+,+,+) 6
    //      0.5f,  0.5f,  0.5f,   // (+,+,+) 6
    //     -0.5f,  0.5f,  0.5f,   // (-,+,+) 7
    //     -0.5f, -0.5f,  0.5f,   // (-,-,+) 4

    //     // -X face
    //     -0.5f,  0.5f,  0.5f,   // (-,+,+) 7
    //     -0.5f,  0.5f, -0.5f,   // (-,+,-) 3
    //     -0.5f, -0.5f, -0.5f,   // (-,-,-) 0
    //     -0.5f, -0.5f, -0.5f,   // (-,-,-) 0
    //     -0.5f, -0.5f,  0.5f,   // (-,-,+) 4
    //     -0.5f,  0.5f,  0.5f,   // (-,+,+) 7

    //     // +X face
    //      0.5f,  0.5f,  0.5f,   // (+,+,+) 6
    //      0.5f,  0.5f, -0.5f,   // (+,+,-) 2
    //      0.5f, -0.5f, -0.5f,   // (+,-,-) 1
    //      0.5f, -0.5f, -0.5f,   // (+,-,-) 1
    //      0.5f, -0.5f,  0.5f,   // (+,-,+) 5
    //      0.5f,  0.5f,  0.5f,   // (+,+,+) 6

    //     // -Y face
    //     -0.5f, -0.5f, -0.5f,   // (-,-,-) 0
    //      0.5f, -0.5f, -0.5f,   // (+,-,-) 1
    //      0.5f, -0.5f,  0.5f,   // (+,-,+) 5
    //      0.5f, -0.5f,  0.5f,   // (+,-,+) 5
    //     -0.5f, -0.5f,  0.5f,   // (-,-,+) 4
    //     -0.5f, -0.5f, -0.5f,   // (-,-,-) 0

    //     // +Y face
    //     -0.5f,  0.5f, -0.5f,   // (-,+,-) 3
    //      0.5f,  0.5f, -0.5f,   // (+,+,-) 2
    //      0.5f,  0.5f,  0.5f,   // (+,+,+) 6
    //      0.5f,  0.5f,  0.5f,   // (+,+,+) 6
    //     -0.5f,  0.5f,  0.5f,   // (-,+,+) 7
    //     -0.5f,  0.5f, -0.5f,   // (-,+,-) 3
    // };
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube), light_cube, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightShader.setMat4("model", model);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    // // draw the light cube object
    // glBindVertexArray(lightVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);

    // IMGUI
    // -----
    #if IMGUI==1
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    #endif

    // Start ffmpeg process
    // --------------------
    // startFFmpeg();
    namespace fs = std::filesystem;
    if (!fs::exists("../output/")) {
        fs::create_directory("../output/"); // Create directory if it doesn't exist
    }
    if (!initializeEncoder("../output/output.mp4")) {
        return -1;
    }
    
    glEnable(GL_BLEND); // enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // bool test = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        // framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error after glTexImage2D: " << error << std::endl;
            break;
        }
        // else {
        //     std::cerr << "no error" << std::endl;
        // }

        // render
        // ------
        // Set the clear color to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Clear the current framebuffer
        // At this point the entire framebuffer is black
        glClear(GL_COLOR_BUFFER_BIT);
        // Set the clear color to the background color of choice
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // background color
        // Clear the area within the scissor box, that is, the viewport
        // glScissor(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, SCR_WIDTH, SCR_HEIGHT);
        // glEnable(GL_SCISSOR_TEST);
        // glClear(GL_COLOR_BUFFER_BIT);
        // glDisable(GL_SCISSOR_TEST);
        
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
		if (deltaTime >= 1.0 / 30.0)
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

        // Render FPS text at the top-left corner
        float scale = static_cast<float>(SCR_WIDTH)*0.3f/800.0f;
        // Position on the screen
        float x = lowerLeftCornerOfViewportX;
        float y = lowerLeftCornerOfViewportY + static_cast<float>(SCR_HEIGHT) - 35.0f * scale * fontsize/48.0f; // Invert Y-axis since OpenGL origin is bottom-left
        glm::vec3 color(1.0f, 1.0f, 1.0f); // White text

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

            // glDisable(GL_BLEND);
            
            // Step 2: Resolve MSAA FBO to standard non-MSAA FBO
            // -------------------------------------------------
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
            glBlitFramebuffer(//lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, lowerLeftCornerOfViewportX+SCR_WIDTH, lowerLeftCornerOfViewportY+SCR_HEIGHT,           // src rect
                              0, 0, SCR_WIDTH, SCR_HEIGHT,
                              0, 0, SCR_WIDTH, SCR_HEIGHT,
                              //lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, lowerLeftCornerOfViewportX + static_cast<int>(SCR_WIDTH), lowerLeftCornerOfViewportY + static_cast<int>(SCR_HEIGHT),           // dst rect
                                      GL_COLOR_BUFFER_BIT,           // buffer mask
                                               GL_LINEAR);           // scale filter
            
            RenderText(textShader, fpsText, x, y, scale, color);
            RenderAtlas(atlasShader, textureAtlasID);
            
            // IMGUI (visible in screen recording, messes up when window is resized)
            // ---------------------------------------------------------------------
            #if IMGUI==1
            // Position ImGui window within the viewport
            // float offsetX = 10.0f;
            // float offsetY = 10.0f;
            // ImGui::SetNextWindowPos(ImVec2(lowerLeftCornerOfViewportX*0 + offsetX, lowerLeftCornerOfViewportY*0 + offsetY));
            // here's the ImGui window resizing code
            // int width, height;
            // glfwGetWindowSize(window, &width, &height);
            // ImGui::SetNextWindowSize(ImVec2(width, height)); // ensures ImGui fits the GLFW window
            // ImGui::SetNextWindowPos(ImVec2(lowerLeftCornerOfViewportX, 100+lowerLeftCornerOfViewportY), ImGuiCond_);//, ImGuiCond_FirstUseEver);
            ImGui::Begin("My name is window, ImGUI window");
            ImGui::Text("Hello there adventurer!");
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            #endif

            // Step 3: Render the scene for on-screen rendering using Blitting: https://stackoverflow.com/a/31487085
            // -----------------------------------------------------------------------------------------------------
            // Bind the target FBO to read
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);

            // PBO off
            if (!pbo)
            {
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
                glBlitFramebuffer(//lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, lowerLeftCornerOfViewportX + static_cast<int>(SCR_WIDTH), lowerLeftCornerOfViewportY + static_cast<int>(SCR_HEIGHT),           // src rect
                                  0, 0, SCR_WIDTH, SCR_HEIGHT,
                                  0, 0, SCR_WIDTH, SCR_HEIGHT,
                                  //lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, lowerLeftCornerOfViewportX + static_cast<int>(SCR_WIDTH), lowerLeftCornerOfViewportY + static_cast<int>(SCR_HEIGHT),
                                          GL_COLOR_BUFFER_BIT,           // buffer mask
                                                  GL_NEAREST);           // scale filter
                // RenderFullscreenQuad(quadShader, fboTexture);

                // Step 4: Read pixels from the resolved FBO for off-screen encoding (without PBOs)
                // --------------------------------------------------------------------------------
                
                glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame);

                // Flip the frame vertically
                flipFrameVertically(frame);

                // sendFrameBufferToFFmpeg(frame, 1);
                encodeFrame(frame);
            }
            // PBO on
            else
            {
                // Step 4: Read pixels from the resolved FBO for off-screen encoding (with PBOs)
                // -----------------------------------------------------------------------------
                // "index" is used for GPU to read pixels from framebuffer to a PBO
                // "nextIndex" is used for CPU to update/process pixels in the other PBO
                index = (index + 1) % 2;
                nextIndex = (index + 1) % 2;
                // read pixels from framebuffer to PBO (pack)
                // glReadPixels() should return immediately.
                glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
                glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, 0);  // array.data() is a built-in function in C++ STL which returns a pointer pointing to the first element in the array object

                // map the PBO to process its data by CPU (unpack)
                glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[nextIndex]);
                GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
                if (ptr)
                {
                    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
                    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                                      0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                                            GL_COLOR_BUFFER_BIT,           // buffer mask
                                                    GL_NEAREST);           // scale filter
                    // RenderFullscreenQuad(quadShader, fboTexture);

                    // Flip the frame vertically
                    flipFrameVertically(ptr);
                    
                    // Encode frame using FFmpeg
                    // -------------------------
                    // sendFrameBufferToFFmpeg(ptr, 1);
                    encodeFrame(ptr);
                    
                    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
                }

                // back to conventional pixel operation
                glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            }
        }
        
        // recording=0
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glEnable(GL_DEPTH_TEST); // Needed for 3D rendering
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(xOffset, yOffset, 0.0f));
            // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            ourShader.setMat4("transform", trans);

            glBindVertexArray(VAO);
            // glBindBuffer(GL_ARRAY_BUFFER, VBO);

            // render the crate
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // static crate
            glm::vec3 translatenew = glm::vec3(0.0f, 0.0f, 0.0f);
            RenderCrate(ourShader, translatenew);

            // controllable crate
            glm::vec3 translate = glm::vec3(xOffset, yOffset, zOffset);
            RenderCrate(ourShader, translate);

            // Render text in front: https://stackoverflow.com/a/5527249
            // glClear(GL_DEPTH_BUFFER_BIT);
            RenderText(textShader, fpsText, x, y, scale, color);
            RenderAtlas(atlasShader, textureAtlasID);

            // IMGUI
            // -----
            #if IMGUI==1
            ImGui::Begin("My name is window, ImGUI window");
            ImGui::Text("Hello there adventurer!");
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
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    #if RENDER_EBO==1 || RENDER_3D==0
    glDeleteBuffers(1, &EBO);
    #endif
    // De-allocate frame buffer
    // free(frame);
    // This deletes all items in the array
    delete[] frame;
    // Delete all the shader programs we've created
	ourShader.Delete();
    lightShader.Delete();
    textShader.Delete();
    atlasShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    if (recording)
    {
        // glDeleteBuffers(PBO_COUNT, pboIds);
        glDeleteFramebuffers(1,&fboMsaaId);
        glDeleteFramebuffers(1,&fboId);
        glDeleteRenderbuffers(1,&rboMsaaColorId);
        glDeleteRenderbuffers(1,&rboMsaaDepthId);
        // glDeleteRenderbuffers(1,&rboId);

        // Stop ffmpeg
        // -----------
        // stopFFmpeg();
        finalizeEncoder();

        // duration = glfwGetTime();
    }
    // IMGUI
    // -----
    #if IMGUI==1
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    #endif

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
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
            glfwSetWindowMonitor(window,
                                 NULL,
                                 window_xPos, window_yPos,
                                //  window_width, window_height,
                                 SCR_WIDTH, SCR_HEIGHT,
                                 framerate
            );
            fullscreen = 0;
        }
        // windowed->fullscreen
        else
        {
            glfwGetWindowPos(window, &window_xPos, &window_yPos);
            glfwGetWindowSize(window, &window_width, &window_height);
            // GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            // const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window,
                                 glfwGetPrimaryMonitor(),
                                 0, 0,
                                //  mode->width, mode->height,
                                 SCR_WIDTH, SCR_HEIGHT,
                                 framerate
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
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        if (paused)
        {
            glfwSetCursorPos(window, lastX, lastY);  // set cursor in centre of screen to remove whiplash cursor jump
            // tell GLFW to capture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            paused = 0;
        }
        else
        {
            glfwSetCursorPos(window, lowerLeftCornerOfViewportX + SCR_WIDTH/2.0f, lowerLeftCornerOfViewportY + SCR_HEIGHT/2.0f);  // set cursor in centre of screen to remove whiplash cursor jump
            // tell GLFW to uncapture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            paused = 1;
        }
    }
    // PBO
    // ---
    // if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    // {
    //     if (pbo)
    //         pbo = 0;
    //     else
    //         pbo = 1;
    // }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    
    // keep viewport size fixed (recording doesn't get messed up)
    // ----------------------------------------------------------
    lowerLeftCornerOfViewportX = 0.5*(width - static_cast<int>(SCR_WIDTH));    // need to convert SCR_WIDTH/HEIGHT from unsigned int->int!
    lowerLeftCornerOfViewportY = 0.5*(height - static_cast<int>(SCR_HEIGHT));
    lowerLeftCornerOfViewportX = std::max(lowerLeftCornerOfViewportX, 0);
    lowerLeftCornerOfViewportY = std::max(lowerLeftCornerOfViewportY, 0);

    // MAKE IT SO THE GAME PAUSES WHEN RESIZING WINDOW.

    // for debugging
    std::cout << "viewportX: " << lowerLeftCornerOfViewportX << " "
              << "viewportY: " << lowerLeftCornerOfViewportY << std::endl;

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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    // only update when unpaused
    if (!paused)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Function to start the ffmpeg process
void startFFmpeg() {
    // https://www.codesansar.com/c-programming/binary-file-modes.htm
    char command[300];
    sprintf(command, FFmpegCommand, SCR_WIDTH, SCR_HEIGHT, framerate);
    ffmpeg = _popen(command, "wb");
    // Annoyingly this error check won't catch if the FFmpeg command is written incorrectly. So double check it is!
    if (!ffmpeg) {
        std::cerr << "Error: Unable to open FFmpeg process." << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Fill frame buffer with nth frame
void appendBuffer(unsigned char* frame, unsigned char* frameBuffer, unsigned int frameCount)
{
    for (int i = 0; i < (int)(SCR_WIDTH * SCR_HEIGHT * 3); i++)
    {
        frameBuffer[(SCR_WIDTH * SCR_HEIGHT * 3) * frameCount + i] = frame[i];
    }
}

// Fill block buffer with nth frame
void sendBlockBufferToFFmpeg(unsigned char* frameBuffer, int N)
{
    int byte_sequence_length = SCR_WIDTH * SCR_HEIGHT * 3;
    unsigned char* byte_sequence = frameBuffer;
    int N_min = std::min(N, static_cast<int>(60 / 20));
    for (int i=0; i < N_min; ++i)
        memcpy(buffer + i*byte_sequence_length, byte_sequence, byte_sequence_length); 
    fwrite(buffer, 3, SCR_WIDTH * SCR_HEIGHT * N_min, ffmpeg);  // Each pixel has 3 bytes (RGB)
    std::cout << "old frame added. fps = " << 1 / timeDiff << " N = " << N_min << std::endl;
}

// Function to send a frame to ffmpeg
void sendFrameBufferToFFmpeg(unsigned char* frameBuffer, unsigned int bufferSize) {
    fwrite(frameBuffer, 3, SCR_WIDTH * SCR_HEIGHT * bufferSize, ffmpeg);  // Each pixel has 3 bytes (RGB)
}

// Function to stop the ffmpeg process
void stopFFmpeg() {
    if (ffmpeg) {
        _pclose(ffmpeg);
        ffmpeg = nullptr;
    }
}

// Flip the frame vertically
void flipFrameVertically(unsigned char* frame) {
    for (unsigned int y = 0; y < SCR_HEIGHT / 2; y++) {
        int oppositeY = SCR_HEIGHT - 1 - y;
        for (unsigned int x = 0; x < SCR_WIDTH * 3; x++) {
            std::swap(frame[y * SCR_WIDTH * 3 + x], frame[oppositeY * SCR_WIDTH * 3 + x]);
        }
    }
}

// Load the font face (you should have a valid file path to the font)
bool loadFont(FT_UInt &fontsize) {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not initialize FreeType Library" << std::endl;
        return false;
    }
    if (FT_New_Face(ft, fontFilepath.c_str(), 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return false;
    }
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        std::cerr << "Failed to set Unicode character map." << std::endl;
        return false;
    }
    // Set the pixel size for glyphs
    if (FT_Set_Pixel_Sizes(face, 0, fontsize)) {
        std::cerr << "ERROR::FREETYPE: Failed to set pixel size." << std::endl;
        return false;
    }
    if (!face) {
        std::cerr << "Failed to load the font face. Ensure the file path is correct." << std::endl;
        return false;
    }
    else {
        std::cout << "FreeType successfully loaded font!" << std::endl;
        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        return true;
    }
}

// Create texture atlas with all the glyphs
void createTextureAtlas() {
    // Variables for positioning glyphs in the atlas
    int offsetX         = 0;
    int offsetY         = 0;
    int rowHeight       = 0;
    // Variables for calculating area used/wasted
    int maxWidth        = 0;
    int totalglyphArea  = 0;
    int wastedArea      = 0;
    int minWastedArea   = 0;

    // int maxAscent, maxDescent = 0;
    glyphs.clear();

    // Create the texture atlas
    glGenTextures(1, &textureAtlasID);
    glBindTexture(GL_TEXTURE_2D, textureAtlasID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after glTexImage2D: " << error << std::endl;
        return;
    }
    // Set texture filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Iterate over all printable ASCII characters
    for (unsigned char c = 32; c < 127; ++c) {
        // std::cout << "Processing character: " << c << std::endl;
        unsigned int glyphIndex = FT_Get_Char_Index(face, c);
        if (glyphIndex == 0) {
            std::cerr << "Character not found in font: " << c << " (" << static_cast<int>(c) << ")" << std::endl;
            continue;
        }
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load character: " << c << " (" << static_cast<int>(c) << ")" << std::endl;
            continue;
        }
        FT_GlyphSlot g = face->glyph;
        if (g->bitmap.buffer == nullptr || g->bitmap.width == 0 || g->bitmap.rows == 0) {
            // std::cerr << "Warning: Glyph '" << c << "' has no valid bitmap data!" << std::endl;
            // continue; // Skip this character (comment out to allow for spaces)
        }
        if (glyphs.find(c) != glyphs.end()) {
            std::cerr << "Error: Character " << c << " already exists in glyph map!" << std::endl;
            break;
        }
        // Check if character doesn't fit in the row
        if (offsetX + g->bitmap.width >= atlasWidth) {
            std::cerr << "REACHED ATLAS WIDTH LIMIT. STARTING NEW ROW. " << offsetX << " + " << g->bitmap.rows << " = " << offsetX + static_cast<int>(g->bitmap.width) << " >= " << atlasWidth << std::endl;
            maxWidth = std::max(maxWidth, offsetX);
            offsetX = 0;
            offsetY += rowHeight;
            rowHeight = 0;
        }
        // Check if character doesn't fit in the atlas
        if (offsetY + g->bitmap.rows >= atlasHeight) {
            std::cerr << "Texture atlas too small!" << std::endl;
            break;
        }

        FT_Bitmap &bitmap = face->glyph->bitmap;
        // Flip the bitmap vertically before uploading
        std::vector<unsigned char> flippedBitmap(bitmap.width * bitmap.rows);
        for (int y = 0; y < (int)bitmap.rows; ++y) {
            std::memcpy(
                &flippedBitmap[y * bitmap.width],
                &bitmap.buffer[(bitmap.rows - 1 - y) * bitmap.width],
                bitmap.width);
        }

        // Copy glyph bitmap to the atlas
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        offsetX, offsetY,
                        g->bitmap.width, g->bitmap.rows,
                        GL_RED, GL_UNSIGNED_BYTE, flippedBitmap.data()
        );

        // Store glyph information
        glyphs[c] = Glyph{
            static_cast<float>(offsetX) / (float)atlasWidth,        // TextureX
            static_cast<float>(offsetY) / (float)atlasHeight,       // TextureY
            static_cast<float>(g->bitmap.width),                    // width
            static_cast<float>(g->bitmap.rows),                     // height
            static_cast<float>(g->bitmap_left),                     // OffsetX
            static_cast<float>(g->bitmap_top),                      // OffsetY
            static_cast<unsigned int>(g->advance.x)                 // AdvanceX
        };
        // maxAscent = int(face->ascender * (face->size->metrics.y_scale / 65536.0)) >> 6;
        // maxDescent = int(abs(face->descender * (face->size->metrics.y_scale / 65536.0))) >> 6;
        totalglyphArea += static_cast<int>(g->bitmap.width) * static_cast<int>(g->bitmap.rows);
        offsetX += g->bitmap.width;
        rowHeight = std::max(rowHeight, static_cast<int>(g->bitmap.rows));

        // std::cout << "Loaded character: " << c << " (" << static_cast<int>(c) << ")" << std::endl;
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);

    wastedArea = atlasWidth * atlasHeight - totalglyphArea;
    minWastedArea = wastedArea - (atlasHeight*(atlasWidth-maxWidth)) - (maxWidth*(atlasHeight-offsetY));
    
    std::cout << " | Texture atlas created: " << atlasWidth << "x" << atlasHeight
              << " | Wasted area: " << wastedArea*100/(atlasWidth*atlasHeight) << "%"
              << " | Minimum size: " << maxWidth << "x" << offsetY
              << " | Minimum wasted area: " << minWastedArea*100/(maxWidth*offsetY) << "% |"
              << std::endl;

    // Format output in columns: https://stackoverflow.com/a/49295288
    // for (const auto& [key, glyph] : glyphs) {
    //     std::cout.precision(6);
    //     std::cout << " | " << "Glyph: "                     << static_cast<char>(key)   << " | "
    //                        << "TextureX: "  << std::setw(9) << glyph.textureX           << " | "
    //                        << "TextureY: "  << std::setw(9) << glyph.textureY           << " | "
    //                        << "Width: "     << std::setw(2) << glyph.width              << " | "
    //                        << "Height: "    << std::setw(2) << glyph.height             << " | "
    //                        << "OffsetX: "   << std::setw(2) << glyph.offsetX            << " | "
    //                        << "OffsetY: "   << std::setw(2) << glyph.offsetY            << " | "
    //                        << "AdvanceX: "  << std::setw(5) << glyph.advanceX           << " | "
    //                        << std::endl;
    // }
}

// Render text (this function is in the render loop)
void RenderText(Shader &textShader, const std::string &text, float x, float y, float scale, glm::vec3 color) {
    // Use your text rendering shader
    textShader.use();
    textShader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlasID);  // Bind the texture atlas
    textShader.setInt("textTextureAtlas", 0);

    // Enable 2D rendering
    glEnable(GL_BLEND); // enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up the transformation matrix for the text position
    glm::mat4 projection = glm::ortho(lowerLeftCornerOfViewportX + 0.0f, lowerLeftCornerOfViewportX + static_cast<float>(SCR_WIDTH), lowerLeftCornerOfViewportY + 0.0f, lowerLeftCornerOfViewportY + static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
    // glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
    textShader.setMat4("projection", projection);

    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);

    // Iterate through characters
    for (const char &c : text) {
        // std::cout << "Processing character: " << c << std::endl;
        // Skip characters that do not exist in the glyph map
        if (glyphs.find(c) == glyphs.end()) {
            // std::cerr << "Character " << c << " not found in glyph map!" << std::endl;
            continue;
        }
        // Retrieve glyph
        Glyph &glyph = glyphs[c];

        // Calculate position and size of quad
        float xpos = x + glyph.offsetX * scale;
        float ypos = y + (glyph.offsetY - glyph.height) * scale;
        float w = glyph.width * scale;
        float h = glyph.height * scale;

        // Update VBO
        float tx = glyph.textureX;
        float ty = glyph.textureY; 
        float tw = glyph.width / atlasWidth;
        float th = glyph.height / atlasHeight;

        float vertices[6][4] = {
           // positions          // texture coords
            { xpos,     ypos + h,   tx,      ty + th }, // Top-left
            { xpos,     ypos,       tx,      ty      }, // Bottom-left
            { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right

            { xpos,     ypos + h,   tx,      ty + th }, // Top-left
            { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right
            { xpos + w, ypos + h,   tx + tw, ty + th }  // Top-right
        };
        // float verticesnew[] = {
        //     // positions          // texture coords
        //     xpos,     ypos + h,   tx,      ty + th, // Top-left     0
        //     xpos,     ypos,       tx,      ty     , // Bottom-left  1
        //     xpos + w, ypos,       tx + tw, ty     , // Bottom-right 2
        //     xpos + w, ypos + h,   tx + tw, ty + th  // Top-right    3
        // };
        // float indices[] = {
        //     0, 1, 2,
        //     0, 2, 3
        // };

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);            // ideal for small subset updates
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // better for reallocating and initializing large buffers

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursor
        x += (glyph.advanceX >> 6) * scale; // Advance in pixels (1/64th units)
    }

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Disable blend mode after rendering the text
    // glDisable(GL_BLEND); // disable transparency
}

std::string GetFPSText(float fps, float ms) {
    char buffer[150];
    // "FPS: %.1f | %.1f ms"
    // "AaBbCcDdEeFfGg1!2£4$"
    // "In the dream, they took me to the light. A beautiful lie."
    snprintf(buffer, sizeof(buffer), "FPS: %.3f | %.3f ms | mix=%.1f | FOV=%.1f | pos=%.1f,%.1f,%.1f | cam=%.1f,%.1f,%.1f | YAW= %.1f | PITCH=%.1f | %s", fps, ms, mixValue, camera.Zoom, xOffset, yOffset, zOffset, camera.Position.x, camera.Position.y, camera.Position.z, camera.Yaw, camera.Pitch, paused ? "PAUSED" : "");
    return std::string(buffer);
}

void UpdateFPS(float crntTime) {
    frameCount++;

    // Calculate FPS every second
    if (crntTime - lastTime >= 1.0f / 10.0f) {
        fps = frameCount / (crntTime - lastTime);
        msPerFrame = 1000.0f / fps; // Convert to milliseconds
        frameCount = 0;
        lastTime = crntTime;
    }
}

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

void RenderAtlas(Shader &atlasShader, GLuint &atlasTexture) {
    atlasShader.use();
    // Enable 2D rendering
    glEnable(GL_BLEND); // enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // bind VAO
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    // Bind the texture (the texture atlas in this case)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasTexture); // atlasTextureId is the texture containing the atlas
    atlasShader.setInt("screenTexture", 0);
    glm::vec3 color(1.0f, 1.0f, 1.0f); // White text
    atlasShader.setVec3("textColor", color);

    // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
    // atlasShader.setMat4("projection", projection);

    // Draw the quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderFullscreenQuad(Shader &quadShader, GLuint &quadTexture) {
    quadShader.use();
    // Enable 2D rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // bind VAO
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

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

    // view
    // ----
    // moveable camera
    // glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    // view = glm::translate(view, glm::vec3(camX, camY, camZ));
    
    // turntable camera
    // const float radius = 10.0f;
    // float camX = sin(glfwGetTime()) * radius;
    // float camZ = cos(glfwGetTime()) * radius;
    // glm::mat4 view;
    // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    
    // moveable camera + swivel
    glm::mat4 view = camera.GetViewMatrix();
    
    ourShader.setMat4("view", view);

    // projection
    // ----------
    // glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
    // ourShader.setMat4("projection", projection);
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

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

// Function to initialize FFmpeg encoder
bool initializeEncoder(const char* filename) {
    // Set up ffmpeg's log callback
    // av_log_set_callback(custom_ffmpeg_log_callback);

    // Optional: Set log level (AV_LOG_DEBUG=full logs, AV_LOG_INFO=default, AV_LOG_WARNING=only warnings, AV_LOG_ERROR=only errors)
    // av_log_set_level(AV_LOG_INFO);

    avformat_alloc_output_context2(&formatCtx, nullptr, "mp4", filename);
    if (!formatCtx) {
        printf("Could not create output context\n");
        return false;
    }

    // Find the H.264 encoder
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        printf("H.264 encoder not found\n");
        return false;
    }

    // Create a new stream
    videoStream = avformat_new_stream(formatCtx, codec);
    if (!videoStream) {
        printf("Could not create video stream\n");
        return false;
    }

    // Set up the codec context
    int FPS = static_cast<int>(framerate);
    codecCtx = avcodec_alloc_context3(codec);
    codecCtx->width = SCR_WIDTH;
    codecCtx->height = SCR_HEIGHT;
    codecCtx->time_base = (AVRational){1, FPS*1000};  // Frame rate
    codecCtx->framerate = (AVRational){FPS, 1};
    codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    codecCtx->bit_rate = 60000000;  // 60 Mbps
    codecCtx->gop_size = 10;
    codecCtx->max_b_frames = 1;
    // codecCtx->rc_max_rate = codecCtx->bit_rate;  // Set maximum bitrate limit
    // codecCtx->rc_buffer_size = codecCtx->bit_rate;  // Set buffer size to match bitrate

    // Set H.264 options
    av_opt_set(codecCtx->priv_data, "preset", "slow", 0);
    av_opt_set(codecCtx->priv_data, "crf", "18", 0);
    // av_opt_set(codecCtx->priv_data, "bitrate", "30000k", 0);   // Set bitrate to 5000 kbps
    // av_opt_set(codecCtx->priv_data, "maxrate", "30000k", 0);   // Max bitrate
    // av_opt_set(codecCtx->priv_data, "bufsize", "30000k", 0);   // Buffer size
    // av_opt_set(codecCtx->priv_data, "profile", "high", 0);     // Set profile to "high" for better quality
    av_opt_set(codecCtx->priv_data, "pix_fmt", "yuv420p", 0);  // Try using yuv422p or yuv444p for better quality


    // Open the encoder
    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        printf("Could not open codec\n");
        return false;
    }

    // Assign codec to stream
    videoStream->codecpar->codec_id = AV_CODEC_ID_H264;
    avcodec_parameters_from_context(videoStream->codecpar, codecCtx);

    // Make sure that your AVStream has the same time_base
    videoStream->time_base = codecCtx->time_base;
    // Force the output stream to interpret it as CFR (Constant Frame Rate)
    // videoStream->avg_frame_rate = (AVRational){60, 1};  // Force the output to exactly 60 FPS
    // av_dict_set(&formatCtx->metadata, "video-cfr", "1", 0);

    // Open output file
    if (!(formatCtx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&formatCtx->pb, filename, AVIO_FLAG_WRITE) < 0) {
            printf("Could not open output file\n");
            return false;
        }
    }

    // Write header
    if (avformat_write_header(formatCtx, nullptr) < 0) {
        std::cerr << "Error writing header to output file!" << std::endl;
        return false;  // Handle failure
    }

    // Allocate frame and conversion context
    frameX = av_frame_alloc();
    frameX->format = AV_PIX_FMT_YUV420P;
    frameX->width = SCR_WIDTH;
    frameX->height = SCR_HEIGHT;
    av_frame_get_buffer(frameX, 32);

    swsCtx = sws_getContext(SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_RGB24,
                            SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_YUV420P,
                            SWS_BILINEAR, nullptr, nullptr, nullptr);
    return true;
}

// Encode frame using FFmpeg
bool encodeFrame(const uint8_t* rgbData) {
    // Convert RGB to YUV420P
    uint8_t* inData[1] = {(uint8_t*)rgbData};  // Input RGB data
    int inLinesize[1] = {3 * SCR_WIDTH};          // Input stride
    sws_scale(swsCtx, inData, inLinesize, 0, SCR_HEIGHT, frameX->data, frameX->linesize);

    frameX->pts = static_cast<int64_t>(crntTime * 60 * 1000); //* AV_TIME_BASE);
    // frameX->pts = 1000 * frameIndex; // Simple frame count to force 60 FPS
    // frameIndex++;
    
    // Encode the frame
    if (avcodec_send_frame(codecCtx, frameX) < 0) {
        printf("Error sending frame to encoder\n");
        return false;
    }

    // Retrieve the encoded packet
    pkt.data = nullptr;
    pkt.size = 0;
    // pkt.dts = (frameX->pts < pkt.dts) ? pkt.dts + 1 : frameX->pts;
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
    if (logFile.is_open())
    {
        logFile.close();
    }

    av_write_trailer(formatCtx);
    avcodec_free_context(&codecCtx);
    avformat_free_context(formatCtx);
    sws_freeContext(swsCtx);
    av_frame_free(&frameX);
}
