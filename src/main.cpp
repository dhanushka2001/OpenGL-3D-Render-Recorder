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
void startFFmpeg();
void sendFrameToFFmpeg(unsigned char* frame);
void stopFFmpeg();
void flipFrameVertically(unsigned char* frame);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int CHANNEL_COUNT = 3;
const int DATA_SIZE = SCR_WIDTH * SCR_HEIGHT * CHANNEL_COUNT;
// const int PBO_COUNT = 2;
// GLuint pboIds[PBO_COUNT];
const int msaa = 4; // 0 = no anti-aliasing. 4 = 4xMSAA
const bool onscreen_render = 1;

// int frame = 0;
// int index = 0;
// int nextIndex = 1;
GLuint fboMsaaId, rboColorId, rboDepthId;
GLuint fboId, rboId;

// uniform variables
float mixValue = 0.2f;
float xOffset = 0.0f;
float yOffset = 0.0f;

// Path to ffmpeg binary, if it's not in the system path, provide the full path. Make sure the frame resolution is correct!
const char* FFmpegCommand = "ffmpeg -y -f rawvideo -pixel_format rgb24 -video_size 800x600 -framerate 30 -i - -c:v libx264 -pix_fmt yuv420p output.mp4 2> ffmpeg_log.txt";

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
    Shader ourShader("4.1.shader.vert", "4.1.shader.frag"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
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

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // once a VAO is bound, any subsequent vertex attribute calls will be stored inside that VAO. (only have to make the calls once)
    glBindVertexArray(VAO);

    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // The last element buffer object (EBO) that gets bound while a VAO is bound, is stored as that VAO's EBO.
    // Binding to a VAO then also automatically binds that EBO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 
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
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
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
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
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
    // or set it via the shader class
    ourShader.setInt("texture2", 1);

    // bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    ourShader.use();
    glBindVertexArray(VAO);
    
    if (onscreen_render)
    {
        // create 2 pixel buffer objects, you need to delete them when program exits.
        // glBufferData() with NULL pointer reserves only memory space.
        // glGenBuffers(PBO_COUNT, pboIds);
        // glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[0]);
        // glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);
        // glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[1]);
        // glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);

        // glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        // //Somewhere at initialization
        // /*  Framebuffer */
        // glGenFramebuffers(1,&fboMsaaId);
        // glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

        // /* 4x MSAA renderbuffer object for colorbuffer */
        // glGenRenderbuffers(1,&rboColorId);
        // glBindRenderbuffer(GL_RENDERBUFFER, rboColorId);
        // /* Storage must be one of: */
        // /* GL_RGBA4, GL_RGB565, GL_RGB5_A1, GL_DEPTH_COMPONENT16, GL_STENCIL_INDEX8. */
        // glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_RGBA4, SCR_WIDTH, SCR_HEIGHT);
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboColorId);
        // glBindRenderbuffer(GL_RENDERBUFFER, 0);
        
        // /* 4x MSAA renderbuffer object for depthbuffer */
        // glGenRenderbuffers(1, &rboDepthId);
        // glBindRenderbuffer(GL_RENDERBUFFER, rboDepthId);
        // glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

        // // attach depthbuffer image to FBO
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
        //                         GL_DEPTH_ATTACHMENT,  // 2. depth attachment point
        //                         GL_RENDERBUFFER,      // 3. rbo target: GL_RENDERBUFFER
        //                         rboDepthId);          // 4. rbo ID
        // glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // // create a normal (no MSAA) FBO to hold a render-to-texture
        // glGenFramebuffers(1, &fboId);
        // glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        // glGenRenderbuffers(1, &rboId);
        // glBindRenderbuffer(GL_RENDERBUFFER, rboId);
        // glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, SCR_WIDTH, SCR_HEIGHT);
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboId);
        // glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // //Before drawing
        // glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

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

        float newvertices[] = {
        // positions                          // colors           // texture coords
         0.5f+xOffset,  0.5f+yOffset, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f+xOffset, -0.5f+yOffset, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f+xOffset, -0.5f+yOffset, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f+xOffset,  0.5f+yOffset, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(newvertices), newvertices, GL_DYNAMIC_DRAW);

        // set the texture mix value in the shader (this needs to be in the render loop)
        ourShader.setFloat("mixValue", mixValue);

        // ourShader.setFloat("xOffset", xOffset);
        // ourShader.setFloat("yOffset", yOffset);

        // render the crate
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // set the target framebuffer to read
        /* GL_BACK, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 */
        glReadBuffer(GL_BACK);

        if (onscreen_render)
        {
            // glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMsaaId);
            // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
            // glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
            //                   0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
            //                           GL_COLOR_BUFFER_BIT,           // buffer mask
            //                                    GL_LINEAR);           // scale filter
            // glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
            // glReadBuffer(GL_READ_FRAMEBUFFER);

            // char filepath[256];
            // sprintf(filepath, "../images/output/frame%03d.png", frame+1);
            // // saveImage(filepath, window);
            // frame++;

            // GLsizei stride = CHANNEL_COUNT * SCR_WIDTH;
            //stride += (stride % 4) ? (4 - stride % 4) : 0;
            //glPixelStorei(GL_PACK_ALIGNMENT, 4);

            // read pixels from framebuffer to PBO
            // glReadPixels() should return immediately.
            // glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
            // glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, 0);

            // map the PBO to process its data by CPU
            // glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[nextIndex]);
            // GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
            // if(ptr)
            // {
            //     stbi_flip_vertically_on_write(true);
            //     stbi_write_png(filepath, SCR_WIDTH, SCR_HEIGHT, CHANNEL_COUNT, ptr, stride);
            //     glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
            // }

            // "index" is used to read pixels from framebuffer to a PBO
            // "nextIndex" is used to update pixels in the other PBO
            // index = (index + 1) % PBO_COUNT;
            // nextIndex = (index + 1) % PBO_COUNT;

            glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame.data());
            
            // Flip the frame vertically
            flipFrameVertically(frame.data());

            // Send the frame data to ffmpeg
            sendFrameToFFmpeg(frame.data());

            // glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (onscreen_render)
    {
        // glDeleteBuffers(PBO_COUNT, pboIds);
        // glDeleteFramebuffers(1,&fboMsaaId);
        // glDeleteFramebuffers(1,&fboId);
        // glDeleteRenderbuffers(1,&rboColorId);
        // glDeleteRenderbuffers(1,&rboDepthId);
        // glDeleteRenderbuffers(1,&rboId);

        // Stop ffmpeg
        stopFFmpeg();
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

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        yOffset += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if(yOffset >= 0.5f)
            yOffset = 0.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        yOffset -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (yOffset <= -0.5f)
            yOffset = -0.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        xOffset -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if(xOffset <= -0.5f)
            xOffset = -0.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        xOffset += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if(xOffset >= 0.5f)
            xOffset = 0.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        mixValue -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if(mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        mixValue += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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