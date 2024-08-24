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
void saveImage(char* filepath, GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int CHANNEL_COUNT = 3;

const int DATA_SIZE = SCR_WIDTH * SCR_HEIGHT * CHANNEL_COUNT;
const int PBO_COUNT = 2;
GLuint pboIds[PBO_COUNT];

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

    int frame = 0;

    int index = 0;
    int nextIndex = 0;

    // create 2 pixel buffer objects, you need to delete them when program exits.
    // glBufferData() with NULL pointer reserves only memory space.
    glGenBuffers(PBO_COUNT, pboIds);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[1]);
    glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);

    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    // bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    ourShader.use();
    glBindVertexArray(VAO);

    //Somewhere at initialization
    /*  Framebuffer */
    GLuint fbo, render_buf;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    /* Color renderbuffer. */
    glGenRenderbuffers(1,&render_buf);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buf);
    /* Storage must be one of: */
    /* GL_RGBA4, GL_RGB565, GL_RGB5_A1, GL_DEPTH_COMPONENT16, GL_STENCIL_INDEX8. */
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, render_buf);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        //Before drawing
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // // bind Texture
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        // render container
        // ourShader.use();
        // glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

        char filepath[256];
        sprintf(filepath, "../images/output/frame%03d.png", frame+1);
        //saveImage(filepath, window);
        frame++;

        // "index" is used to read pixels from framebuffer to a PBO
        // "nextIndex" is used to update pixels in the other PBO
        index = (index + 1) % PBO_COUNT;
        nextIndex = (index + 1) % PBO_COUNT;
        GLsizei stride = CHANNEL_COUNT * SCR_WIDTH;
        //stride += (stride % 4) ? (4 - stride % 4) : 0;
        //glPixelStorei(GL_PACK_ALIGNMENT, 4);

        // set the target framebuffer to read
        /* GL_BACK, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 */
        glReadBuffer(GL_READ_FRAMEBUFFER);

        // read pixels from framebuffer to PBO
        // glReadPixels() should return immediately.
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
        glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, 0);

        // map the PBO to process its data by CPU
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[nextIndex]);
        GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        if(ptr)
        {
            stbi_flip_vertically_on_write(true);
            stbi_write_png(filepath, SCR_WIDTH, SCR_HEIGHT, CHANNEL_COUNT, ptr, stride);
            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        }

        // back to conventional pixel operation
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

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
    glDeleteBuffers(PBO_COUNT, pboIds);
    //At deinit:
    glDeleteFramebuffers(1,&fbo);
    glDeleteRenderbuffers(1,&render_buf);

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