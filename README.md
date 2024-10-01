*Coding Project*
# LearnOpenGL
Learning to use OpenGL (and OpenCL) so I can do high-performance scientific computing and modelling on the GPU.
Using www.learnopengl.com as my main resource

## Progress update 1 - Hello Triangle - 30/06/24
* I understand how VBOs (vertex buffer objects), VAOs (vertex array objects), and EBOs (element buffer objects) work and why we need them.
* The VAO essentially stores the pointers (memory addresses) for all the vertex attributes (position, colour, etc.) in a specific VBO, which stores the actual values of those vertex attributes.
* An EBO fixes the issue of duplicate vertices when they overlap by storing only the unique vertices and an index to render them in the correct order.
* Was able to render a static 3D RGB cube using 3 fragment shaders and 3 VBOs, 3 VAOs, and 3 EBOs.
* I feel like 3 fragment shaders, 3 VBOs, 3 VAOs, and 3 EBOs are overkill just to render 3 different colours for 3 sets of 2 triangles. There's probably a way to have all 6 triangles in one VBO with their colour attributes also included.
  This will probably be explained in the next chapters.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/OpenGL_EBO.png" width=100%>

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/learnopengl.png" width=45%>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/learnopengl2.png" width=45%>

## Progress update 2 - Shaders - 20/07/24
* I understand the render pipeline for OpenGL and what all the shaders in the pipeline do.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/OpenGL_graphics-shader-pipeline.png" width=50%>
  
