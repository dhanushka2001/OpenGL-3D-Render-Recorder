#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>
#include <iostream>
#include <cstdio>
# define M_PI           3.14159265358979323846
#include <cmath>
#include <vector>
#include <windows.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float redValue(float T);
float blueValue(float T);
float greenValue(float T);
float xRotate(float r, float theta, float T);
float yRotate(float r, float theta, float T);
void startFFmpeg();
void sendFrameToFFmpeg(unsigned char* frame);
void stopFFmpeg();
void flipFrameVertically(unsigned char* frame);

// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 1080;
const unsigned int CHANNEL_COUNT = 3;
const int DATA_SIZE = SCR_WIDTH * SCR_HEIGHT * CHANNEL_COUNT;
const int msaa = 4; // 0 = no anti-aliasing. 4 = 4xMSAA
const bool offscreen_render = 1;
GLuint fboMsaaId, rboColorId, rboDepthId;
GLuint fboId, rboId;

// Path to ffmpeg binary, if it's not in the system path, provide the full path.
const char* FFmpegCommand = "ffmpeg -y -f rawvideo -pixel_format rgb24 -video_size 1080x1080 -framerate 30 -i - -c:v libx264 -pix_fmt yuv420p output.mp4 2> ffmpeg_log.txt";

FILE* ffmpeg;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // OpenGL 4.3 => major=4,minor=3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("3.3.shader.vert", "3.3.shader.frag"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions                   // colors
         1.0f,           -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -1.0f,           -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  (float)sqrt(3), 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };
    float offset = 0.5f;
    float r = 0.7f;
    float ang_vel = 0.2f;
    float color_vel = 1.0f;

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    if (offscreen_render)
    {
        //Somewhere at initialization
        /*  Framebuffer */
        glGenFramebuffers(1,&fboMsaaId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

        /* 4x MSAA renderbuffer object for colorbuffer */
        glGenRenderbuffers(1,&rboColorId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboColorId);
        /* Storage must be one of: */
        /* GL_RGBA4, GL_RGB565, GL_RGB5_A1, GL_DEPTH_COMPONENT16, GL_STENCIL_INDEX8. */
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_RGBA4, SCR_WIDTH, SCR_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboColorId);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        
        /* 4x MSAA renderbuffer object for depthbuffer */
        glGenRenderbuffers(1, &rboDepthId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepthId);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

        // attach depthbuffer image to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
                                GL_DEPTH_ATTACHMENT,  // 2. depth attachment point
                                GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
                                rboDepthId);          // 4. rbo ID
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // create a normal (no MSAA) FBO to hold a render-to-texture
        glGenFramebuffers(1, &fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        glGenRenderbuffers(1, &rboId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, SCR_WIDTH, SCR_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboId);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        //Before drawing
        glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return -1;
        }

        // Start ffmpeg process
        startFFmpeg();
    }

    // Frame buffer to hold the raw frame data (RGB)
    std::vector<unsigned char> frame(SCR_WIDTH * SCR_HEIGHT * 3);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // activate the shader
        ourShader.use();

        // update the color
        float timeValue = glfwGetTime();
        float newvertices[] = {
            // positions                                                                            // colors
            xRotate(r,     0.0f, ang_vel*timeValue), yRotate(r,     0.0f, ang_vel*timeValue), 0.0f, redValue(color_vel*timeValue),           greenValue(color_vel*timeValue),          blueValue(color_vel*timeValue),           // bottom right
            xRotate(r, 2*M_PI/3, ang_vel*timeValue), yRotate(r, 2*M_PI/3, ang_vel*timeValue), 0.0f, redValue(color_vel*timeValue+2*M_PI/3),  greenValue(color_vel*timeValue+2*M_PI/3), blueValue(color_vel*timeValue+2*M_PI/3),  // bottom left
            xRotate(r, 4*M_PI/3, ang_vel*timeValue), yRotate(r, 4*M_PI/3, ang_vel*timeValue), 0.0f, redValue(color_vel*timeValue+4*M_PI/3),  greenValue(color_vel*timeValue+4*M_PI/3), blueValue(color_vel*timeValue+4*M_PI/3)   // top 
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(newvertices), newvertices, GL_DYNAMIC_DRAW);
        ourShader.setFloat("xOffset", offset);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // set the target framebuffer to read
        /* GL_BACK, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 */
        glReadBuffer(GL_BACK);

        if (offscreen_render)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMsaaId);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
            glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                              0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                                      GL_COLOR_BUFFER_BIT,           // buffer mask
                                               GL_LINEAR);           // scale filter
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
            glReadBuffer(GL_READ_FRAMEBUFFER);

            glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame.data());

            // Flip the frame vertically
            flipFrameVertically(frame.data());

            // Send the frame data to ffmpeg
            sendFrameToFFmpeg(frame.data());

            glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

        }

        // back to conventional pixel operation
        // glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (offscreen_render)
    {
        // glDeleteBuffers(PBO_COUNT, pboIds);
        glDeleteFramebuffers(1,&fboMsaaId);
        glDeleteFramebuffers(1,&fboId);
        glDeleteRenderbuffers(1,&rboColorId);
        glDeleteRenderbuffers(1,&rboDepthId);
        glDeleteRenderbuffers(1,&rboId);

        // Stop ffmpeg
        stopFFmpeg();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

float redValue(float T)
{
    float redValue = cos(T) / 2.0f + 0.5f;
    return redValue;
}

float greenValue(float T)
{
    float greenValue = sin(T - M_PI/6) / 2.0f + 0.5f;
    return greenValue;
}

float blueValue(float T)
{
    float blueValue = -(cos(T) - M_PI/3) / 2.0f + 0.5f;
    return blueValue;
}

float xRotate(float r, float theta, float T)
{
    float x = cos(theta);
    float y = sin(theta);
    float xRotate = x*cos(T) - y*sin(T);
    return r*xRotate;
}

float yRotate(float r, float theta, float T)
{
    float x = cos(theta);
    float y = sin(theta);
    float yRotate = x*sin(T) + y*cos(T);
    return r*yRotate;
}

// Function to start the ffmpeg process
void startFFmpeg() {
    ffmpeg = _popen(FFmpegCommand, "wb");
    if (!ffmpeg) {
        std::cerr << "Error: Unable to open FFmpeg process." << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Function to send a frame to ffmpeg
void sendFrameToFFmpeg(unsigned char* frame) {
    if (ffmpeg) {
        fwrite(frame, 3, SCR_WIDTH * SCR_HEIGHT, ffmpeg);  // Each pixel has 3 bytes (RGB)
    }
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
    for (unsigned int y = 0; y < SCR_HEIGHT / 2; ++y) {
        int oppositeY = SCR_HEIGHT - 1 - y;
        for (unsigned int x = 0; x < SCR_WIDTH * 3; ++x) {
            std::swap(frame[y * SCR_WIDTH * 3 + x], frame[oppositeY * SCR_WIDTH * 3 + x]);
        }
    }
}