#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <learnopengl/shader_s.h>
#include <iostream>
# define M_PI           3.14159265358979323846
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define GL_GLEXT_PROTOTYPES 1
#include <array>
#include <vector>
#include <map>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#include <ft2build.h>
#include FT_FREETYPE_H


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void startFFmpeg();
void sendFrameToFFmpeg(unsigned char* frame);
void stopFFmpeg();
void flipFrameVertically(unsigned char* frame);
// void setupQuad();
// void renderQuadWithTexture(GLuint texture, Shader& shader);

struct Glyph {
    float textureX, textureY;  // Texture coordinates in the atlas
    float width, height;       // Glyph's width and height
    float offsetX, offsetY;    // Offsets (for positioning)
    unsigned int advanceX;     // Horizontal advance (for spacing)
};

bool loadFont();
void createTextureAtlas();
void RenderText(Shader &textShader, const std::string &text, float x, float y, float scale, glm::vec3 color);
std::string GetFPSText(float fps, float ms);
void UpdateFPS();
// void RenderFullscreenQuad();

// settings
const unsigned int  SCR_WIDTH       = 800;
const unsigned int  SCR_HEIGHT      = 600;
const unsigned int  CHANNEL_COUNT   = 3;
const int           DATA_SIZE       = SCR_WIDTH * SCR_HEIGHT * CHANNEL_COUNT;
// const int           PBO_COUNT       = 2;
const int           msaa            = 4; // 0 = no anti-aliasing. 4 = 4xMSAA
const bool          offscreen       = 1;
bool                fullscreen      = 0;


// int frame = 0;
// int index = 0;
// int nextIndex = 1;
// GLuint pboIds[PBO_COUNT];
GLuint fboMsaaId, rboMsaaColorId, rboMsaaDepthId;
GLuint fboId, rboId;
GLuint fboTexture;
GLuint textureAtlasID;
// Initialize atlas dimensions
const unsigned int atlasWidth   = 512;
const unsigned int atlasHeight  = 512;

// uniform variables
float   mixValue    = 0.2f;
float   xOffset     = 0.0f;
float   yOffset     = 0.0f;

// Variables to create periodic event for FPS displaying (on title)
float prevTime, oldTime = 0.0f;
float crntTime, newTime = 0.0f;
float timeDiff, deltaTime = 0.0f;
// Keeps track of the amount of frames in timeDiff
unsigned int counter = 0;

// Path to ffmpeg binary, if it's not in the system path, provide the full path. MAKE SURE THE FRAME RESOLUTION IS CORRECT!
// split strings over multiple lines in C (MAKE SURE TO ADD A SPACE AT THE END OF EACH LINE!): https://stackoverflow.com/a/797351
const char* FFmpegCommand = "ffmpeg "
                            "-y "                   // Overwrites the output file if it exists.
                            "-f rawvideo "          // Tells FFmpeg to expect raw video frames.
                            "-pixel_format rgb24 "   // The format of the raw pixel data (RGB, 8 bits per channel).
                            "-video_size 800x600 "  // The resolution of each frame.
                            "-framerate 60 "        // The frame rate of the video.
                            "-i - "                 // Tells FFmpeg to read input from stdin ("-").
                            "-c:v libx264 "         // Uses the H.264 codec to compress the video.
                            "-pix_fmt yuv420p "     // Sets the pixel format to YUV 4:2:0, which is widely supported by media players.
                            "output.mp4 "           // The output file name.
                            "2> ffmpeg_log.txt";    // Saves FFmpeg’s output and error messages to a file called "ffmpeg_log.txt".

FILE* ffmpeg;

// "C:/WINDOWS/FONTS/ARIAL.TTF"
// "C:/WINDOWS/FONTS/BKANT.TTF"
const std::string fontFilepath = "C:/WINDOWS/FONTS/ARIAL.TTF";
FT_Library ft;
FT_Face face;

std::map<char, Glyph> glyphs;          // Store info about each glyph
// Glyph glyph;
GLuint textVAO, textVBO;

