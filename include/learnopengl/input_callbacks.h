#ifndef INPUT_CALLBACKS_H
#define INPUT_CALLBACKS_H

#include <glm/glm.hpp>
#include <memory>

// Forward declarations
// --------------------
struct GLFWwindow;
class Encoder;

// Function declarations
// ---------------------
namespace Input {
    void processInput(GLFWwindow *window, float timeDiff, float crntTime, std::unique_ptr<Encoder> &encoder);
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void cursor_pos_callback(GLFWwindow* window, double xposIn, double yposIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void char_callback(GLFWwindow* window, unsigned int codepoint);
}


#endif // INPUT_CALLBACKS_H
