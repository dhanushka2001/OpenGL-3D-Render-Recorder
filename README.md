*Coding Project*
# LearnOpenGL
* Learning to use OpenGL (GLFW) so I can do high-performance scientific computing and modelling on the GPU.
* Read [this forum thread](https://news.ycombinator.com/item?id=19674197) and watch [this video](https://www.youtube.com/watch?v=zQUYsMYElC0) for an explanation of what GLFW is and a comparison between other popular OpenGL API libraries.
* ["GLFW is the library to create the window, OpenGL context, and handle all the input-related stuff. Its version is not related to the OpenGL version you use in any way." "The version of glad is the version of OpenGL you can use. The version of GLFW is irrelevant."](https://www.reddit.com/r/opengl/comments/1dwz5i2/confused_as_to_which_version_of_glad_to_use/)
* I am using MinGW-w64 (GCC) to compile the C++ code. I recommend installing it using [MSYS2](https://www.msys2.org/), [here](https://www.youtube.com/watch?v=C3IxeHthNnM) is a good tutorial. Make sure to install in the C drive (``C:\msys64``), and to run this command in MSYS2: ``pacman -S --needed base-devel mingw-w64-x86_64-toolchain`` (it's important to run this command and not one with "``ucrt64``" in it as the compiler won't be saved in the correct place and the code won't run). Once it has finished downloading you need to add the filepath to Windows PATH by going to Environment Variables and adding ``C:\msys64\mingw64\bin`` to PATH (I recommend putting it at the top of the list, if you have a long list of filepaths and you put the directory where the compiler is stored at the bottom, it may not work, I learned this the hard way).
* Using www.learnopengl.com as my main resource.

## Progress update 1 - Hello Triangle - 30/06/24
* [This blog](https://medium.com/@vivekjha92/setup-opengl-with-vs-code-82852c653c43#:~:text=The%20Setup,clone%20it%20and%20get%20started.) helped a lot with setting up OpenGL with VSCode.
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

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/OpenGL_graphics-shader-pipeline.png" width=70%>
  
* Gave each vertex a colour attribute as well as a position attribute, so instead of having 3 VBOs, 3 VAOs, and 3 EBOs, we just have 1 of each and 2 vertex attributes. One for each object makes sense (I assume when we get to rendering 1000s of identical objects we will encounter "instancing").
* Removed the vertex shader and fragment shader GLSL code from the main.cpp file and put them into separate ``shader.vert`` and ``shader.frag`` text files (I recommend installing a GLSL syntax highlighter extension).
* Made a shader class and header file that can handle retrieving source code; compiling, linking, deleting, and activating shaders; defining utility functions; and handling errors.
* I understand what uniform variables are (basically global variables that all shaders can access), you need to define them in the main.cpp file and give them to the shader (using utility uniform functions if you have a separate shader header file, or just by giving the uniform location with ``glUniform[](...)``).
* Switched from OpenGL 3.3 to 4.3 so that I can specify the ``layout (location=...)`` for uniforms, for some reason in OpenGL 3.3 that didn't work. This is a really nice video that helped me understand the ``layout (location=...)`` specifier. At [21:11](https://youtu.be/yrFo1_Izlk0?si=YLplgGlBy1hqR0u2&t=1271) it talks about the specifier and how you can omit the specifier but if you don't then you can change the variable name in the next shader, and vertex attributes and uniforms are stored in different arrays so you can have seemingly two different variables stored in ``location=0`` but they are actually in different arrays.
  [![Watch the video](https://img.youtube.com/vi/yrFo1_Izlk0/maxresdefault.jpg)](https://www.youtube.com/watch?v=yrFo1_Izlk0)

* This animation was done by storing a green colour uniform variable in the render loop that changes over time, this uniform variable can be accessed by the fragment shader to render the triangle with changing colour.

  https://github.com/user-attachments/assets/e2bdee00-6a7d-4f6b-a29b-513f5611c3d3

* This rainbow triangle was done by storing a red, green, and blue colour value as a second vertex attribute for each of the 3 vertices. "Fragment interpolation" occurs, where a linear combination of the colours is used for all the fragments (can think of them as pixels) between the 3 vertices.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/rainbow-shader.png" width=80%>

* This flipped offset gradient colour triangle was done by setting an offset float variable in the ``main.cpp`` file, then setting it as a uniform in the render loop using the utility uniform functions created in the shader header file. The weird colours were done by defining the ``FragColor`` to be the positions of the 3 vertices rather than the colour (Exercise 3), and fragment interpolation causes the gradient effect. (One thing to note is that if you input a negative float into one of the RGB channels it will be clamped to 0.0f (black)).

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/rainbow-shader2.png" width=80%>

## Progress update 3 - Textures - 22/08/24
* I now understand how textures work, and I was able to render this rectangle with a crate PNG texture with an RGB overlay and the "Awesome face" PNG. I still need to do the exercises.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/textures.png" width=50%>

* Alongside learning how to use textures, I wanted to do a mini-project rendering a rotating animated RGB triangle and learning how to render off-screen frames as images to memory rather than to a window on-screen.
* The texture section introduced me to the [stb](https://github.com/nothings/stb) repo by Sean Barrett, in particular the ``stb_image.h`` header, a single header image loading library used to load an image into a texture; and the ``stb_image_write.h`` header, used for image writing from OpenGL to disk (PNG).
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
  
* In order to implement off-screen rendering I initially found [this blog post](https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/) which worked fine but I felt like the rendering could be done faster which led me to a [few posts](https://stackoverflow.com/a/25127895) from Stack Overflow that said to use [PBOs (pixel buffer objects)](https://www.khronos.org/opengl/wiki/Pixel_Buffer_Object), which allow for [asynchronous readback](https://www.songho.ca/opengl/gl_pbo.html#pack), which means rendering to system memory later rather than as soon as possible in the hopes of it being faster. Unfortunately, I found PBOs to not make any difference in performance, which led me to discover FBOs (framebuffer objects) which are essentially a non-default [framebuffer](https://www.khronos.org/opengl/wiki/Framebuffer) (unlike the FRONT and BACK buffers which are) that allows you to do proper off-screen rendering to a memory buffer instead of the default screen buffers[<sup>[1]</sup>](https://stackoverflow.com/a/12159293) (OpenGL forces a window to be loaded on-screen, but with FBOs it will be black and you can just hide the screen (but don't minimize!), Vulkan is designed to support off-screen rendering better than OpenGL[<sup>[2]</sup>](https://stackoverflow.com/a/14324292) but is more verbose and thus harder to learn (maybe in the future...)). FBOs are optimized for data to be read back to the CPU, while the default buffers are made to stay on the GPU and display pixels on-screen[<sup>[2]</sup>](https://stackoverflow.com/a/14324292).

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl5.3.png" width=80%>
  
* Learning about FBOs naturally led me to learn about [RBOs (renderbuffer objects)](https://www.khronos.org/opengl/wiki/Renderbuffer_Object) which are specifically used by FBOs as a render target. Textures can alternatively be used if you want to re-use the pixels on-screen (e.g. a naive "security camera" in a game)[<sup>[1]</sup>](https://stackoverflow.com/a/12159293) or [sample the pixels for post-processing](https://www.songho.ca/opengl/gl_pbo.html#pack), however since we just want to read-back the pixels and render off-screen, RBOs are the logical choice.
* I have decided to keep the code with the PBO as even though it doesn't make any performative difference and just adds more lines of code, I may need them in the future. I ended up using 2 PBOs, while one has pixel data from the FBO written into it, the other is being mapped for reading. This has no performative benefit to just reading and writing to system memory from the FBO directly on every cycle since on every cycle I am still invoking ``glReadPixels`` which stalls the pipeline "because it has to safely map the data from the read buffer to the PBO and to a block of memory in main RAM", and I am mapping the other PBO which also stalls the pipeline until the pixel data has been converted to a png and saved to system memory[<sup>[1]</sup>](https://stackoverflow.com/a/12159293).
* I have tried to implement multiple PBOs to delay the writing to system memory step till every n<sup>th</sup> cycle, however, it doesn't seem to work properly. For some reason after every every n<sup>th</sup> cycle the rendered frames jump ahead, seemingly skipping multiple frames, is mapping the data not stalling the pipeline?
* Implemented [off-screen MSAA](https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing) (multisample anti-aliasing) which is an advanced topic but I skipped ahead. Needed to use 2 FBOs now since MSAA requires one to be multisample storage and the other to be a normal FBO to downsample the result to a single-sample image using glBlitFramebuffer(), as we cannot directly use the result from MSAA FBO (see: https://www.songho.ca/opengl/gl_fbo.html#msaa). Also required creating a depthbuffer alongside the colorbuffer for the MSAA FBO.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/gl_fbo04.png" width=50%>

* The video below was rendered using the FFmpeg command below after running off-screen rendering for around 1 minute, converting 300 frames to a 10s video at 30fps. In the future, I'd like to make it so that the program feeds the frames into ffmpeg and continually builds the video at runtime rather than generating losslessly compressed frames which is costly for memory, especially for longer videos. For reference, the 300 frames total 35.3MB while the video rendered using those frames is just 856KB, ~40x less memory used.                                               

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

## Progress update 4 - Off-screen recording - 18/10/24

* I have finally figured out how to render an mp4 at runtime off-screen, as mentioned earlier this is much better than rendering an image sequence which is an I/O bottleneck. Previously it would render the PNGs at ~4 fps (which I would have to convert to an mp4 after runtime) but now the program encodes the video from the raw binary data using FFmpeg at runtime and outputs the mp4 after terminating instantly, so seemingly at 60+ fps, I know this since the rendered video is pretty much identical to the on-screen animation, compared to the image sequence→mp4 which skips frames. A 1-minute mp4 is just 3MB, assuming the same memory ratio as earlier, the image sequence equivalent would be 120MB, not to mention it would not look nearly as smooth.

* Trying to decipher the code from [this Stack Overflow answer](https://stackoverflow.com/a/36488003) was too difficult, so I ended up resorting to using ChatGPT, which, after a few tweaks, gave me the code to encode videos in real-time. The code is very concise and does the job. The only requirement is that FFmpeg is installed and in your system's ``PATH``.

  ```cpp
  // Path to ffmpeg binary, if it's not in the system path, provide the full path.
  const char* FFmpegCommand = "ffmpeg -y -f rawvideo -pixel_format rgb24 -video_size 1080x1080 -framerate 30 -i - -c:v libx264 -pix_fmt yuv420p output.mp4 2> ffmpeg_log.txt";
  ```
  * ``-y``: Overwrites the output file if it exists.
  * ``-f rawvideo``: Tells FFmpeg to expect raw video frames.
  * ``-pixel_format rgb24``: The format of the raw pixel data (RGB, 8 bits per channel).
  * ``-video_size 1000x1000``: The resolution of each frame.
  * ``-framerate 30``: The frame rate of the video.
  * ``-i -``: Tells FFmpeg to read input from stdin (``-``).
  * ``-c:v libx264``: Uses the H.264 codec to compress the video.
  * ``-pix_fmt yuv420p``: Sets the pixel format to YUV 4:2:0, which is widely supported by media players.
  * ``output.mp4``: The output file name.
  * ``2> ffmpeg_log.txt``: Saves FFmpeg’s output and error messages to a file called ``ffmpeg_log.txt``.
  &nbsp;
* Use the ``_popen()`` function to start an FFmpeg process. FFmpeg will read the raw RGB frames from stdin, encode them, and write the output to an MP4 file.
* On Windows, using GCC (MinGW), the functions ``popen()`` and ``pclose()`` are not directly available because these functions are POSIX-specific. To solve this issue, you can use ``_popen()`` and ``_pclose()``, which are Windows-specific equivalents of ``popen()`` and ``pclose()``.
* On Windows, it's important to use ``"wb"`` (write binary) mode when opening a pipe for writing raw data.
  ```cpp
  FILE* ffmpeg;
  // Function to start the ffmpeg process
  void startFFmpeg() {
      ffmpeg = _popen(FFmpegCommand, "wb");
      if (!ffmpeg) {
          std::cerr << "Error: Unable to open FFmpeg process." << std::endl;
          exit(EXIT_FAILURE);
      }
  }
  ```
  
  ```cpp
  // Frame buffer to hold the raw frame data (RGB)
  std::vector<unsigned char> frame(SCR_WIDTH * SCR_HEIGHT * 3);
  ```
  
  ```cpp
  // Flip the frame vertically
  void flipFrameVertically(unsigned char* frame) {
      for (unsigned int y = 0; y < SCR_HEIGHT / 2; ++y) {
          int oppositeY = SCR_HEIGHT - 1 - y;
          for (unsigned int x = 0; x < SCR_WIDTH * 3; ++x) {
              std::swap(frame[y * SCR_WIDTH * 3 + x], frame[oppositeY * SCR_WIDTH * 3 + x]);
          }
      }
  }
  ```

  ```cpp
  // Function to send a frame to ffmpeg
  void sendFrameToFFmpeg(unsigned char* frame) {
      if (ffmpeg) {
          fwrite(frame, 3, SCR_WIDTH * SCR_HEIGHT, ffmpeg);  // Each pixel has 3 bytes (RGB)
      }
  }
  ```

  ```cpp
  // Function to stop the ffmpeg process
  void stopFFmpeg() {
      if (ffmpeg) {
          _pclose(ffmpeg);
          ffmpeg = nullptr;
      }
  }
  ```
* Writing frames to FFmpeg via ``popen()`` can be I/O bound, so performance depends on your disk and CPU speed. However, this is a major improvement over trying to render PNGs every frame at runtime, which is immensely I/O bound. [This Stack Overflow answer](https://stackoverflow.com/a/19071087) outlined steps to doing real-time video encoding, however they recommended using ``libavcodec`` and ``libavformat`` as "these are the libraries upon which ``ffmpeg`` is built, and will allow you to encode video and store it in a standard stream/interchange format (e.g. RIFF/AVI) without using a separate program". The general steps are as follows:
1. Pick a container format and CODEC
2. Start filling a buffer with your still frames
3. Periodically encode your buffer of still frames and write to your output (packet writing in MPEG terms)
     * You will do this either when the buffer becomes full, or every n-many ms; you might prefer one over the other depending on whether you want to stream your video live.
4. When your program terminates flush the buffer and close your stream

    My program currently encodes frame by frame which works fine right now, however, I may need to use a buffer for longer renders or when the renders are more intensive.
  
* OpenGL reads pixels starting from the bottom-left of the window, but many image or video formats expect pixels to start from the top-left (i.e., row 0 is at the top). This was done for the PNG sequence using the ``stbi_flip_vertically_on_write(true);`` function call in the render loop, likewise with the function call ``flipFrameVertically(frame.data());`` for each frame of the real-time encoded video.

* I have finished all the exercises for the **Textures** chapter, and I did my own small project to allow the user to move around the object and control the opacity. Now the real-time encoded video starts to show signs of deviating from the on-screen render, the object in the encoded video moves slower than in the on-screen render, which I assume is the result of the video frames not being encoded at ~60fps, due to the I/O bottleneck. This shows how fast FBOs render off-screen compared with the BACK buffer (which wasn't designed for that task[<sup>[2]</sup>](https://stackoverflow.com/a/14324292)), as the spinning RGB triangle rendered perfectly even though I assume it is more intensive than the crate/awesome-face texture. The solution would have to involve implementing the packet method mentioned above, it would be ideal to use FBOs to render off-screen but the user would also need to see the screen to interact with it in real-time, I'm not sure if there is a way to use FBOs and the BACK buffer at the same time though. The solution might involve PBOs, one rendering the current frame on-screen while the other stores the previous frame in an FBO to be sent to a buffer which will be periodically encoded using FFmpeg to build the video asynchronously.

  https://github.com/user-attachments/assets/d86f9581-7abe-4446-9abc-8528bee0bff9

## Progress update 5 - Simultaneous on-screen and off-screen rendering - 16/11/24

* I am now finally able to render on-screen and off-screen simultaneously. I was able to do this naively by just rendering to the BACK buffer and using glReadPixels to render off-screen, however, this isn't ideal as I have already explained, the BACK buffer isn't designed for data to be read back to the CPU. A better method would be to render to a FBO, read the pixel data and feed it to FFmpeg to encode a video off-screen, and somehow also render the FBO pixel data on-screen. An FBO stores pixel data not using the default window framebuffer so it won't be visible, it is designed to be read back to the CPU which is partly what we want. My first approach to get the pixel data to be visible on-screen was to use a fullscreen quad and the texture of the FBO, which others had suggested[<sup>[5]</sup>](https://stackoverflow.com/a/10400048). I spent a lot of time down this path, which involved using another shader program to render the quad, but after spending many, many days on this it just didn't work, I got off-screen rendering to work but not on-screen rendering. At least I learned about using multiple shader programs. The next approach which did work was to blit (copy) the FBO pixel data to the default framebuffer, which I probably should have just done from the start, this solution also just seems to be better performance-wise than the fullscreen quad approach as it means less API calls, not needing to bind another shader program, and many GPUs have dedicated units for blitting data[<sup>[6]</sup>](https://stackoverflow.com/a/31487085). While this does work, I would eventually like to go back to FBO textures and a fullscreen quad as this seems like you can do more stuff like rendering the scene onto objects (textures are easier to manipulate/attach to objects), as well as post-processing using PBOs to modify the texture efficiently[<sup>[7]</sup>](http://www.songho.ca/opengl/gl_fbo.html#example).

* I adjusted the FFmpeg command to encode the video at 60fps rather than 30fps, I should have done that from the beginning, as it matches the 60fps on-screen window. At 30fps, the encoded video seems sluggish. Unfortunately, you will have to manually adjust the frame resolution if needed, I wanted to make it automated but formatting strings in C++ is headache-inducing (side note: it seems like they made it easier to format strings in C++20 using ``std::format``, the person responsible for standardizing it and getting it through the C++20 committee wrote a blog post essentially explaining "what took so long?": https://vitaut.net/posts/2019/std-format-cpp20/, but as I am using C++17 I won't bother unless I choose to update my C++ version).
  ```cpp
  // Path to ffmpeg binary, if it's not in the system path, provide the full path. MAKE SURE THE FRAME RESOLUTION IS CORRECT!
  const char* FFmpegCommand = "ffmpeg -y -f rawvideo -pixel_format rgb24 -video_size 800x600 -framerate 60 -i - -c:v libx264 -pix_fmt yuv420p output.mp4 2> ffmpeg_log.txt";
  ```

  https://github.com/user-attachments/assets/ec9950bc-0568-404e-a57b-5b08b84452d0

  I am still blown away by how quickly the encoded video is rendered, basically instantly after the program terminates, the max runtime I've tried is 1 minute, maybe I should try rendering for longer, but I don't think it would be any less slow since FFmpeg is encoding the video frame by frame every cycle in the render loop. This could also just be due to the scene not being very intensive. The on-screen and off-screen renders match basically exactly, now I can do whatever I want, my first goal is to do some fractal rendering, continue the LearnOpenGL sections, and once I've learnt about instancing, try to implement the Saturn's rings code in OpenGL.

* I also changed from using a C++ vector to using an array to store the frame (pixel) data, as this seems better suited to the job as the frame data is of a fixed size, and an array uses the heap rather than the stack making it faster.
  ```cpp
  // Frame buffer to hold the raw frame data (RGB)
  // std::vector<unsigned char> frame(SCR_WIDTH * SCR_HEIGHT * 3);
  std::array<unsigned char, SCR_WIDTH * SCR_HEIGHT * 3> frame;
  ```


5. [OpenGL rendering from FBO to screen?](https://stackoverflow.com/a/10400048)
6. [OpenGL - Is there an easier way to fill window with a texture, instead using VBO,etc?](https://stackoverflow.com/a/31487085) 
7. [OpenGL Frame Buffer Object (FBO) - Example: Render To Texture - Songho](http://www.songho.ca/opengl/gl_fbo.html#example)

<!-- ADD BIBLIOGRAPHY -->
<!-- ADD CODE SHOWING FBO, RBO, PBO, etc. -->
<!-- FINALLY SHOW RESULTS WITH TEXTURES -->
## License
GNU General Public License v3.0
