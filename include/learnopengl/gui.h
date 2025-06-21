#ifndef GUI_H    // #include guard
#define GUI_H

#include <learnopengl/encoder.h>

namespace GUI {
    void Init(GLFWwindow *window);
    void NewFrame();
    void Render(Encoder *encoder);
    void Exit();
}

#endif /* GUI_H */
