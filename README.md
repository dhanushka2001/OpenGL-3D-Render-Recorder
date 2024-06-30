*Coding Project*
# LearnOpenGL
Learning to use OpenGL (and OpenCL) so I can do high performance scientific computing and modelling on the GPU.
Using www.learnopengl.com as my main resource

## Progress update 1 - 30/06/24
* I understand how VBOs (vertex buffer objects), VAOs (vertex array objects), and EBOs (element buffer objects) work and why we need them.
* The VAO essentially stores the pointers (memory addresses) for all the vertex attributes (position, color, etc.) in a specific VBO, which stores the actual values of those vertex attributes.
* An EBO fixes the issue of duplicate vertices when they overlap by storing only the unique vertices and an index to render them in the correct order.
* Was able to render a static 3D RGB cube using 3 fragment shaders and 3 VBOs, 3VAOs, and 3 EBOs.
* I feel like 3 fragment shaders, 3 VBOs, 3VAOs, and 3EBOs is overkill just to render 3 different colors for 3 sets of 2 triangles. There's probably a way to have all 6 triangles in one VBO with their color attributes also included.
  This will probably be explained in the next chapters.

![OpenGL EBO explained](images/OpenGL_EBO.png)
![LearnOpenGL 3D RGB cube](images/learnopengl.png)
![LearnOpenGL 3D RGB cube](images/learnopengl2.png)

## License
GNU General Public License v3.0