// Variables for timing
float lastTime = 0.0f;
int frameCount = 0;
float fps = 0.0f;
float msPerFrame = 0.0f;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); // glfwGetPrimaryMonitor(), NULL);
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

    // The last element buffer object (EBO) that gets bound while a VAO is bound is stored as that VAO's EBO.
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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

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
    ourShader.Activate(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the shader class
    ourShader.setInt("texture2", 1);

    // bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    if (offscreen)
    {
        // create 2 pixel buffer objects, you need to delete them when program exits.
        // glBufferData() with NULL pointer reserves only memory space.
        // glGenBuffers(PBO_COUNT, pboIds);
        // glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[0]);
        // glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);
        // glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[1]);
        // glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);

        // glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

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

        // glGenRenderbuffers(1, &rboId);
        // glBindRenderbuffer(GL_RENDERBUFFER, rboId);
        // glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, SCR_WIDTH, SCR_HEIGHT);
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboId);
        // glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // // attach the renderbuffer to depth attachment point
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
        //                           GL_DEPTH_ATTACHMENT, // 2. attachment point
        //                           GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
        //                           rboId);              // 4. rbo ID 

        // Check if the FBO is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer fboId is not complete!\n" << std::endl;
        }

        // // Start ffmpeg process
        // startFFmpeg();

        // glEnable(GL_DEPTH_TEST);
    }

    // Frame buffer to hold the raw frame data (RGB)
    std::vector<unsigned char> frame(SCR_WIDTH * SCR_HEIGHT * 3);
    // std::array<unsigned char, SCR_WIDTH * SCR_HEIGHT * 3> frame;

	// Use this to disable VSync (not advised)
	// glfwSwapInterval(0);

    std::ifstream file(fontFilepath);
    if (!file.good()) {
        std::cerr << "ERROR: Font file not found at " << fontFilepath << std::endl;
        return 1;
    }
    loadFont();
    createTextureAtlas();
    // build and compile our text shader program
    Shader textShader("text_shader.vert", "text_shader.frag");
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    // Set up text shader and projection matrix
    textShader.Activate();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    textShader.setMat4("projection", projection);

    // Start ffmpeg process
    startFFmpeg();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        // framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // background color
        glClear(GL_COLOR_BUFFER_BIT);
        
        // FPS Counter: https://www.youtube.com/watch?v=BA6aR_5C_BM
        // Updates counter and times
		crntTime = glfwGetTime();
        newTime = crntTime;
		timeDiff = crntTime - prevTime;
        deltaTime = newTime - oldTime;
        prevTime = crntTime;
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
			oldTime = newTime;
            counter = 0;

			// Use this if you have disabled VSync
			//camera.Inputs(window);
		}

        // Update timing info
        UpdateFPS();
        std::string fpsText = GetFPSText(fps, msPerFrame);

        // Render FPS text at the top-left corner
        float x = 10.0f; // Position on the screen
        float y = SCR_HEIGHT - 35.0f; // Invert Y-axis since OpenGL origin is bottom-left
        float scale = 1.0f;
        glm::vec3 color(1.0f, 1.0f, 1.0f); // White text

        /***************** Render scene *****************/
        float newvertices[] = {
            // positions                          // colors           // texture coords
            0.5f+xOffset,  0.5f+yOffset, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f+xOffset, -0.5f+yOffset, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
           -0.5f+xOffset, -0.5f+yOffset, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
           -0.5f+xOffset,  0.5f+yOffset, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
        };
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(newvertices), newvertices, GL_DYNAMIC_DRAW);

        if (offscreen)
        {
            // Step 1: Render the scene to the MSAA FBO
            glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
            glEnable(GL_DEPTH_TEST); // Needed for 3D rendering
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // set the texture mix value in the shader (this needs to be in the render loop)
            ourShader.Activate();
            ourShader.setFloat("mixValue", mixValue);

            // ourShader.setFloat("xOffset", xOffset);
            // ourShader.setFloat("yOffset", yOffset);

            // bind Texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            ourShader.setInt("texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            ourShader.setInt("texture2", 1);

            // render the scene
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);    // crate
            // RenderText(textShader, fpsText, x, y, scale, color);    // text
            /****************************************************/
            
            // Step 2: Resolve MSAA FBO to standard FBO
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMsaaId);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
            glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                              0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                                      GL_COLOR_BUFFER_BIT,           // buffer mask
                                               GL_LINEAR);           // scale filter
            RenderText(textShader, fpsText, x, y, scale, color);
            // Step 3: Render the scene for on-screen rendering using Blitting
            // https://stackoverflow.com/a/31487085
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                              0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                                      GL_COLOR_BUFFER_BIT,           // buffer mask
                                              GL_NEAREST);           // scale filter

            // Step 4: Read pixels from the resolved FBO for off-screen encoding
            glBindFramebuffer(GL_FRAMEBUFFER, fboId);
            glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame.data());  // array.data() is a built-in function in C++ STL which returns a pointer pointing to the first element in the array object

            // Flip the frame vertically
            flipFrameVertically(frame.data());

            // Send the frame data to ffmpeg
            sendFrameToFFmpeg(frame.data());
        }
        // offscreen=0
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glEnable(GL_DEPTH_TEST); // Needed for 3D rendering
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // set the texture mix value in the shader (this needs to be in the render loop)
            ourShader.Activate();
            ourShader.setFloat("mixValue", mixValue);

            ourShader.setFloat("xOffset", xOffset);
            ourShader.setFloat("yOffset", yOffset);

            // bind Texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            ourShader.setInt("texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            ourShader.setInt("texture2", 1);

            // render the crate
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            RenderText(textShader, fpsText, x, y, scale, color);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);    // swap the BACK buffer with the FRONT buffer
        glfwPollEvents();           // take care of all GLFW events
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (offscreen)
    {
        // glDeleteBuffers(PBO_COUNT, pboIds);
        glDeleteFramebuffers(1,&fboMsaaId);
        glDeleteFramebuffers(1,&fboId);
        glDeleteRenderbuffers(1,&rboMsaaColorId);
        glDeleteRenderbuffers(1,&rboMsaaDepthId);
        // glDeleteRenderbuffers(1,&rboId);

        // Stop ffmpeg
        stopFFmpeg();
    }

    // Delete all the objects we've created
	ourShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // crntTime = glfwGetTime();
    // timeDiff = crntTime - prevTime;
    // prevTime = crntTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        yOffset += 0.1f * timeDiff * 10;
        if(yOffset >= 0.5f)
            yOffset = 0.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        yOffset -= 0.1f * timeDiff * 10;
        if (yOffset <= -0.5f)
            yOffset = -0.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        xOffset -= 0.1f * timeDiff * 10;
        if(xOffset <= -0.5f)
            xOffset = -0.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        xOffset += 0.1f * timeDiff * 10;
        if(xOffset >= 0.5f)
            xOffset = 0.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        mixValue -= 0.1f * timeDiff * 10;
        if(mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        mixValue += 0.1f * timeDiff * 10;
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {
        if (fullscreen)
        {
            glfwSetWindowMonitor(window,
                                nullptr,
                                0, 0,
                                SCR_WIDTH, SCR_HEIGHT,
                                0
            );
            fullscreen = 0;
        }
        else
        {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window,
                                 glfwGetPrimaryMonitor(),
                                 0, 0,
                                 mode->width, mode->height,
                                 0
            );
            fullscreen = 1;
        }
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
    // Annoyingly this error check won't catch if the FFmpeg command is written incorrectly. So double check it is!
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

// Load the font face (you should have a valid file path to the font)
bool loadFont() {
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
    if (FT_Set_Pixel_Sizes(face, 0, 48)) {
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

void createTextureAtlas() {
    // Variables for positioning glyphs in the atlas
    int offsetX     = 0;
    int offsetY     = 0;
    int rowHeight   = 0;
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
    for (unsigned char c = 32; c < 128; ++c) {
        std::cout << "Processing character: " << c << std::endl;
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
            std::cerr << "Warning: Glyph '" << c << "' has no valid bitmap data!" << std::endl;
            // continue; // Skip this character (comment out to allow for spaces)
        }
        if (glyphs.find(c) != glyphs.end()) {
            std::cerr << "Error: Character " << c << " already exists in glyph map!" << std::endl;
            break;
        }
        // Check if character doesn't fit in the row
        if (offsetX + g->bitmap.width >= atlasWidth) {
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
            static_cast<float>(offsetX) / (float)atlasWidth,
            static_cast<float>(offsetY) / (float)atlasHeight,
            static_cast<float>(g->bitmap.width),
            static_cast<float>(g->bitmap.rows),
            static_cast<float>(g->bitmap_left),
            static_cast<float>(g->bitmap_top),
            static_cast<unsigned int>(g->advance.x)
        };

        offsetX += g->bitmap.width;
        rowHeight = std::max(rowHeight, static_cast<int>(g->bitmap.rows));

        std::cout << "Loaded character: " << c << " (" << static_cast<int>(c) << ")" << std::endl;
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "Texture atlas created: " << atlasWidth << "x" << atlasHeight << std::endl;

    for (const auto& [key, glyph] : glyphs) {
        std::cout << "Glyph: " << static_cast<char>(key) 
                << " TextureX: " << glyph.textureX 
                << " TextureY: " << glyph.textureY 
                << " Width: " << glyph.width 
                << " Height: " << glyph.height 
                << std::endl;
    }
}

void RenderText(Shader &textShader, const std::string &text, float x, float y, float scale, glm::vec3 color) {
    // Use your text rendering shader
    textShader.Activate();
    textShader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlasID);  // Bind the texture atlas
    textShader.setInt("textTextureAtlas", 0);

    // Enable 2D rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up the transformation matrix for the text position
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
    textShader.setMat4("projection", projection);

    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

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
            { xpos,     ypos + h,   tx,      ty + th },
            { xpos,     ypos,       tx,      ty      },
            { xpos + w, ypos,       tx + tw, ty      },

            { xpos,     ypos + h,   tx,      ty + th },
            { xpos + w, ypos,       tx + tw, ty      },
            { xpos + w, ypos + h,   tx + tw, ty + th }
        };

        // glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    glDisable(GL_BLEND);
}

std::string GetFPSText(float fps, float ms) {
    char buffer[50];
    // "FPS: %.1f | %.1f ms"
    // "AaBbCcDdEeFfGg1!2£4$"
    // "In the dream, they took me to the light. A beatiful lie."
    snprintf(buffer, sizeof(buffer), "FPS: %.3f | %.3f ms ESC, F11", fps, ms);
    return std::string(buffer);
}

void UpdateFPS() {
    float currentTime = glfwGetTime();
    frameCount++;

    // Calculate FPS every second
    if (currentTime - lastTime >= 1.0f / 60.0f) {
        fps = frameCount / (currentTime - lastTime);
        msPerFrame = 1000.0f / fps; // Convert to milliseconds
        frameCount = 0;
        lastTime = currentTime;
    }
}

// void RenderFullscreenQuad() {
//     // Define vertices for a fullscreen quad (-1, 1 for both axes)
//     float vertices[] = {
//         // Positions        // Texture coordinates
//         -1.0f,  1.0f,  0.0f,  1.0f,  // Top-left
//         -1.0f, -1.0f,  0.0f,  0.0f,  // Bottom-left
//          1.0f, -1.0f,  1.0f,  0.0f,  // Bottom-right

//         -1.0f,  1.0f,  0.0f,  1.0f,  // Top-left
//          1.0f, -1.0f,  1.0f,  0.0f,  // Bottom-right
//          1.0f,  1.0f,  1.0f,  1.0f   // Top-right
//     };

//     // Generate and bind VAO
//     GLuint quadVAO = 0, quadVBO = 0;
//     glGenVertexArrays(1, &quadVAO);
//     glGenBuffers(1, &quadVBO);
//     glBindVertexArray(quadVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     // Position attribute
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);

//     // Texture coordinate attribute
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);

//     // Bind the texture (the texture atlas in this case)
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, textureAtlasID); // atlasTextureId is the texture containing the atlas

//     // Draw the quad
//     glDrawArrays(GL_TRIANGLES, 0, 6);

//     // Cleanup
//     glBindVertexArray(0);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
// }

