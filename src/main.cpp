#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>

#include <iostream>

# define M_PI           3.14159265358979323846
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float redValue(float timeValue);
float blueValue(float timeValue);
float greenValue(float timeValue);
float xRotate(float r, float theta, float timeValue);
float yRotate(float r, float theta, float timeValue);
void saveImage(char* filepath, GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 1080;
const unsigned int CHANNEL_COUNT = 3;
const int DATA_SIZE = SCR_WIDTH * SCR_HEIGHT * CHANNEL_COUNT;
const int PBO_COUNT = 2;
const int msaa = 4;
const bool offscreen_render = 0;
int frame = 0;
int index = 0;
int nextIndex = 1;
// int index_stride = 0;
GLuint pboIds[PBO_COUNT];
GLuint fboMsaaId, rboColorId, rboDepthId;
GLuint fboId, rboId;

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
        // positions         // colors
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  (float)sqrt(3), 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };
    float offset = 0.5f;
    float r = 0.7f;
    float ang_vel = 0.1f;
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
        // create 2 pixel buffer objects, you need to delete them when program exits.
        // glBufferData() with NULL pointer reserves only memory space.
        glGenBuffers(PBO_COUNT, pboIds);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[0]);
        glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[1]);
        glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

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
    }


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
            glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,             // src rect
                            0, 0, SCR_WIDTH, SCR_HEIGHT,             // dst rect
                            GL_COLOR_BUFFER_BIT,                     // buffer mask
                            GL_LINEAR);                              // scale filter
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);

            char filepath[256];
            sprintf(filepath, "../images/output/frame%03d.png", frame+1);
            // saveImage(filepath, window);
            frame++;

            GLsizei stride = CHANNEL_COUNT * SCR_WIDTH;
            //stride += (stride % 4) ? (4 - stride % 4) : 0;
            //glPixelStorei(GL_PACK_ALIGNMENT, 4);

            

            // read pixels from framebuffer to PBO
            // glReadPixels() should return immediately.
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
            glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, 0);

            // // map the PBO to process its data by CPU
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[nextIndex]);
            GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
            if(ptr)
            {
                stbi_flip_vertically_on_write(true);
                stbi_write_png(filepath, SCR_WIDTH, SCR_HEIGHT, CHANNEL_COUNT, ptr, stride);
                glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
            }

            // if (index == 4)
            // {
            //     for (int i = 0; i<5; i++)
            //     {
            //         sprintf(filepath, "../images/output/frame%03d.png", i + PBO_COUNT*index_stride);
            //         glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[i]);
            //         GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
            //         stbi_flip_vertically_on_write(true);
            //         stbi_write_png(filepath, SCR_WIDTH, SCR_HEIGHT, CHANNEL_COUNT, ptr, stride);
            //         glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
            //     }

            //     index_stride++;
            // }

            // "index" is used to read pixels from framebuffer to a PBO
            // "nextIndex" is used to update pixels in the other PBO
            index = (index + 1) % PBO_COUNT;
            nextIndex = (nextIndex + 1) % PBO_COUNT;
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
        glDeleteBuffers(PBO_COUNT, pboIds);
        glDeleteFramebuffers(1,&fboMsaaId);
        glDeleteFramebuffers(1,&fboId);
        glDeleteRenderbuffers(1,&rboColorId);
        glDeleteRenderbuffers(1,&rboDepthId);
        glDeleteRenderbuffers(1,&rboId);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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

float redValue(float timeValue)
{
    float redValue = cos(timeValue) / 2.0f + 0.5f;
    return redValue;
}

float greenValue(float timeValue)
{
    float greenValue = sin(timeValue - M_PI/6) / 2.0f + 0.5f;
    return greenValue;
}

float blueValue(float timeValue)
{
    float blueValue = -(cos(timeValue) - M_PI/3) / 2.0f + 0.5f;
    return blueValue;
}

float xRotate(float r, float theta, float timeValue)
{
    float x = r*cos(theta);
    float y = r*sin(theta);
    float xRotate = x*cos(timeValue) - y*sin(timeValue);
    return xRotate;
}

float yRotate(float r, float theta, float timeValue)
{
    float x = r*cos(theta);
    float y = r*sin(theta);
    float yRotate = x*sin(timeValue) + y*cos(timeValue);
    return yRotate;
}

// https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/#:~:text=Quick%20answer,data%20to%20file%20using%20stbi_write_png%20.&text=I%20used%20GLFW%20to%20create,image%20data%20to%20png%20files.
void saveImage(char* filepath, GLFWwindow* window) {
 int width, height;
 glfwGetFramebufferSize(window, &width, &height);
 GLsizei nrChannels = 3;
 GLsizei stride = nrChannels * width;
 stride += (stride % 4) ? (4 - stride % 4) : 0;
 GLsizei bufferSize = stride * height;
 std::vector<char> buffer(bufferSize);
 glPixelStorei(GL_PACK_ALIGNMENT, 4);
 glReadBuffer(GL_FRONT);
 glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
 stbi_flip_vertically_on_write(true);
 stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
}