#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>

#include <iostream>

# define M_PI           3.14159265358979323846
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float redValue(float timeValue);
float blueValue(float timeValue);
float greenValue(float timeValue);
float xRotate(float r, float theta, float timeValue);
float yRotate(float r, float theta, float timeValue);

// settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1440;

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
        float timeValue = 1*glfwGetTime();
        float r = 0.7f;
        float ang_vel = 1.0f;
        float newvertices[] = {
        // positions         // colors
        xRotate(r,            0.0f, ang_vel*timeValue), yRotate(r,            0.0f, ang_vel*timeValue), 0.0f, redValue(timeValue),           greenValue(timeValue),          blueValue(timeValue),           // bottom right
        xRotate(r, (float)2*M_PI/3, ang_vel*timeValue), yRotate(r, (float)2*M_PI/3, ang_vel*timeValue), 0.0f, redValue(timeValue+2*M_PI/3),  greenValue(timeValue+2*M_PI/3), blueValue(timeValue+2*M_PI/3),  // bottom left
        xRotate(r, (float)4*M_PI/3, ang_vel*timeValue), yRotate(r, (float)4*M_PI/3, ang_vel*timeValue), 0.0f, redValue(timeValue+4*M_PI/3),  greenValue(timeValue+4*M_PI/3), blueValue(timeValue+4*M_PI/3)   // top 
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(newvertices), newvertices, GL_DYNAMIC_DRAW);
        ourShader.setFloat("xOffset", offset);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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