* Gave each vertex a colour attribute as well as a position attribute, so instead of having 3 VBOs, 3 VAOs, and 3 EBOs, we just have 1 of each and 2 vertex attributes. One for each object makes sense (I assume when we get to rendering 1000s of identical objects we will encounter "instancing").
* Removed the vertex shader and fragment shader GLSL code from the main.cpp file and put them into separate "shader.vert" and "shader.frag" text files (I recommend installing a GLSL syntax highlighter extension).
* Made a shader class and header file that can handle retrieving source code; compiling, linking, deleting, and activating shaders; defining utility functions; and handling errors.
* I understand what uniform variables are (basically global variables that all shaders can access), you need to define them in the main.cpp file and give them to the shader (using utility uniform functions if you have a separate shader header file, or just by giving the uniform location with "glUniform[](...)").
* Switched from OpenGL 3.3 to 4.3 so that I can specify the "layout (location=...)" for uniforms, for some reason in OpenGL 3.3 that didn't work. This is a really nice video that helped me understand the "layout (location=...)" specifier. At [21:11](https://youtu.be/yrFo1_Izlk0?si=YLplgGlBy1hqR0u2&t=1271) it talks about the specifier and how you can omit the specifier but if you don't then you can change the variable name in the next shader, and vertex attributes and uniforms are stored in different arrays so you can have seemingly two different variables stored in "location=0" but they are actually in different arrays.
  [![Watch the video](https://img.youtube.com/vi/yrFo1_Izlk0/maxresdefault.jpg)](https://www.youtube.com/watch?v=yrFo1_Izlk0)

* This animation was done by storing a green colour uniform variable in the render loop that changes over time, this uniform variable can be accessed by the fragment shader to render the triangle with changing colour.

  https://github.com/user-attachments/assets/e2bdee00-6a7d-4f6b-a29b-513f5611c3d3

* This rainbow triangle was done by storing a red, green, and blue colour value as a second vertex attribute for each of the 3 vertices. "Fragment interpolation" occurs, where a linear combination of the colours is used for all the fragments (can think of them as pixels) between the 3 vertices.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/rainbow-shader.png" width=80%>

* This flipped offset gradient colour triangle was done by setting an offset float variable in the main.cpp file, then setting it as a uniform in the render loop using the utility uniform functions created in the shader header file. The weird colours were done by defining the FragColor to be the positions of the 3 vertices rather than the colour (Exercise 3), and fragment interpolation causes the gradient effect. (One thing to note is that if you input a negative float into one of the RGB channels it will be clamped to 0.0f (black)).

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/rainbow-shader2.png" width=80%>

## Progress update 3 - Textures - 22/08/24
* I now understand how textures work, and I was able to render this rectangle with a crate PNG texture with an RGB overlay and the "Awesome face" PNG. I still need to do the exercises.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/textures.png" width=50%>

* Alongside learning how to use textures, I wanted to do a mini-project rendering a rotating animated RGB triangle and learning how to render off-screen frames as images to memory rather than to a window on-screen.
* The texture section introduced me to the [stb](https://github.com/nothings/stb) repo by Sean Barrett, in particular the stb_image.h header, a single header image loading library used to load an image into a texture; and the stb_image_write.h header, used for image writing from OpenGL to disk (PNG).
* In order to make the triangle spin I used the standard 2D [rotation matrix](https://en.wikipedia.org/wiki/Rotation_matrix).

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl-rotation-matrix.png" width=45%>&nbsp;&nbsp;<img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl2.png" width=53%>

  ```cpp
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
  ```

* In order to make the colours inside the triangle spin I used the [Sinebow](https://basecase.org/env/on-rainbows) over the HSV function as it has no branches making it faster for GPGPUs[<sup>[3]</sup>](https://basecase.org/env/on-rainbows).

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/HSV-vs-Sinebow.png" width=40%>

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl3.1.png" width=48%><img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl4.1.png" width=45%>

  ```cpp
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
  ```
  ```cpp
  // update the color
  float timeValue = glfwGetTime();
  float newvertices[] = {
                               // bottom right
           xRotate(r, 0.0f, ang_vel*timeValue),  // x
           yRotate(r, 0.0f, ang_vel*timeValue),  // y
                                          0.0f,  // z
                 redValue(color_vel*timeValue),  // R
               greenValue(color_vel*timeValue),  // G
                blueValue(color_vel*timeValue),  // B
                                // bottom left           
       xRotate(r, 2*M_PI/3, ang_vel*timeValue),  // x
       yRotate(r, 2*M_PI/3, ang_vel*timeValue),  // y
                                          0.0f,  // z
        redValue(color_vel*timeValue+2*M_PI/3),  // R
      greenValue(color_vel*timeValue+2*M_PI/3),  // G
       blueValue(color_vel*timeValue+2*M_PI/3),  // B
                                        // top 
       xRotate(r, 4*M_PI/3, ang_vel*timeValue),  // x
       yRotate(r, 4*M_PI/3, ang_vel*timeValue),  // y
                                          0.0f,  // z
        redValue(color_vel*timeValue+4*M_PI/3),  // R
      greenValue(color_vel*timeValue+4*M_PI/3),  // G
       blueValue(color_vel*timeValue+4*M_PI/3)   // B
  };
  ```
  
* In order to implement off-screen rendering I initially found [this blog post](https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/) which worked fine but I felt like the rendering could be done faster which led me to a [few posts](https://stackoverflow.com/a/25127895) from Stack Overflow that said to use [PBOs (pixel buffer objects)](https://www.khronos.org/opengl/wiki/Pixel_Buffer_Object), which allow for [asynchronous readback](https://www.songho.ca/opengl/gl_pbo.html#pack), which means rendering to system memory later rather than as soon as possible in the hopes of it being faster. Unfortunately, I found PBOs to not make any difference in performance, which led me to discover FBOs (framebuffer objects) which are essentially a non-default [framebuffer](https://www.khronos.org/opengl/wiki/Framebuffer) (unlike the FRONT and BACK buffers which are) that allows you to do proper off-screen rendering to a memory buffer instead of the default screen buffers[<sup>[1]</sup>](https://stackoverflow.com/a/12159293) (OpenGL forces a window to be loaded on-screen but with FBOs it will be black and you can just hide the screen (but don't minimize!), Vulkan is designed to support off-screen rendering better than OpenGL[<sup>[2]</sup>](https://stackoverflow.com/a/14324292) but is more verbose and thus harder to learn (maybe in the future...)). FBOs are optimized for data to be read back to the CPU, while the default buffers are made to stay on the GPU and display pixels on-screen[<sup>[2]</sup>](https://stackoverflow.com/a/14324292).

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl5.3.png" width=80%>
  
* Learning about FBOs naturally led me to learn about [RBOs (renderbuffer objects)](https://www.khronos.org/opengl/wiki/Renderbuffer_Object) which are specifically used by FBOs as a render target. Textures can alternatively be used if you want to re-use the pixels on-screen (e.g. a naive "security camera" in a game)[<sup>[1]</sup>](https://stackoverflow.com/a/12159293) or [sample the pixels for post-processing](https://www.songho.ca/opengl/gl_pbo.html#pack), however since we just want to read-back the pixels and render off-screen, RBOs are the logical choice.
* I have decided to keep the code with the PBO as even though it doesn't make any performative difference and just adds more lines of code, I may need them in the future. I ended up using 2 PBOs, while one has pixel data from the FBO written into it, the other is being mapped for reading. This has no performative benefit to just reading and writing to system memory from the FBO directly on every cycle since on every cycle I am still invoking glReadPixels which stalls the pipeline "because it has to safely map the data from the read buffer to the PBO and to a block of memory in main RAM", and I am mapping the other PBO which also stalls the pipeline until the pixel data has been converted to a png and saved to system memory[<sup>[1]</sup>](https://stackoverflow.com/a/12159293).
* I have tried to implement multiple PBOs to delay the writing to system memory step till every n<sup>th</sup> cycle, however, it doesn't seem to work properly. For some reason after every every n<sup>th</sup> cycle the rendered frames jump ahead, seemingly skipping multiple frames, is mapping the data not stalling the pipeline?
* Implemented [off-screen MSAA](https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing) (multisample anti-aliasing) which is an advanced topic but I skipped ahead. Needed to use 2 FBOs now since MSAA requires one to be multisample storage and the other to be a normal FBO to downsample the result to a single-sample image using glBlitFramebuffer(), as we cannot directly use the result from MSAA FBO (see: https://www.songho.ca/opengl/gl_fbo.html#msaa). Also required creating a depthbuffer alongside the colorbuffer for the MSAA FBO.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/gl_fbo04.png" width=50%>

* The video below was rendered using the ffmpeg command below after running off-screen rendering for around 1 minute, converting 300 frames to a 10s video at 30fps. In the future, I'd like to make it so that the program feeds the frames into ffmpeg and continually builds the video at runtime rather than generating losslessly compressed frames which is costly for memory, especially for longer videos. For reference, the 300 frames total 35.3MB while the video rendered using those frames is just 856KB, ~40x less memory used.                                               

  ```cmd
  ffmpeg -framerate 30 -start_number 2 -i "frame%03d.png" -c:v libx264 -pix_fmt yuv420p out.mp4
  ```
  
  https://github.com/user-attachments/assets/66e9a3a9-d633-4e1d-adf6-1e6356896643

* The triangle isn't cycling through the Sinebow properly so it isn't true RGB, I'm not sure why yet, however, I prefer the colour scheme it's cycling through so I just kept it.

  <!-- ADD BIBLIOGRAPHY -->
  <!-- ADD CODE SHOWING FBO, RBO, PBO, etc. -->
  <!-- FINALLY SHOW RESULTS WITH TEXTURES -->

1. [How to render offscreen on OpenGL?](https://stackoverflow.com/a/12159293)
2. [How to use GLUT/OpenGL to render to a file?](https://stackoverflow.com/a/14324292)
3. [On rainbows by Charlie Loyd](https://basecase.org/env/on-rainbows)
4. [Save the OpenGL rendering to an image file - Lencerfs Walk](https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/)



## License
GNU General Public License v3.0
