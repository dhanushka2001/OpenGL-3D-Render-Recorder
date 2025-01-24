*Coding Project*
# LearnOpenGL
* Learning to use OpenGL (GLFW) so I can do high-performance scientific computing and modelling on the GPU.
* You may come across things like GLFW, GLUT, GLAD, GLEW, ..., a lot of 4-letter names beginning with "GL", or heard about other OpenGL APIs like SFML, I'll give sources explaining what they are below.
  
  <details><summary>Information on GLFW and GLAD</summary>

    * Read [this forum thread](https://news.ycombinator.com/item?id=19674197) and watch [this video](https://www.youtube.com/watch?v=zQUYsMYElC0) for an explanation of what GLFW is and a comparison between other popular OpenGL API libraries (GLFW, SFML, SDL, ...).
    * "GLFW is the library to create the window, OpenGL context, and handle all the input-related stuff. Its version is not related to the OpenGL version you use in any way."[^1]
    * "The version of glad is the version of OpenGL you can use. The version of GLFW is irrelevant."[^2]
    * GLFW (Graphics Library Framework) and GLUT (OpenGL Utility Toolkit) "allow us to create a window, and receive mouse and keyboard input. OpenGL does not handle window creation or input, so we have to use these library for handling window, keyboard, mouse, joysticks, input and other purposes."[^3] "GLFW used to stand for OpenGL FrameWork, but now it stands for "Graphics Library Framework". This makes sense, as GLFW is no longer an OpenGL focused library, but also supports Vulkan."[^4]
    * "GLUT and GLFW are basically the same, but what you need to know is that the original GLUT has been unsupported for 20 years, and there is a free open-source alternative for it called freeglut."[^5]
    * [Here](https://stackoverflow.com/questions/25708688/what-is-the-difference-between-freeglut-vs-glfw) is a post describing the differences between GLFW and FreeGLUT.
    * GLAD (Multi-Language GL Loader-Generator) and GLEW (OpenGL Extension Wrangler) are "libraries that manage and give access to OpenGL functions and extensions. The difference is that GLAD allows for greater flexibility & customization for more recent versions of OpenGL."[^6] While it seems debatable how GLAD got its name, ChatGPT tells me: "It conveys the "relief" or "gladness" developers might feel when avoiding the tedious task of manually writing OpenGL function loaders" which I found amusing.
    * [Here](https://stackoverflow.com/a/78197031) is a nice post talking about why we need external OpenGL extension and function loading libraries. "A long time ago (two decades maybe) for some reason operating systems stopped including up-to-date OpenGL headers. I suppose they did it this way so that graphics drivers could have newer OpenGL versions than the operating system. On Windows, it's the app's responsibility to include all the #define numbers itself, and use wglGetProcAddress to get the addresses of the functions. Anyway, an up-to-date copy of GLEW knows all the up-to-date bits of OpenGL. (You may notice it was last updated in 2017 - so was OpenGL as the team moved on to Vulkan after that)."[^7]
    * "[GLEW and GLAD] handle function pointers. You technically don't need to use those libraries to actually use openGL, but you would need to load the function pointers yourself which is very very annoying to do. It's not difficult per se, it's just a long and tedious process. Just look at the headers from those libraries and you'll see that they have a large amount of code.
The reason why you need those function pointers is that the code is located in the driver (and some of it might even be on the gpu). OpenGL, Vulkan and DirectX are only specifications so Khronos (they make OpenGL and Vulkan) and Microsoft ( they make DirectX) do not provide an implementation for them; the gpu driver is charged with implementing all of the specification. You can see that a bit like Khronos is providing the OpenGL .h file and the driver provides the .cpp file. Also, the fact that OpenGL is a state machine has nothing to do about needing to load the function pointers (for example Vulkan is not a state machine and you also need to load the function pointers).
GLEW and GLAD also come with the OpenGL headers because you also need those along with the function pointers, but you can also get those headers from Khronos directly."[^8]
    * "In simple words, GLAD manages function pointers for OpenGL. It is useful becuase OpenGL is only really a standard/specification it is up to the driver manufacturer to implement the specification to a driver that the specific graphics card supports. Since there are many different versions of OpenGL drivers, the location of most of its functions is not known at compile-time and needs to be queried at run-time. GLFW helps us at compile time only."[^9]
    * "OpenGL functions (core or extension) must be loaded at runtime, dynamically, whenever the function in question is not part of the platforms original OpenGL ABI (application binary interface). For Windows the ABI covers is OpenGL-1.1. In Windows you're going to need a function loader for pretty much everything, except single textured, shaderless, fixed function drawing; it may be possible to load further functionality, but this is not a given. The difference between core OpenGL functions and extensions is, that core functions are found in the OpenGL specification, while extensions are functionality that may or may be not available in addition to what the OpenGL version available provides. Both extensions and newer version core functions are loaded through the same mechanism."[^10]
    * "GLFW, the one we use. GLUT, the oldest. Not maintained, horrible license. Not recommended. FreeGlut, same API, but Open Source. SDL I think this one is weird but some people like it. SFML. Includes a coffee machine, a washing machine, and a cellular."[^11]

[^1]: gl_drawelements. "Confused as to which version of glad to use ?" _Reddit_, 9 July 2024, [reddit.com/r/opengl/comments/1dwz5i2/comment/lcb0fkb/](https://www.reddit.com/r/opengl/comments/1dwz5i2/comment/lcb0fkb/).
[^2]: uke5273. "Confused as to which version of glad to use ?" _Reddit_, 6 July 2024, [reddit.com/r/opengl/comments/1dwz5i2/comment/lby4ai8/](https://www.reddit.com/r/opengl/comments/1dwz5i2/comment/lby4ai8/).
[^3]: Fredericvo. "GLFW or GLEW? Which OpenGL library should I use" _GameDev.net_, 22 July 2015, [gamedev.net/forums/topic/670250-glfw-or-glew-which-opengl-library-should-i-use/5241850/](https://gamedev.net/forums/topic/670250-glfw-or-glew-which-opengl-library-should-i-use/5241850/).
[^4]: unigeek, Roovy. "What does GLFW acronym stand for?" _Stack Overflow_, 11 Feb. 2014, 17 Sep. 2019, [stackoverflow.com/questions/21691755/what-does-glfw-acronym-stand-for](https://stackoverflow.com/questions/21691755/what-does-glfw-acronym-stand-for).
[^5]: Nicolas Iceberg. "What is Glew, Glut and glfw3? Which ones are deprecated in Opengl 3/4" _Stack Overflow_, 8 May 2020, [stackoverflow.com/a/61685993](https://stackoverflow.com/a/61685993).
[^6]: Eric Buitrón López. "Exploring Computer Graphics: Weekly Chronicle #1" _DEV Community_, 16 Oct 2023, [dev.to/ericbl3/exploring-computer-graphics-weekly-chronicle-1-3nlj](https://dev.to/ericbl3/exploring-computer-graphics-weekly-chronicle-1-3nlj).
[^7]: Criticize SE actions means ban. "Where to get GLEXT.h?" _Stack Overflow_, 21 Mar. 2024, [stackoverflow.com/a/78197031](https://stackoverflow.com/a/78197031).
[^8]: cppBestLanguage. "Good Explanations of differences between GLFW, GLUT, GLAD, GLEW, etc?" _Reddit_, 8 Jan. 2022, [reddit.com/r/cpp_questions/comments/ryr3fk/comment/hrqyder/](https://www.reddit.com/r/cpp_questions/comments/ryr3fk/comment/hrqyder/).
[^9]: Avtar Sohi. "Why use 'glad' library for opengl initialization?" _Stack Overflow_, 11 Oct. 2021, [stackoverflow.com/questions/55267854/why-use-glad-library-for-opengl-initialization#comment122890513_55268023](https://stackoverflow.com/questions/55267854/why-use-glad-library-for-opengl-initialization#comment122890513_55268023).
[^10]: datenwolf. "When do I need to use an OpenGL function loader?" _Stack Overflow_, 10 Jan. 2015, [stackoverflow.com/a/27876187](https://stackoverflow.com/a/27876187).
[^11]: GraphicsTut. _opengl-tutorial.org_, [opengl-tutorial.org/miscellaneous/useful-tools-links/](http://www.opengl-tutorial.org/miscellaneous/useful-tools-links/).

  </details>

* Info on OpenGL
  <details><summary>Information on OpenGL and Vulkan</summary>
  
    * "In OpenGL getting something on the screen is by far easier. Even without classic fixed function, just rendering full-screen effects or image-processing takes only few lines of code. Vulkan’s level of verbosity to get to the first pixel on the screen is far higher. As hinted in the previous blog posts on resource bindings or memory management, these additional complexities will require more code to be written. Especially for people new to graphics, it may be better to use OpenGL or rendering middleware that hides this complexity and focus on the actual task."[^12]
    * "Fixed-function OpenGL: Pre-assembled toy car, fun out of the box, not much room for customization. Modern AZDO (Approaching [Zero Driver Overhead](https://www.reddit.com/r/GraphicsProgramming/comments/2y9w93/zero_driver_overhead_misleading/)) OpenGL with Programmable Shaders: LEGO Kit, you build it yourself, comes with plenty of useful, pre-shaped pieces. Vulkan: Pine Wood Derby Kit, you build it yourself to race from raw materials, power tools used to assemble, adult supervision highly recommended."[^13]
    * "The fixed-function pipeline is as the name suggests — the functionality is fixed. So someone wrote a list of different ways you'd be permitted to transform and rasterise geometry, and that's everything available. In broad terms, you can do linear transformations and then rasterise by texturing, interpolate a colour across a face, or by combinations and permutations of those things. But more than that, the fixed pipeline enshrines certain deficiencies. For example, it was obvious at the time of design that there wasn't going to be enough power to compute lighting per pixel. So lighting is computed at vertices and linearly interpolated across the face. [...] the programmable pipeline lets you do whatever you want at each stage, giving you complete flexibility. In the first place that allowed better lighting, then better general special effects (ripples on reflective water, imperfect glass, etc), and more recently has been used for things like deferred rendering that flip the pipeline on its end. All support for the fixed-functionality pipeline is implemented by programming the programmable pipeline on hardware of the last decade or so. The programmable pipeline is an advance on its predecessor, afforded by hardware improvements."[^14]
    * "Graphics Processing Units started off very simply with fixed functions, that allowed for quick 3D maths (much faster than CPU maths), and texture lookup, and some simple lighting and shading options (flat, phong, etc). These were very basic but allowed the CPU to offload the very repetitive tasks of 3D rendering to the GPU. Once the Graphics was taken away from the CPU, and given to the GPU, Games made a massive leap forward. It wasn't long before the fixed functions needed to be changed to assembly programs and soon there was demand for doing more than simple shading, basic reflections, and single texture maps offered by the fixed function GPUs. So the 2nd breed of GPU was created, this had two distinct pipelines, one that processed vertex programs and moved verts around in 3D space, and the shader programs that worked with pixels allowing multiple textures to be merged, and more lights and shades to be created. Now in the latest form of GPU all the pipes in the card are generic, and can run any type of GPU assembler code. This increased in the number of uses for the pipe - they still do vertex mapping, and pixel color calculation, but they also do geometry shaders (tessellation), and even Compute shaders (where the parallel processor is used to do a non-graphics job). So fixed function is limited but easy, and now in the past for all but the most limited devices. Programmable function shaders using OpenGL (GLSL) or DirectX (HLSL) are the de-facto standard for modern GPUs."[^15]
    * "The term Fixed Function Pipeline commonly refers to the set of configurable processing state present in older versions of OpenGL that have since been replaced by Shaders of some sort. While the current OpenGL pipeline still retains non-programmable state, that is not typically what people are referring to when they say "fixed function". The purpose of OpenGL is to expose features of the underlying graphics hardware to application developers. In the days before graphics hardware was programmable, such hardware exposed a different rendering pipeline based on user-provided configuration rather than user-provided programs."[^16]
    * "The OpenGL specification defines the concept of the Context. The context contains all of the information that will be used by the OpenGL system to render, when the system is given a rendering command. A context effectively is OpenGL, because OpenGL cannot be used without one. The OpenGL context contains information used by the rendering system. This information is called State, which has given rise to the saying that OpenGL is a "state machine". A piece of state is simply some value stored in the OpenGL context."[^17]
    * "If you want to call any OpenGL function beyond OpenGL 1.1 (and that one is from 1997), you have to use the OpenGL extension mechanism in every case, as opengl32.dll does not provide these entry points at all, and the compiler/linker will of course not find them."[^18]

      [![opengl-ref-manual-v1 4-2004](https://github.com/user-attachments/assets/0fd01ab5-2b7f-4eef-971b-963cc725c36d)](https://www.informit.com/store/opengl-reference-manual-the-official-reference-document-9780321173836)
    * "A long time ago, there was simply OpenGL. At this time, video cards were a new thing and hardware architecture was all over the place. Some computer scientists and software engineers said, "No, no, no, this will not do!" So what did they choose? To make an API that would interface with different hardware drivers as easily as bar and foo. It shipped with Windows as OpenGL 1.0 in 1992. OpenGL is a standard that was defined by the Khronos group before they moved on to Vulkan. They used to implement the standard themselves. At the time, C-style programming was well established, OOP was not a big thing yet, and the OpenGL functions had to be hooked up somehow with different drivers. Solution: Function pointers everywhere. OpenGL's header files are filled with #defines for function pointers. None of them are defined when main(...) begins to run."[^19]


[^12]: "Transitioning from OpenGL to Vulkan" _NVIDIA Developer_, 12 Feb. 2016, [developer.nvidia.com/transitioning-opengl-vulkan](https://developer.nvidia.com/transitioning-opengl-vulkan).
[^13]: Mark Kilgard. "Migrating from OpenGL to Vulkan" _NVIDIA_, 19 Jan. 2016, [my.eng.utah.edu/~cs5610/lectures/Migrating_from_OpenGL_to_Vulkan.pdf](https://my.eng.utah.edu/~cs5610/lectures/Migrating_from_OpenGL_to_Vulkan.pdf).
[^14]: Tommy. "fixed function vs shader based" _Stack Overflow_, 23 Sep. 2013, [stackoverflow.com/a/18950521](https://stackoverflow.com/a/18950521).
[^15]: Strings. "fixed function vs shader based" _Stack Overflow_, 23 Sep. 2013, [stackoverflow.com/a/18952580](https://stackoverflow.com/a/18952580).
[^16]: Alfonse, et al. "Fixed Function Pipeline" _Khronos_, 9 Apr. 2015, [khronos.org/opengl/wiki/Fixed_Function_Pipeline](https://www.khronos.org/opengl/wiki/Fixed_Function_Pipeline).
[^17]: Alfonse. "Portal:OpenGL Concepts" _Khronos_, 14 Sep. 2017, [khronos.org/opengl/wiki/Portal:OpenGL_Concepts](https://www.khronos.org/opengl/wiki/Portal:OpenGL_Concepts).
[^18]: derhass. "Why use 'glad' library for opengl initialization?" _Stack Overflow_, 20 Mar. 2019, [stackoverflow.com/a/55268023](https://stackoverflow.com/a/55268023).
[^19]: amdreallyfast. _Reddit_, 7 Jan. 2017, [reddit.com/r/opengl/comments/5m8iyp/comment/dc4h5g9](https://www.reddit.com/r/opengl/comments/5m8iyp/comment/dc4h5g9).

    
  </details>
  
* I am using MinGW-w64 (GCC) to compile the C++ code. I recommend installing it using [MSYS2](https://www.msys2.org/), [here](https://www.youtube.com/watch?v=C3IxeHthNnM) is a good tutorial. Make sure to install MSYS2 in the C drive (``C:\msys64``), and to run this command in MSYS2 (ensure you're using the ``mingw64`` [environment](https://www.msys2.org/docs/environments/)):

  ```console
  pacman -S --needed base-devel mingw-w64-x86_64-toolchain
  ```
  
  (it's important to run this command and not one with "``ucrt64``" in it as the compiler won't be saved in the correct place and the code won't run). Once it has finished downloading you need to add the filepath to Windows PATH by going to Environment Variables and adding ``C:\msys64\mingw64\bin`` to PATH (I recommend putting it at the top of the list, if you have a long list of filepaths and you put the directory where the compiler is stored at the bottom, it may not work, I learned this the hard way).
* Using www.learnopengl.com as my main resource.

## Progress update 1 - Hello Triangle - 30/06/24
* [This blog](https://medium.com/@vivekjha92/setup-opengl-with-vs-code-82852c653c43#:~:text=The%20Setup,clone%20it%20and%20get%20started.) helped a lot with setting up OpenGL in VSCode.
* I understand how VBOs (vertex buffer objects), VAOs (vertex array objects), and EBOs (element buffer objects) work and why we need them.
* The VAO essentially stores the pointers (memory addresses) for all the vertex attributes (position, colour, etc.) in a specific VBO, which stores the actual values of those vertex attributes.
* An EBO fixes the issue of duplicate vertices when they overlap by storing only the unique vertices and an index to render them in the correct order.
* Was able to render a static 3D RGB cube using 3 fragment shaders and 3 VBOs, 3 VAOs, and 3 EBOs.
* I feel like 3 fragment shaders, 3 VBOs, 3 VAOs, and 3 EBOs are overkill just to render 3 different colours for 3 sets of 2 triangles. There's probably a way to have all 6 triangles in one VBO with their colour attributes also included.
  This will probably be explained in the next chapters.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/OpenGL_EBO.png" width=80%>

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/learnopengl.png" width=49%>
  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/learnopengl2.png" width=49%>

## Progress update 2 - Shaders - 20/07/24
* I understand the render pipeline for OpenGL and what all the shaders in the pipeline do.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/OpenGL_graphics-shader-pipeline.png" width=80%>
  
* Gave each vertex a colour attribute as well as a position attribute, so instead of having 3 VBOs, 3 VAOs, and 3 EBOs, we just have 1 of each and 2 vertex attributes. One for each object makes sense (I assume when we get to rendering 1000s of identical objects we will encounter "instancing").
* Removed the vertex shader and fragment shader GLSL code from the main.cpp file and put them into separate ``shader.vert`` and ``shader.frag`` text files (I recommend installing a GLSL syntax highlighter extension).
* Made a shader class and header file that can handle retrieving source code; compiling, linking, deleting, and activating shaders; defining utility functions; and handling errors.
* I understand what uniform variables are (basically global variables that all shaders can access), you need to define them in the main.cpp file and give them to the shader (using utility uniform functions if you have a separate shader header file, or just by giving the uniform location with ``glUniform[](...)``).
* Switched from OpenGL 3.3 to 4.3 so that I can specify the ``layout (location=...)`` for uniforms, for some reason in OpenGL 3.3 that didn't work. This is a really nice video that helped me understand the ``layout (location=...)`` specifier. At [21:11](https://youtu.be/yrFo1_Izlk0?si=YLplgGlBy1hqR0u2&t=1271) it talks about the specifier and how you can omit the specifier but if you don't then you can change the variable name in the next shader, and vertex attributes and uniforms are stored in different arrays so you can have seemingly two different variables stored in ``location=0`` but they are actually in different arrays.
  [![Watch the video](https://img.youtube.com/vi/yrFo1_Izlk0/maxresdefault.jpg)](https://www.youtube.com/watch?v=yrFo1_Izlk0)

* This animation was done by storing a green colour uniform variable in the render loop that changes over time, this uniform variable can be accessed by the fragment shader to render the triangle with changing colour.

  https://github.com/user-attachments/assets/e2bdee00-6a7d-4f6b-a29b-513f5611c3d3

* This rainbow triangle was done by storing a red, green, and blue colour value as a second vertex attribute for each of the 3 vertices. "Fragment interpolation" occurs, where a linear combination of the colours is used for all the fragments (can think of them as pixels) between the 3 vertices.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/rainbow-shader.png" width=50%>

* This flipped offset gradient colour triangle was done by setting an offset float variable in the ``main.cpp`` file, then setting it as a uniform in the render loop using the utility uniform functions created in the shader header file. The weird colours were done by defining the ``FragColor`` to be the positions of the 3 vertices rather than the colour (Exercise 3), and fragment interpolation causes the gradient effect. (One thing to note is that if you input a negative float into one of the RGB channels it will be clamped to 0.0f (black)).

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/rainbow-shader2.png" width=50%>

## Progress update 3 - Textures - 22/08/24
* I now understand how textures work, and I was able to render this rectangle with a crate PNG texture with an RGB overlay and the "Awesome face" PNG. I still need to do the exercises.

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/textures.png" width=50%>

* Alongside learning how to use textures, I wanted to do a mini-project rendering a rotating animated RGB triangle and learning how to render off-screen frames as images to memory rather than to a window on-screen.
* The texture section introduced me to the [stb](https://github.com/nothings/stb) repo by Sean Barrett, in particular the ``stb_image.h`` header, a single header image loading library used to load an image into a texture; and the ``stb_image_write.h`` header, used for image writing from OpenGL to disk (PNG).
* In order to make the triangle spin I used the standard 2D [rotation matrix](https://en.wikipedia.org/wiki/Rotation_matrix).

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl-rotation-matrix.png" width=100%>
  <!--
  &nbsp;&nbsp;
  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl2.png" width=53%>
  -->
  
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

* In order to make the colours inside the triangle spin I used the [Sinebow](https://basecase.org/env/on-rainbows) over the HSV function as it has no branches making it faster for GPGPUs.[^22]

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/HSV-vs-Sinebow.png" width=40%>

  <!--
  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl3.1.png" width=48%>
  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl4.1.png" width=45%>
  -->

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
  
* In order to implement off-screen rendering I initially found [this blog post](https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/) which worked fine but I felt like the rendering could be done faster which led me to a [few posts](https://stackoverflow.com/a/25127895) from Stack Overflow that said to use [PBOs (pixel buffer objects)](https://www.khronos.org/opengl/wiki/Pixel_Buffer_Object), which allow for [asynchronous readback](https://www.songho.ca/opengl/gl_pbo.html#pack), which means rendering to system memory later rather than as soon as possible in the hopes of it being faster. Unfortunately, I found PBOs to not make any difference in performance, which led me to discover FBOs (framebuffer objects) which are essentially a non-default [framebuffer](https://www.khronos.org/opengl/wiki/Framebuffer) (unlike the FRONT and BACK buffers which are) that allows you to do proper off-screen rendering to a memory buffer instead of the default screen buffers[^20] (OpenGL forces a window to be loaded on-screen, but with FBOs it will be black and you can just hide the screen (but don't minimize!), Vulkan is designed to support off-screen rendering better than OpenGL[^21] but is more verbose[^12] and thus harder to learn (maybe in the future...)). FBOs are optimized for data to be read back to the CPU, while the default buffers are made to stay on the GPU and display pixels on-screen.[^21]

  <img src="https://github.com/dhanushka2001/LearnOpenGL/blob/main/images/opengl5.3.png" width=80%>
  
* Learning about FBOs naturally led me to learn about [RBOs (renderbuffer objects)](https://www.khronos.org/opengl/wiki/Renderbuffer_Object) which are specifically used by FBOs as a render target. Textures can alternatively be used if you want to re-use the pixels on-screen (e.g. a naive "security camera" in a game)[^20] or sample the pixels for post-processing,[^24] however since we just want to read-back the pixels and render off-screen, RBOs are the logical choice.
* I have decided to keep the code with the PBO as even though it doesn't make any performative difference and just adds more lines of code, I may need them in the future. I ended up using 2 PBOs, while one has pixel data from the FBO written into it, the other is being mapped for reading. This has no performative benefit to just reading and writing to system memory from the FBO directly on every cycle since on every cycle I am still invoking ``glReadPixels`` which stalls the pipeline "because it has to safely map the data from the read buffer to the PBO and to a block of memory in main RAM", and I am mapping the other PBO which also stalls the pipeline until the pixel data has been converted to a png and saved to system memory.[^20]
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

<!--
1. [How to render offscreen on OpenGL?](https://stackoverflow.com/a/12159293)
2. [How to use GLUT/OpenGL to render to a file?](https://stackoverflow.com/a/14324292) 
3. [On rainbows by Charlie Loyd](https://basecase.org/env/on-rainbows)
4. [Save the OpenGL rendering to an image file - Lencerfs Walk](https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/)
5. [Transitioning from OpenGL to Vulkan- NVIDIA](https://developer.nvidia.com/transitioning-opengl-vulkan)
-->

[^20]: KillianDS. "How to render offscreen on OpenGL?" _Stack Overflow_, 28 Aug. 2012, [stackoverflow.com/a/12159293](https://stackoverflow.com/a/12159293).
[^21]: Ciro Santilli OurBigBook.com. "How to use GLUT/OpenGL to render to a file?" _Stack Overflow_, 14 Jan. 2013, [stackoverflow.com/a/14324292](https://stackoverflow.com/a/14324292).
[^22]: Charlie Loyd. "On rainbows" _Env - basecase.org_, 25 Dec. 2011, [basecase.org/env/on-rainbows](https://basecase.org/env/on-rainbows).
[^23]: Lencerf. "Save the OpenGL rendering to an image file" _Lencerf's Walk_, 20 Sep. 2019, [lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/](https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/).
[^24]: Song Ho Ahn (안성호). "OpenGL Pixel Buffer Object (PBO)" _songho.ca_, [songho.ca/opengl/gl_pbo.html#pack](https://www.songho.ca/opengl/gl_pbo.html#pack).

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

* I have finished all the exercises for the **Textures** chapter, and I did my own small project to allow the user to move around the object and control the opacity. Now the real-time encoded video starts to show signs of deviating from the on-screen render, the object in the encoded video moves slower than in the on-screen render, which I assume is the result of the video frames not being encoded at ~60fps, due to the I/O bottleneck. This shows how fast FBOs render off-screen compared with the BACK buffer (which wasn't designed for that task),[^21] as the spinning RGB triangle rendered perfectly even though I assume it is more intensive than the crate/awesome-face texture. The solution would have to involve implementing the packet method mentioned above, it would be ideal to use FBOs to render off-screen but the user would also need to see the screen to interact with it in real-time, I'm not sure if there is a way to use FBOs and the BACK buffer at the same time though. The solution might involve PBOs, one rendering the current frame on-screen while the other stores the previous frame in an FBO to be sent to a buffer which will be periodically encoded using FFmpeg to build the video asynchronously.

  https://github.com/user-attachments/assets/d86f9581-7abe-4446-9abc-8528bee0bff9

## Progress update 5 - Simultaneous on-screen and off-screen rendering - 16/11/24

* I am now finally able to render on-screen and off-screen simultaneously. I was able to do this naively by just rendering to the BACK buffer and using glReadPixels to render off-screen, however, this isn't ideal as I have already explained, the BACK buffer isn't designed for data to be read back to the CPU. A better method would be to render to a FBO, read the pixel data and feed it to FFmpeg to encode a video off-screen, and somehow also render the FBO pixel data on-screen. An FBO stores pixel data not using the default window framebuffer so it won't be visible, it is designed to be read back to the CPU which is partly what we want. My first approach to get the pixel data to be visible on-screen was to use a fullscreen quad and the texture of the FBO, which others had suggested.[^25] I spent a lot of time down this path, which involved using another shader program to render the quad, but after spending many, many days on this it just didn't work, I got off-screen rendering to work but not on-screen rendering. At least I learned about using multiple shader programs. The next approach which did work was to blit (copy) the FBO pixel data to the default framebuffer, which I probably should have just done from the start, this solution also just seems to be better performance-wise than the fullscreen quad approach as it means less API calls, not needing to bind another shader program, and many GPUs have dedicated units for blitting data.[^26] While this does work, I would eventually like to go back to FBO textures and a fullscreen quad as this seems like you can do more stuff like rendering the scene onto objects (textures are easier to manipulate/attach to objects), as well as post-processing using PBOs to modify the texture efficiently.[^27]
  
  ```cpp
  // Step 1: Render the scene to the MSAA FBO
  // ----------------------------------------
  glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
  glEnable(GL_DEPTH_TEST); // Needed for 3D rendering
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  RenderCrate(ourShader);
  
  // Step 2: Resolve MSAA FBO to standard non-MSAA FBO
  // -------------------------------------------------
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
  glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                    0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                            GL_COLOR_BUFFER_BIT,           // buffer mask
                                     GL_LINEAR);           // scale filter

  // Step 3: Render the scene for on-screen rendering using Blitting: https://stackoverflow.com/a/31487085
  // -----------------------------------------------------------------------------------------------------
  // Bind the target FBO to read
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                    0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                            GL_COLOR_BUFFER_BIT,           // buffer mask
                                    GL_NEAREST);           // scale filter
  ```

* I adjusted the FFmpeg command to encode the video at 60fps rather than 30fps, I should have done that from the beginning, as it matches the 60fps on-screen window. At 30fps, the encoded video seems sluggish. Unfortunately, you will have to manually adjust the frame resolution if needed, I wanted to make it automated but formatting strings in C++ is headache-inducing (side note: it seems like they made it easier to format strings in C++20 using ``std::format``, the person responsible for standardizing it and getting it through the C++20 committee wrote a blog post essentially explaining "what took so long?": https://vitaut.net/posts/2019/std-format-cpp20/, but as I am using C++17 I won't bother unless I choose to update my C++ version).
  ```cpp
  // Path to ffmpeg binary, if it's not in the system path, provide the full path. MAKE SURE THE FRAME RESOLUTION IS CORRECT!
  const char* FFmpegCommand = "ffmpeg -y -f rawvideo -pixel_format rgb24 -video_size 800x600 -framerate 60 -i - -c:v libx264 -pix_fmt yuv420p output.mp4 2> ffmpeg_log.txt";
  ```

  https://github.com/user-attachments/assets/ec9950bc-0568-404e-a57b-5b08b84452d0

  I am still blown away by how quickly the encoded video is rendered, basically instantly after the program terminates, the max runtime I've tried is 1 minute, maybe I should try rendering for longer, but I don't think it would be any less fast since FFmpeg is encoding the video frame by frame every cycle in the render loop. This could also just be due to the scene not being very intensive. The on-screen and off-screen renders match basically exactly, now I can do whatever I want, my first goal is to do some fractal rendering, continue the LearnOpenGL sections, and once I've learnt about instancing, try to implement the Saturn's rings code in OpenGL.

* I also changed from using a C++ vector to using an array to store the frame (pixel) data, as this seems better suited to the job as the frame data is of a fixed size, and an array uses the heap rather than the stack making it faster.
  ```cpp
  // Frame buffer to hold the raw frame data (RGB)
  // std::vector<unsigned char> frame(SCR_WIDTH * SCR_HEIGHT * 3);
  std::array<unsigned char, SCR_WIDTH * SCR_HEIGHT * 3> frame;
  ```

<!--
19. [OpenGL rendering from FBO to screen?](https://stackoverflow.com/a/10400048)
20. [OpenGL - Is there an easier way to fill window with a texture, instead using VBO,etc?](https://stackoverflow.com/a/31487085) 
21. [OpenGL Frame Buffer Object (FBO) - Example: Render To Texture - Songho](http://www.songho.ca/opengl/gl_fbo.html#example)
-->

[^25]: genpfault. "OpenGL rendering from FBO to screen" _Stack Overflow_, 1 May 2012, [stackoverflow.com/a/10400048](https://stackoverflow.com/a/10400048).
[^26]: Reto Koradi. "OpenGL - Is there an easier way to fill window with a texture, instead using VBO,etc?" _Stack Overflow_, 18 July 2015, [stackoverflow.com/a/31487085](https://stackoverflow.com/a/31487085).
[^27]: Song Ho Ahn (안성호). "OpenGL Frame Buffer Object (FBO)" _songho.ca_, [songho.ca/opengl/gl_fbo.html#example](http://www.songho.ca/opengl/gl_fbo.html#example).

## Progress update 6 - Text rendering - 29/11/24
* I wanted to be able to render text on-screen, to display the FPS for example. Rendering text in OpenGL is surprisingly non-trivial, a quick and easy way to see the FPS is to display it on the title of the window.
  ```cpp
  // FPS Counter: https://www.youtube.com/watch?v=BA6aR_5C_BM
  newTime = glfwGetTime();
  deltaTime = newTime - oldTime;
  counter++;
  // update FPS every 30th of a second (you can change this)
  if (deltaTime >= 1.0 / 30.0)
  {
  // Creates new title
  std::string FPS = std::to_string((1.0 / deltaTime) * counter);
  std::string ms = std::to_string((deltaTime / counter) * 1000);
  std::string newTitle = "LearnOpenGL - " + FPS + "FPS / " + ms + "ms";
  glfwSetWindowTitle(window, newTitle.c_str());
  
  // Resets times and counter
  oldTime = newTime;
  counter = 0;
  }
  ```
<!-- PICTURE OF FPS IN TITLE BUT NOT ON SCREEN AS TEXT HERE -->
* This is fine but it would be nice to see the FPS as text on-screen as then I can see it in the encoded video recording too. Getting text to appear on screen turned out to be very difficult.
* There are two main ways to render text in OpenGL, the old way of rendering text using bitmap fonts, which is fast but you're limited with the number of characters and size, or the modern way which involves TrueType fonts that use mathematical equations (splines) to give better quality and you can change the size easily, but this method is more involved. I opted for the modern way as I felt it would be more useful to learn, however it was a pain to set up.
* The [chapter](https://learnopengl.com/In-Practice/Text-Rendering) on LearnOpenGL.com for modern text rendering actually doesn't give the best way to do TrueType font rendering, they generate a texture for each glyph/character of the text, which they even admit is not good performance-wise. They did however recommend at the end that the best way would be to combine the old and new approach, dynamically generating a rasterized bitmap font texture atlas featuring all TrueType character glyphs as loaded with FreeType, which is what I decided to try implementing.
* Before doing this I first had to download and import FreeType, which is used to load the TrueType fonts. I wish I could say it was as simple as downloading FreeType and adding the include statements, but it wasn't. FreeType has a lot of dependencies which I had in my ``C:\msys64\mingw64\lib`` and ``C:\msys64\mingw64\include`` folders but they weren't being recognised. I wanted to make my project folder self-contained so I decided to move all dependencies to the project lib and include folders, this took quite a while as there was a lot of dependencies that weren't explicitly told and I was only made aware after being given error after error and checking online which dependency files needed to be added and then linking to them in the ``tasks.json`` file's ``args`` and the ``c_cpp_properties.json`` file's ``includePath``. I was honestly ready to give up but after copying over enough headers (.h files) and statically linked library files (.a files) I no longer had any errors.
* I used MSYS2 to build FreeType and all its dependencies, running this command (ensure you're using the ``mingw64`` [environment](https://www.msys2.org/docs/environments/)):

   ```console
  pacman -S mingw-w64-x86_64-freetype
  ```

* Verify installation:
  * Library file: Look for ``libfreetype.a`` in ``/mingw64/lib``.
  * Header files: Look for FreeType headers in ``/mingw64/include/freetype2``.
  If these files exist, the installation was successful. 

* On the [MSYS2 website](https://packages.msys2.org/packages/mingw-w64-x86_64-freetype) it shows the dependencies. I find that when building FreeType it will prompt you to also install the dependencies, so they should already be located in your ``C:\msys64\mingw64\lib`` and ``C:\msys64\mingw64\include`` folder, I recommend copying the dependency files to your project's ``lib`` and ``include`` folders. If you run ``main.cpp`` and you get ``undefined reference...`` errors then it probably means you are missing a dependency file or haven't linked to them in your compiler flags. There are two additional dependencies that you will need to also add: ``rpcrt4`` (Windows RPC (Remote Procedure Call) runtime library) and ``gdi32`` (Windows GDI (Graphics Device Interface)).

  |  Dependency  |             Description              |               MSYS2 command              | Library location | Header location | Compiler flag  | Linker flag   |
  |     :---:    |                :---:                 |                    :---:                 |       :---:      |     :---:       |     :---:      |       :---:   |
  | ``brotli``   | compression library                  | ``pacman -S mingw-w64-x86_64-brotli``    | ``C:\msys64\mingw64\lib\libbrotlicommon.a`` ``C:\msys64\mingw64\lib\libbrotlidec.a`` ``C:\msys64\mingw64\lib\libbrotlienc.a`` | ``C:\msys64\mingw64\include\brotli\`` | ``-I${workspaceFolder}/include/brotli`` | ``"-lbrotlidec", "-lbrotlienc", "-lbrotlicommon"`` |
  | ``libpng``   | for PNG support                      | ``pacman -S mingw-w64-x86_64-libpng``    | ``C:\msys64\mingw64\lib\libpng.a`` | ``C:\msys64\mingw64\include\libpng16\`` |  | ``"-lpng"`` |
  | ``zlib``     | compression library                  | ``pacman -S mingw-w64-x86_64-zlib``      | ``C:\msys64\mingw64\lib\libz.a`` | ``C:\msys64\mingw64\include\zlib.h`` |  | ``"-lz"`` |
  | ``libbz2``   | optional, for BZip2-compressed fonts | ``pacman -S mingw-w64-x86_64-bzip2``     | ``C:\msys64\mingw64\lib\libbz2.a`` | ``C:\msys64\mingw64\include\bzlib.h`` |  | ``"-lbz2"`` |
  | ``HarfBuzz`` | text shaping library                 | ``pacman -S mingw-w64-x86_64-harfbuzz``  | ``C:\msys64\mingw64\lib\libharfbuzz.a`` | ``C:\msys64\mingw64\include\harfbuzz\`` | ``"-I${workspaceFolder}/include/harfbuzz"`` | ``"-lharfbuzz"`` |
  | ``Graphite2``| text shaping library                 | ``pacman -S mingw-w64-x86_64-graphite2`` | ``C:\msys64\mingw64\lib\libgraphite2.a`` | ``C:\msys64\mingw64\include\graphite2\`` | ``"-I${workspaceFolder}/include/graphite2"`` | ``"-lgraphite2"`` |
  | ``rpcrt4``| Windows RPC runtime library |  | ``C:\msys64\mingw64\lib\librpcrt4.a`` |  |  | ``"-lrpcrt4"`` |
  | ``gdi32``| Windows GDI (Graphics Device Interface) |  | ``C:\msys64\mingw64\lib\libgdi32.a`` |  |  | ``"-lgdi32"`` |

  <!--
  mingw-w64-x86_64-brotli
  mingw-w64-x86_64-bzip2
  mingw-w64-x86_64-gcc-libs
  mingw-w64-x86_64-harfbuzz
  mingw-w64-x86_64-libpng
  mingw-w64-x86_64-zlib
  -->

* Copy files to your local project folder:
  * Copy the Library Files:
    ```bash
    YourProject
    └── lib
        ├── libfreetype.a
        ├── libbrotlicommon.a
        ├── libbrotlidec.a
        ├── libbrotlienc.a
        ├── libpng.a
        ├── libz.a
        ├── libbz2.a
        ├── libharfbuzz.a
        ├── libgraphite2.a
        ├── libharfbuzz.a
        ├── librpcrt4.a
        ├── libgdi32.a
        ├── ...
    ```
  
  * Copy the Header Files: Copy the entire ``freetype2``, ``brotli``, ``graphite2``, ``harfbuzz`` and ``libpng16`` folders into your ``include`` folder as well as the ``zlib.h`` and ``bzlib.h`` files.
    ```bash
    YourProject
    └── include
        ├── freetype2
        │   ├── ft2build.h
        │   └── freetype
        │       ├── freetype.h
        │       ├── ftglyph.h
        │       └── ...
        ├── brotli
        │   ├── decode.h
        │   ├── encode.h
        │   └── ...
        ├── graphite2
        │   ├── Font.h
        │   ├── Log.h
        │   └── ...
        ├── harfbuzz
        │   ├── hb.h
        │   ├── hb-aat.h
        │   └── ...
        ├── libpng16
        │   ├── png.h
        │   ├── pngconf.h
        │   └── ...
        ├── zlib.h
        ├── bzlib.h
        ├── ...
    ```
  
* Update Compiler Flags in ``tasks.json`` (tell your compiler where to find the libraries and headers):
  * Add ``"-I${workspaceFolder}/include",`` to specify the include directory.
  * Add ``"-I${workspaceFolder}/include/freetype2",`` to specify the freetype include directory.
  * Add ``"-L${workspaceFolder}/lib2",`` to specify the library directory.
  * Add ``"-lfreetype",`` to link the FreeType library.
  * Add ``"-lharfbuzz",`` to link the HarfBuzz library.
  * Add ``"-lgraphite2",`` to link the Graphite2 library.
  * Add ``"-lpng",`` to link the libpng library.
  * Add ``"-lz",`` to link the zlib library.
  * Add ``"-lbz2",`` to link the libbz2 library.
  * Add ``"-lbrotlidec", "-lbrotlienc", "-lbrotlicommon",`` to link the Brotli library.
  * Add ``"-lrpcrt4",`` to link the Windows RPC runtime library.
  * Add ``"-lgdi32"`` to link the Windows GDI (Graphics Device Interface).
* In your ``main.cpp`` file, include FreeType by adding the following include statements:
  ```cpp
  #include <ft2build.h>
  #include FT_FREETYPE_H
  ```
  ``ft2build.h`` is a configuration header provided by FreeType, this file is the entry point that sets up the necessary paths for the FreeType headers. You don't include 
  ``freetype.h`` directly. Instead, after including ``ft2build.h``, you include ``freetype.h`` indirectly using ``#include FT_FREETYPE_H``. This macro is defined in 
  ``ft2build.h`` and resolves the correct path for the ``freetype.h`` header based on your FreeType installation.
* And just like that we have finished linking all the dependencies and now the C++ code should run with no errors, this took way too long and was such a pain at the time. In the future I might scrap all of this and use a Makefile or CMake to build the program with all the dependencies, there's a nice video explaining how to do all this. But for now, this works fine as is.
* Now that we have FreeType, we first need to load the font we want to use, it will be helpful to put this step in its own function.
  ```cpp
  const std::string fontFilepath = "C:/WINDOWS/FONTS/ARIAL.TTF";
  FT_Library ft;
  FT_Face face;
  FT_UInt fontsize = 48;

  // Load the font face (you should have a valid file path to the font)
  bool loadFont(FT_UInt &fontsize) {
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
      if (FT_Set_Pixel_Sizes(face, 0, fontsize)) {
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
  ```
* The next step is to create the texture atlas that will store all the rasterized glyphs (characters) in the font, we put this in its own function as well. You should create a Glyph ``struct`` and a glyphs ``map`` to the top of your ``main.cpp`` code that will store info about each glyph.
  ```cpp
  struct Glyph {
      float textureX, textureY;  // Texture coordinates in the atlas
      float width, height;       // Glyph's width and height
      float offsetX, offsetY;    // Offsets (for positioning)
      unsigned int advanceX;     // Horizontal advance (for spacing)
  };
  GLuint textureAtlasID;
  std::map<char, Glyph> glyphs;  // Store info about each glyph
  ```
  ```cpp
  // Create texture atlas with all the glyphs
  void createTextureAtlas() {
      // Variables for positioning glyphs in the atlas
      int offsetX         = 0;
      int offsetY         = 0;
      int rowHeight       = 0;
      // Variables for calculating area used/wasted
      int maxWidth        = 0;
      int totalglyphArea  = 0;
      int wastedArea      = 0;
      int minWastedArea   = 0;
  
      // int maxAscent, maxDescent = 0;
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
          // std::cout << "Processing character: " << c << std::endl;
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
              // std::cerr << "Warning: Glyph '" << c << "' has no valid bitmap data!" << std::endl;
              // continue; // Skip this character (comment out to allow for spaces)
          }
          if (glyphs.find(c) != glyphs.end()) {
              std::cerr << "Error: Character " << c << " already exists in glyph map!" << std::endl;
              break;
          }
          // Check if character doesn't fit in the row
          if (offsetX + g->bitmap.width >= atlasWidth) {
              std::cerr << "REACHED ATLAS WIDTH LIMIT. STARTING NEW ROW. " << offsetX << " + " << g->bitmap.rows << " = " << offsetX + static_cast<int>(g->bitmap.width) << " >= " << atlasWidth << std::endl;
              maxWidth = std::max(maxWidth, offsetX);
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
              static_cast<float>(offsetX) / (float)atlasWidth,        // TextureX
              static_cast<float>(offsetY) / (float)atlasHeight,       // TextureY
              static_cast<float>(g->bitmap.width),                    // width
              static_cast<float>(g->bitmap.rows),                     // height
              static_cast<float>(g->bitmap_left),                     // OffsetX
              static_cast<float>(g->bitmap_top),                      // OffsetY
              static_cast<unsigned int>(g->advance.x)                 // AdvanceX
          };
          // maxAscent = int(face->ascender * (face->size->metrics.y_scale / 65536.0)) >> 6;
          // maxDescent = int(abs(face->descender * (face->size->metrics.y_scale / 65536.0))) >> 6;
          totalglyphArea += static_cast<int>(g->bitmap.width) * static_cast<int>(g->bitmap.rows);
          offsetX += g->bitmap.width;
          rowHeight = std::max(rowHeight, static_cast<int>(g->bitmap.rows));
  
          // std::cout << "Loaded character: " << c << " (" << static_cast<int>(c) << ")" << std::endl;
      }
      
      glBindTexture(GL_TEXTURE_2D, 0);
  
      wastedArea = atlasWidth * atlasHeight - totalglyphArea;
      minWastedArea = wastedArea - (atlasHeight*(atlasWidth-maxWidth)) - (maxWidth*(atlasHeight-offsetY));
      
      std::cout << " | Texture atlas created: " << atlasWidth << "x" << atlasHeight
                << " | Wasted area: " << wastedArea*100/(atlasWidth*atlasHeight) << "%"
                << " | Minimum size: " << maxWidth << "x" << offsetY
                << " | Minimum wasted area: " << minWastedArea*100/(maxWidth*offsetY) << "% |"
                << std::endl;
  
      // Format output in columns: https://stackoverflow.com/a/49295288
      // for (const auto& [key, glyph] : glyphs) {
      //     std::cout.precision(6);
      //     std::cout << " | " << "Glyph: "                     << static_cast<char>(key)   << " | "
      //                        << "TextureX: "  << std::setw(9) << glyph.textureX           << " | "
      //                        << "TextureY: "  << std::setw(9) << glyph.textureY           << " | "
      //                        << "Width: "     << std::setw(2) << glyph.width              << " | "
      //                        << "Height: "    << std::setw(2) << glyph.height             << " | "
      //                        << "OffsetX: "   << std::setw(2) << glyph.offsetX            << " | "
      //                        << "OffsetY: "   << std::setw(2) << glyph.offsetY            << " | "
      //                        << "AdvanceX: "  << std::setw(5) << glyph.advanceX           << " | "
      //                        << std::endl;
      // }
  }
  ```
  The way the texture atlas shifts each glyph into place is a little confusing, so I made this diagram below. Each glyph is contiguously placed in a row, and when a new glyph goes outside the bounds of the width of the texture atlas, it starts a new row.

  ![textureAtlas](https://github.com/user-attachments/assets/7c9cd38e-89d1-4f1f-9407-2002abad67d3)

  
* These two functions above won't be inside the render loop so they don't need to be very efficient. The final function will be, so every step should be scrutinized. The final function will be responsible for rendering the text to the framebuffer given some inputs like the text, position, and color, as well as the shader to handle text rendering, which we also need to create.
  ```cpp
  // Render text (this function is in the render loop)
  void RenderText(Shader &textShader, const std::string &text, float x, float y, float scale, glm::vec3 color) {
      // Use your text rendering shader
      textShader.use();
      textShader.setVec3("textColor", color);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textureAtlasID);  // Bind the texture atlas
      textShader.setInt("textTextureAtlas", 0);
  
      // Enable 2D rendering
      glEnable(GL_BLEND); // enable transparency
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
      // Set up the transformation matrix for the text position
      glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
      textShader.setMat4("projection", projection);
  
      glBindVertexArray(textVAO);
      glBindBuffer(GL_ARRAY_BUFFER, textVBO);
  
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
              { xpos,     ypos + h,   tx,      ty + th }, // Top-left
              { xpos,     ypos,       tx,      ty      }, // Bottom-left
              { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right
  
              { xpos,     ypos + h,   tx,      ty + th }, // Top-left
              { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right
              { xpos + w, ypos + h,   tx + tw, ty + th }  // Top-right
          };
  
          glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);            // ideal for small subset updates
          // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // better for reallocating and initializing large buffers
  
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
      // glDisable(GL_BLEND); // disable transparency
  }
  ```
* You'll notice I use ampersands (&) before the input variables for the functions (``void RenderText(Shader &textShader, const std::string &text, float x, float y, float scale, glm::vec3 color)``), this is known as "passing by reference", "it allows a function to modify a variable without having to create a copy of it."[^29] In C++ there are two main ways to pass variables to functions, "pass by reference" and "pass by value".[^28] With pass by reference you give the function the memory location where the variable is stored, allowing it to change the original variable, this can be done using referencing (``int &variable``) or pointers (``int *variable``), it is generally recommended to use referencing over pointers.[^29] With pass by value we give the function a copy of the variable which it can modify and leaves the original variable alone. This is good when you don't want to change the original variable, but as you can imagine will be slow if you pass large arrays as it will need to copy the entire array.[^28] You can "retain the performance advantages of pass by reference and still protect our variables from changes by passing a const reference"[^28] which is what I did inside the ``RenderText()`` function when looping through the characters in the text: ``for (const char &c : text) {``. "Passing small types like int or float by value is fine, as they are the same size as a reference"[^28] which is why I didn't use referencing for the float variables.

  [^28]: Dr. Michael McLeod. "COMP0210: Research Computing with C++ - Week 2: Pass by Value and Pass by Reference" _UCL_, [github-pages.ucl.ac.uk/research-computing-with-cpp/02cpp1/sec02PassByValueOrReference.html](https://github-pages.ucl.ac.uk/research-computing-with-cpp/02cpp1/sec02PassByValueOrReference.html).
  [^29]: Rohit Kasle. "Passing By Pointer vs Passing By Reference in C++" _GeeksforGeeks.org_, 11 Oct. 2024, [geeksforgeeks.org/passing-by-pointer-vs-passing-by-reference-in-cpp/](https://www.geeksforgeeks.org/passing-by-pointer-vs-passing-by-reference-in-cpp/).
  
* Vertex shader for the text shader program.
  ```cpp
  #version 430 core
  
  layout (location = 0) in vec4 vertex; // x, y, z, w -> position and texture coords
  out vec2 TexCoord; // Texture coordinates to pass to the fragment shader
  
  uniform mat4 projection; // Projection matrix to transform the text positions
  
  void main()
  {
      gl_Position = projection * vec4(vertex.x, vertex.y, 0.0, 1.0); // Apply projection
      TexCoord = vertex.zw; // Set texture coordinates
  }
  ```
* Fragment shader for the text shader program.
  ```cpp
  #version 430 core
  
  in vec2 TexCoord;   // Texture coordinates from the vertex shader
  out vec4 FragColor; // Final color of the pixel
  
  uniform sampler2D textTextureAtlas; // The texture atlas containing all the glyphs
  uniform vec3 textColor;             // The color of the text (usually white or any desired color)
  
  void main()
  {
      // Sample the texture atlas at the given coordinates
      // Alpha is stored in the red channel of the texture
      float alpha = texture(textTextureAtlas, TexCoord).r;
      
      // If the glyph has an alpha value (not transparent), render it
      if (alpha < 0.1) {
          discard; // Avoid rendering transparent parts of the glyph
      }
      // Apply the text color to the glyph
      FragColor = vec4(textColor, alpha);           // Set the text color, using alpha for transparency
  }
  ```

* I switched from using ``std::array`` to using ``malloc()`` to store the RGB pixel data. ``std::array`` stores data on the stack[^33] which is limited in size and caused a stack overflow[^30][^31] when I tried increasing the size of the frame. From what I understand, ``malloc()`` and ``std:vector`` create a pointer on the stack that manages a block of memory on the heap.[^34][^35][^36] The heap is not as fast and you have to manually deallocate memory, but you have much more memory on the heap.[^30][^32] The ``malloc()`` type-casting, ``(usigned char*) malloc(...)`` is frowned upon in C as it suppresses useful compiler diagnostics, however it seems to be necessary when using ``malloc()`` in C++. Although upon further reading people seem to say that you should not use ``malloc()`` and ``free()`` in C++ but instead ``new`` and ``delete``.[^37][^38]
  ```cpp
  unsigned char* frame;
  // Frame buffer to hold the raw frame data (RGB)
  // std::vector<unsigned char> frame(SCR_WIDTH * SCR_HEIGHT * 3);
  // std::array<unsigned char, SCR_WIDTH * SCR_HEIGHT * 3> frame;
  // Why use unsigned char? https://stackoverflow.com/a/13642984
  frame = (unsigned char*) malloc(SCR_WIDTH * SCR_HEIGHT * 3);
  ```
  
[^30]: "What’s the difference between a stack and a heap?" _ProgrammerInterview.com_, [programmerinterview.com/data-structures/difference-between-stack-and-heap/](https://www.programmerinterview.com/data-structures/difference-between-stack-and-heap/).
[^31]: nullDev. "When is it best to use the stack instead of the heap and vice versa?" _Stack Overflow_, 19 Sep. 2008, [stackoverflow.com/a/102061](https://stackoverflow.com/a/102061).
[^32]: Jeff Hill. "What and where are the stack and heap?" _Stack Overflow_, 17 Sep. 2008, [stackoverflow.com/a/80113](https://stackoverflow.com/a/80113).
[^33]: Yakk - Adam Nevraumont. "Does std::array<> guarantee allocation on the stack only?" _Stack Overflow_, 17 Sep. 2016, [stackoverflow.com/a/39549597](https://stackoverflow.com/a/39549597).
[^34]: collin. "Heap Memory in C Programming" _Stack Overflow_, 17 Apr. 2012, [stackoverflow.com/a/10200727](https://stackoverflow.com/a/10200727).
[^35]: nysra. "Is std::vector allocated on Heap?" _Reddit_, 1 Oct. 2023, [reddit.com/r/cpp_questions/comments/16wzd94/comment/k2zm68x](https://www.reddit.com/r/cpp_questions/comments/16wzd94/comment/k2zm68x).
[^36]: Doug T.. "OpenGL rendering from FBO to screen" _Stack Overflow_, 28 Apr. 2012, [stackoverflow.com/a/10366497](https://stackoverflow.com/a/10366497).
[^37]: Lundin. "Should I cast the result of malloc (in C)?" _Stack Overflow_, 20 Mar. 2014, [stackoverflow.com/a/22538350/7875204](https://stackoverflow.com/a/22538350/7875204).
[^38]: John Bode. "Why does C++ require a cast for malloc() but C doesn't?" _Stack Overflow_, 13 Aug. 2010, [stackoverflow.com/a/3477952/7875204](https://stackoverflow.com/a/3477952/7875204).

* The function to update the FPS
  ```cpp
  void UpdateFPS() {
      float currentTime = glfwGetTime();
      frameCount++;
  
      // Calculate FPS every 10th of a second
      if (currentTime - lastTime >= 1.0f / 10.0f) {
          fps = frameCount / (currentTime - lastTime);
          msPerFrame = 1000.0f / fps; // Convert to milliseconds
          frameCount = 0;
          lastTime = currentTime;
      }
  }
  ```
* The function to get the text to display
  ```cpp
  std::string GetFPSText(float fps, float ms) {
      char buffer[100];
      // "FPS: %.1f | %.1f ms"
      // "AaBbCcDdEeFfGg1!2£4$"
      // "In the dream, they took me to the light. A beautiful lie."
      snprintf(buffer, sizeof(buffer), "FPS: %.3f | %.3f ms | Press ESC to exit, F11 for fullscreen", fps, ms);
      return std::string(buffer);
  }
  ```
* Outside render loop:
  ```cpp
  // load font and create texture atlas
  // ----------------------------------
  std::ifstream file(fontFilepath);
  if (!file.good()) {
      std::cerr << "ERROR: Font file not found at " << fontFilepath << std::endl;
      return 1;
  }
  FT_UInt fontsize = 48;
  loadFont(fontsize);
  createTextureAtlas();

  // build and compile our text shader program
  // -----------------------------------------
  Shader textShader("text_shader.vert", "text_shader.frag");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  glGenVertexArrays(1, &textVAO);
  glGenBuffers(1, &textVBO);

  glBindVertexArray(textVAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, textVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  
  // Position and texture attribute
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  // Set up text shader and projection matrix
  // ----------------------------------------
  textShader.use();
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
  textShader.setMat4("projection", projection);
  ```
  
* Inside the render loop:
  ```cpp
  // Update timing info
  UpdateFPS();
  std::string fpsText = GetFPSText(fps, msPerFrame);

  // Render FPS text at the top-left corner
  float scale = 0.5f;
  float x = 0.0f; // Position on the screen
  float y = SCR_HEIGHT - 35.0f * scale * fontsize/48; // Invert Y-axis since OpenGL origin is bottom-left. 
  glm::vec3 color(1.0f, 1.0f, 1.0f); // White text

  // Step 1: Render the scene to the MSAA FBO
  // ----------------------------------------
  glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
  glEnable(GL_DEPTH_TEST); // Needed for 3D rendering
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  RenderCrate(ourShader);
  
  // Step 2: Resolve MSAA FBO to standard non-MSAA FBO
  // -------------------------------------------------
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
  glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                    0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                            GL_COLOR_BUFFER_BIT,           // buffer mask
                                     GL_LINEAR);           // scale filter
  
  RenderText(textShader, fpsText, x, y, scale, color);
  RenderAtlas(atlasShader, textureAtlasID);

  // Step 3: Render the scene for on-screen rendering using Blitting: https://stackoverflow.com/a/31487085
  // -----------------------------------------------------------------------------------------------------
  // Bind the target FBO to read
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
  
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                    0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                            GL_COLOR_BUFFER_BIT,           // buffer mask
                                    GL_NEAREST);           // scale filter
  
  // Step 4: Read pixels from the resolved FBO for off-screen encoding (without PBOs)
  // --------------------------------------------------------------------------------
  glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame);

  // Flip the frame vertically
  flipFrameVertically(frame);
  
  sendFrameBufferToFFmpeg(frame, 1);
  ```
* Rendering text using a TrueType font (loaded with FreeType) rasterized onto a bitmap font texture atlas (V-Sync ON, explained further down).

  https://github.com/user-attachments/assets/2159f8b1-9488-44c0-9c7a-274c98ef78f3

* Along with rendering text, I also wanted to render the texture atlas on-screen, just so that I could see all the glyphs, which means creating another shader program and a function to activate the shader program, bind the VAO and VBO for a fullscreen quad, and render the texture of the texture atlas.
  * Vertex shader for the texture atlas shader program.
    ```cpp
    #version 430 core
    layout(location = 0) in vec2 aPos;          // Vertex position (2D quad)
    layout(location = 1) in vec2 aTexCoord;     // Texture coordinates
    
    out vec2 TexCoord;                          // Pass texture coordinates to fragment shader
    
    uniform mat4 projection;                    // Projection matrix to transform the text positions
    
    void main()
    {
        gl_Position = vec4(aPos.xy, 0.0, 1.0);  // Set the position of each vertex
        TexCoord = aTexCoord;                   // Pass texture coordinates to fragment shader
    }
    ```
  * Fragment shader for the texture atlas shader program.
    ```cpp
    #version 430 core
    in vec2 TexCoord;    // Texture coordinates from vertex shader
    out vec4 FragColor;  // Final output color
    
    uniform sampler2D screenTexture;    // The texture to sample from
    uniform vec3 textColor;             // The color of the text (usually white or any desired color)
    
    void main()
    {
        float alpha = texture(screenTexture, TexCoord).r;   // Sample the texture at given coordinates
        // If the glyph has an alpha value (not transparent), render it
        if (alpha < 0.1) {
            discard; // Avoid rendering transparent parts of the glyph
        }
        // Apply the text color to the glyph
        FragColor = vec4(textColor, alpha); // Set the text color, using alpha for transparency
    }
    ```
  * The ``RenderAtlas(...)`` function.
    ```cpp
    void RenderAtlas(Shader &atlasShader, GLuint &atlasTexture) {
        atlasShader.use();
        // Enable 2D rendering
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // bind VAO
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    
        // Bind the texture (the texture atlas in this case)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlasTexture); // atlasTextureId is the texture containing the atlas
        atlasShader.setInt("screenTexture", 0);
        glm::vec3 color(1.0f, 1.0f, 1.0f); // White text
        atlasShader.setVec3("textColor", color);
        // Draw the quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
    
        // Cleanup
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    ```
* Rendering text and the font texture atlas, I assume the pixelated quality is due to the fact the resolution is 800x600. Unfortunately, screen recording at a resolution any higher on my laptop with Intel HD Graphics 520 is not as smooth. Maybe there are some optimizations I can do to the code to improve performance.
  
  https://github.com/user-attachments/assets/9727acd9-bd7e-475d-ae30-800a8bf87f17

* [Here](https://youtu.be/qcMuyHzhvpI?si=WnrzZSOz15sXxyao&t=343) is a nice video[^61] which talks about all the different methods of text rendering, from bitmap fonts to vector/TrueType fonts rasterized onto a bitmap font texture atlas (which I am currently doing), and also talks about the disadvantages of the latter method as the "glyph textures are stored with a fixed font size, so a significant amount of scaling may introduce jagged edges"[^60] when rendering text in a 3D playable world where the text needs to scale and deform every frame to match the user's camera angle, referring to newer more advanced methods like SDF (signed distance field) fonts, best known from a paper released by _Valve_ in 2007,[^61] and MSDF (multi-channel distance field) fonts, developed by Viktor Chlumský in his Master's thesis.[^60] However, in the comments, SebastianLague mentions that rendering directly from curves may not be impractically slow with some optimizations, that he says is referenced in his video on text rendering.[^61]

  [![Watch the video](https://img.youtube.com/vi/qcMuyHzhvpI/maxresdefault.jpg)](https://youtu.be/qcMuyHzhvpI?si=WnrzZSOz15sXxyao&t=343)

  [^61]: VoxelRifts. "A Brief look at Text Rendering" _YouTube_, 27 Dec. 2024, [youtube.com/watch?v=qcMuyHzhvpI&t=343s](https://www.youtube.com/watch?v=qcMuyHzhvpI&t=343s).
  [^60]: Joey de Vries. "Text Rendering" _LearnOpenGL.com_, [learnopengl.com/In-Practice/Text-Rendering](https://learnopengl.com/In-Practice/Text-Rendering).
  [^61]: Chris Green. "Improved Alpha-Tested Magnification for Vector Textures and Special Effects" _Valve_, 5 Aug. 2007, [steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf](https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf).
  [^60]: Viktor Chlumský. "Shape Decomposition for Multi-channel
Distance Fields" _Czech Technical University in Prague_, 5 May 2015, [github.com/Chlumsky/msdfgen?tab=readme-ov-file](https://github.com/Chlumsky/msdfgen?tab=readme-ov-file).
  [^61]: SebastianLague. "Coding Adventure: Rendering Text" _YouTube_, 13 Apr. 2024, [youtube.com/watch?v=SO83KQuuZvg](https://www.youtube.com/watch?v=SO83KQuuZvg).

* Screen recording is slightly complicated when V-Sync is OFF as this means the FPS can change, so you have to choose carefully which frames to use. In order to get a smooth 60fps screen recording of a window with variable FPS you will need to take a frame every 60th of a second, which might mean discarding excess frames if the FPS>60, or reusing frames if the FPS<60. My current code just takes every next frame and feeds it to the FFmpeg pipe to encode a video and forces it to 60fps regardless of the FPS the frames were displayed at. This is why I was having the problem where high FPS led to a "slowed" screen recording, and low FPS led to a "sped-up" screen recording, as when there is high FPS my program is "slowing" the frames down to 60fps, and vice versa for low FPS. For now I am just going to keep V-Sync ON, so I don't have to deal with this problem, but maybe in the future I can tackle it. V-Sync being ON is just generally a good idea anyway as it eliminates screen-tearing. Would be also nice to implement "last x minutes" screen recording, which would probably involve saving the last N frames (x minutes) into a buffer and continuously updating the buffer, removing the oldest frames and feeding new frames in, and encoding the video with FFmpeg once the program ends.

* I was able to enable V-Sync with the help of this [Stack Overflow answer](https://stackoverflow.com/a/589232), using the ``WGL_EXT_swap_control`` extension function ``wglSwapIntervalEXT()``. Unfortunately, you will need to include the ``wglext.h`` and ``Windows.h`` header files, thus making the program tied to Windows, and as an insightful comment on that answer mentions, "That is not an extension to OpenGL, but rather to WGL (the Microsoft Windows window system API for OpenGL). Buffer swapping is by its very nature a window system specific operation. As far as GL is concerned it just draws to a front/back left/right buffer or some arbitrary FBO. The window system is the only thing with enough knowledge of the underlying host system to synchronize the presentation of drawn buffers to some event (in this case the monitor's vertical retrace)".[^61] You can use ``wglGetSwapIntervalEXT()`` to get the current swap interval,[^60] and change it to V-Sync ON using ``wglSwapIntervalEXT(1)``, V-Sync OFF using ``wglSwapIntervalEXT(0)``, and Adaptive V-Sync using ``wglSwapIntervalEXT(-1)``, "adaptive vsync enables v-blank synchronisation when the frame rate is higher than the sync rate, but disables synchronisation when the frame rate drops below the sync rate".[^60]
* You will need to copy the ``wglext.h`` header file, which should be located in the ``C:\msys64\mingw64\include\GL`` folder, into your project's ``include`` folder.
```bash
  YourProject
  └── include
      ├── wglext.h
      ├── ...
```
* You will also need to link to the library by adding ``"-lopengl32",`` to your compiler flags in your ``tasks.json`` file.

[^60]: eugensk. "OpenGL rendering from FBO to screen" _Stack Overflow_, 26 Feb. 2009, [stackoverflow.com/a/589232](https://stackoverflow.com/a/589232).
[^61]: Andon M. Coleman. "how to enable vertical sync in opengl?" _Stack Overflow_, 6 June 2014, [stackoverflow.com/questions/589064/how-to-enable-vertical-sync-in-opengl#comment37124053_589232](https://stackoverflow.com/questions/589064/how-to-enable-vertical-sync-in-opengl#comment37124053_589232).
[^60]: Alfonse, et al. "Swap Interval" _Khronos_, 17 June 2017, [khronos.org/opengl/wiki/Swap_Interval](https://www.khronos.org/opengl/wiki/Swap_Interval).

* I moved the ``myprogram.exe`` file to the ``src`` folder so now it is runnable (I should have done this a long time ago...). To ensure the ``main.cpp`` file saves the ``.exe`` file in the correct location you have to change the compiler output file flag to: ``-o "${workspaceFolder}/src/myprogram.exe"`` in your ``tasks.json`` file. If I wanted to give the project to someone else, all I would have to do is copy the project folder to them and they would just need to click ``myprogram.exe`` to run the program without needing MinGW-w64 (GCC) to compile the ``main.cpp`` code.
  ```bash
  YourProject
  └── src
      ├── myprogram.exe
      ├── ...
  ```
* I made the program rendering independent of the framerate by calculating the time between consecutive frames, ``timeDiff``, and multiplying this with every movement update calculation.[^61]
  [^61]: TravisG. "Framerate independence" _Game Development Stack Exchange_, 11 June 2011, [gamedev.stackexchange.com/a/13486](https://gamedev.stackexchange.com/a/13486).
  * Inside the render loop:
    ```cpp
    crntTime = glfwGetTime();
    timeDiff = crntTime - prevTime;
    prevTime = crntTime;
    ```
  * Function that processes all inputs:
    ```cpp
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window)
    {
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
    ```
* From the function above you can see that I also implemented a fullscreen toggle with the ``F11`` key and exiting with the ``ESC`` key.
* You'll notice above I did ``if`` statements without curly brackets/braces, this isn't recommended,[^60] but for simple one-line code it makes it look cleaner, just be careful.

  [^60]: clee, doynax. "Is it a bad practice to use an if-statement without curly braces?" _Stack Overflow_, 23 Jan. 2010, [stackoverflow.com/questions/2125066/is-it-a-bad-practice-to-use-an-if-statement-without-curly-braces](https://stackoverflow.com/questions/2125066/is-it-a-bad-practice-to-use-an-if-statement-without-curly-braces).

* I added another flag to the FFmpegCommand to control the encoded video quality, lowering the CRF and using a slower encoding preset will give better video quality but may make the program laggy.[^61]
  ```cpp
  "-crf 17 -preset slow "     // Constant rate factor (18=visually lossless, 23=default)
  ```
[^61]: llogan, et al. "H.264 Video Encoding Guide" _FFmpeg_, 12 Dec. 2024, [trac.ffmpeg.org/wiki/Encode/H.264](https://trac.ffmpeg.org/wiki/Encode/H.264).


<!-- FONT LOADER AND TEXTURE ATLAS -->
<!-- QUAD TEXT SHADERS (TALK ABOUT OPENGL OLD vs NEW (FIXED FUNCTION vs SHADER BASED) -->
<!-- FINALLY SHOW RESULTS WITH TEXTURES -->

## Progress update 7 - Transformations - 29/12/24
* Switched from regenerating the vectors array to using a ``mat4`` uniform variable to store the resultant translation (and rotation) matrix transformations.
* It's better to just follow from the LearnOpenGL [tutorial](https://learnopengl.com/Getting-started/Transformations), but I will outline the steps here.
* You will need the GLM (Open**GL M**athematics) headers, which will allow you to do vector and matrix math. You can download them from their [website](https://glm.g-truc.net/0.9.8/index.html).
* Once downloaded, move the ``glm`` folder into your project's ``include`` folder like so:
  ```bash
  YourProject
  └── include
      └── glm
          ├── detail
          │   └── ...
          ├── ext
          │   └── ...
          ├── gtc
          │   └── ...
          ├── gtx
          │   └── ...
          ├── simd
          │   └── ...
          ├── common.hpp
          ├── exponential.hpp
          ├── ...
  ```
* And include the headers at the top of ``main.cpp`` like so:
  ```cpp
  #include <glm/glm.hpp>
  #include <glm/gtc/matrix_transform.hpp>
  #include <glm/gtc/type_ptr.hpp>
  ```
* At initialization do the same steps, building and compiling the shader program, initializing the vertices (to be created once), vertex attributes, and the VAO, VBO, and EBO.
  ```cpp
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

  // unsigned int VBO, VAO, EBO;
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
  ```

* Inside the ``RenderCrate()`` function.
  ```cpp
  void RenderCrate(Shader &ourShader) {
      // set the texture mix value in the shader (this needs to be in the render loop)
      ourShader.use();
      ourShader.setFloat("mixValue", mixValue);
  
      // bind Texture
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, crateTexture);
      ourShader.setInt("texture1", 0);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, awesomeTexture);
      ourShader.setInt("texture2", 1);
  
      glm::mat4 trans = glm::mat4(1.0f);
      trans = glm::translate(trans, glm::vec3(xOffset, yOffset, 0.0f));
      // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
      ourShader.setMat4("transform", trans);
  
      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
      // render the crate
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  
      // Cleanup
      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  ```
* Updated scene vertex shader.
  ```cpp
  #version 430 core
  layout (location = 0) in vec3 aPos;
  layout (location = 1) in vec3 aColor;
  layout (location = 2) in vec2 aTexCoord;
  
  out vec3 ourColor;
  out vec2 TexCoord;
  
  uniform mat4 transform;
  
  void main()
  {
  	gl_Position = transform * vec4(aPos, 1.0);
  	ourColor = aColor;
  	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
  }
  ```
* Updated scene fragment shader.
  ```cpp
  #version 430 core
  out vec4 FragColor;
  
  in vec3 ourColor;
  in vec2 TexCoord;
  uniform float mixValue;
  
  // texture sampler
  uniform sampler2D texture1;	// crate
  uniform sampler2D texture2;	// awesome face
  
  void main()
  {
  	FragColor = mix(texture(texture1, vec2(TexCoord.x, TexCoord.y)), texture(texture2, vec2(TexCoord.x, TexCoord.y)), mixValue) * vec4(ourColor, 1.0); 
  	// FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Output solid red to test shader
  }
  ```
* I also switched from using ``malloc()`` and ``free()`` to using ``new`` and ``delete``. 
  ```cpp
  unsigned char* frame = NULL;
  ```
  ```cpp
  // Frame buffer to hold the raw frame data (RGB)
  // std::vector<unsigned char> frame(SCR_WIDTH * SCR_HEIGHT * 3);
  // std::array<unsigned char, SCR_WIDTH * SCR_HEIGHT * 3> frame;
  // Why use unsigned char? https://stackoverflow.com/a/13642984
  // frame = (unsigned char*) malloc(SCR_WIDTH * SCR_HEIGHT * 3 * sizeof(unsigned char));
  frame = new unsigned char[SCR_WIDTH * SCR_HEIGHT * 3];// - 2415];
  ```
  ```cpp
  // This deletes all items in the array
  delete[] frame;
  ```
* For some reason ``new`` seems to allocate more memory than expected, I need to store 800x600 RGB (0-255) pixel data, but I can get away with subtracting 2415 before getting a heap buffer overflow causing a segfault. When initializing a pointer, people recommended to set it to ``NULL``.[^60]
  [^60]: Dmitry. "Proper Way To Initialize Unsigned Char*" _Stack Overflow_, 2 Feb. 2011, [stackoverflow.com/a/4876907](https://stackoverflow.com/a/4876907).
* The final exercise for Transformations asks to render two crates using only transformations and two draw calls (``glDrawElements``). This was quite simple to do as I already created the function ``RenderCrate`` which did all the tedious stuff and all I'd need to do is just call this function again. I added another input for the function so it could receive a ``vec3`` translation.
* Because one of the textures of the object has transparency I thought it would be cool to see the other object behind the front object. Transparency/Blending is an "Advanced OpenGL" topic, but again it doesn't hurt to explore it early.
* People have said you should use these two functions before rendering transparent textures, however, at least for me commenting them out doesn't make a difference. Maybe it makes a difference when dealing with multiple textures.
  ```cpp
  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ```
* I was a little stuck until I realized I needed to change the z-value of the texture's position to make it appear in front or behind.
* What seemed to be the most important thing was rendering the stuff that's furthest away from the "camera" first, and the closest stuff last. I'm not entirely sure where the "camera" is on the z-axis, but rendering the textures in order of increasing z-values produces the desired result, as if an increasing z-value implies it is further away from the "camera". I thought the "camera" would be on the z=0 plane so negative z-value textures wouldn't appear but that isn't the case. I don't actually have a "camera" set up so most likely it is just casting all textures onto the screen in order of increasing z-value.
  * Rendering the controllable crate last and giving it a lower z-value (``0.0f``) than the static crate (``0.5f``) makes it appear in front of the static crate and transparency/blending is successful.
    ```cpp
    // static crate
    glm::vec3 translatenew = glm::vec3(-0.5f, 0.5f, 0.5f);
    RenderCrate(ourShader, translatenew);
  
    // controllable crate
    glm::vec3 translate = glm::vec3(xOffset, yOffset, 0.0f);
    RenderCrate(ourShader, translate);
    ```
    https://github.com/user-attachments/assets/5ab8c9c1-95c2-4017-a4bb-831ae98bfe29
    
  * Rendering the controllable crate last but giving it a higher z-value (``0.5f``) than the static crate (``0.0f``) makes it appear behind the static crate but transparency/blending fails.
    ```cpp
    // static crate
    glm::vec3 translatenew = glm::vec3(-0.5f, 0.5f, 0.0f);
    RenderCrate(ourShader, translatenew);

    // controllable crate
    glm::vec3 translate = glm::vec3(xOffset, yOffset, 0.5f);
    RenderCrate(ourShader, translate);
    ```
    https://github.com/user-attachments/assets/0210f5d8-7e40-4dc8-b2a6-10c13d1ba93e
    
  * Rendering the controllable crate first and giving it a higher z-value (``0.5f``) than the static crate (``0.0f``) makes it appear behind the static crate and transparency/blending is successful.
    ```cpp
    // controllable crate
    glm::vec3 translate = glm::vec3(xOffset, yOffset, 0.5f);
    RenderCrate(ourShader, translate);

    // static crate
    glm::vec3 translatenew = glm::vec3(-0.5f, 0.5f, 0.0f);
    RenderCrate(ourShader, translatenew);
    ```
    https://github.com/user-attachments/assets/71cff1c4-886d-415f-a5cc-43936775df04
    
  * Rendering the controllable crate first but giving it a lower z-value (``0.0f``) than the static crate (``0.5f``) makes it appear in front of the static crate but transparency/blending fails.
    ```cpp
    // controllable crate
    glm::vec3 translate = glm::vec3(xOffset, yOffset, 0.0f);
    RenderCrate(ourShader, translate);

    // static crate
    glm::vec3 translatenew = glm::vec3(-0.5f, 0.5f, 0.5f);
    RenderCrate(ourShader, translatenew);
    ```
    https://github.com/user-attachments/assets/00f4b6f8-61fb-4a1c-a1ff-72e4adf5cd7d
* [This website](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-10-transparency/) has a nice tutorial on Transparency in OpenGL and how to render multiple transparent triangles. The steps are essentially to draw everything that is opaque first so the depth buffer can discard hidden transparent triangles, sort the transparent triangles from furthest to closest, and render them in that order. They mention that needing to switch textures, or even worse, switching shader programs from triangle to triangle is very bad for performance,[^61] that's why games like _Minecraft_ use a texture atlas.
* Here is a nice video that covers Transparency/Blending in their OpenGL _Minecraft_ clone:[^60]
  [![Watch the video](https://img.youtube.com/vi/_rPRVk75Y6Q/maxresdefault.jpg)](https://www.youtube.com/watch?v=_rPRVk75Y6Q&t=34s)
* Here's another good video on Transparency & Blending:[^61]
  [![Watch the video](https://img.youtube.com/vi/crOfyWiWxmc/maxresdefault.jpg)](https://www.youtube.com/watch?v=crOfyWiWxmc&t=190s)
  

  [^61]: GraphicsTut. "Tutorial 10 : Transparency" _opengl-tutorial.org_, [opengl-tutorial.org/intermediate-tutorials/tutorial-10-transparency/](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-10-transparency/).
  [^60]: Low Level Game Dev. "How the rendering pipeline of a Minecraft-like game looks like? OpenGL C++" _YouTube_, 4 Sep. 2024, [youtube.com/watch?v=_rPRVk75Y6Q](https://www.youtube.com/watch?v=_rPRVk75Y6Q).
  [^61]: Victor Gordan. "OpenGL Tutorial 17 - Transparency & Blending" _YouTube_, 28 May 2021, [youtube.com/watch?v=crOfyWiWxmc](https://www.youtube.com/watch?v=crOfyWiWxmc).

## Progress update 8 - Coordinate Systems and Camera - 19/01/25

* For some reason the program would display a white window for a brief second and immediately close with no error message, this happened when I increased the dimensions of the window. It turned out the issue was due to a segfault,[^60] I didn't allocate enough memory for the FFmpeg frame buffer, I was experimenting with it earlier as mentioned above noticing that I could allocate less than the number of pixels and it would work just fine, however just to be safe I put it back to normal and it works fine again.

[^60]: swalog. "OpenGL GLFW window closes as soon as it opens" _Stack Overflow_, 26 Aug. 2014, [stackoverflow.com/a/25499922](https://stackoverflow.com/a/25499922).

* I can now render in 3D using transform, model, view, and projection matrices. The diagram below shows the pipeline. The reason we break it into these steps is that making changes can be easier in different parts of the process. I switch from orthographic to perspective projection for 3D rendering.

  ![coordinate_systems](https://github.com/user-attachments/assets/8200e64e-22af-42fd-be4e-5fe1247f3698)


* For the crate which is now a 3D cuboid, we will need 36 vertices (6 faces * 2 triangles * 3 vertices per triangle). At first I thought this was inefficient as a cuboid only has 8 vertices, however, in order for each face to have its own unique texture, each of the 3 vertices for the two triangles for every face will need to be stored, along with their texture coords. We could use EBOs to bring the number of vertices down to 8, but then different faces will be sharing vertices and so also sharing texture coords, so the faces can't all be assigned unique textures.

  ![ebo2](https://github.com/user-attachments/assets/ba763f3f-19d6-43ee-a823-d5d74ef54673)
  For context, there was very little performance difference with and without using an EBO. Others have explained that the main benefit of EBOs comes when dealing with meshes of models that contain hundreds of vertices, and in those situations faces sharing vertices (and thus texture coords) with neighboring faces is fine as they are so close to each other that their texture coords will be approximately the same.


  ```cpp
  #if RENDER_3D==1
  // 3D cube without EBO (6 faces * 2 triangles * 3 vertices each = 36 vertices) (each face has its own unique texture coords)
  #if RENDER_EBO==0
  float vertices[] = {
      // positions          // texture coords
      // -Z face
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // (-,-,-) 0
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // (+,-,-) 1
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   // (-,+,-) 3
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // (-,-,-) 0

      // +Z face
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // (+,-,+) 5
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // (+,+,+) 6
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // (+,+,+) 6
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   // (-,+,+) 7
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4

      // -X face
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (-,+,+) 7
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (-,+,-) 3
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (-,-,-) 0
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (-,-,-) 0
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (-,+,+) 7

      // +X face
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (+,+,+) 6
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (+,-,-) 1
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (+,-,-) 1
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (+,-,+) 5
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (+,+,+) 6

      // -Y face
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (-,-,-) 0
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   // (+,-,-) 1
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // (+,-,+) 5
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // (+,-,+) 5
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   // (-,-,-) 0

      // +Y face
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   // (-,+,-) 3
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (+,+,+) 6
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   // (+,+,+) 6
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   // (-,+,+) 7
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f    // (-,+,-) 3
  };
  #endif  /* RENDER_EBO==0 */
  // 3D cube with EBO (8 vertices. Each vertex is shared by 3 faces. One vertex's texture coords for one face are being shared for the two other neighboring faces, rather than them having their own unique texture)
  #if RENDER_EBO==1
  float vertices[] = {
      // positions          // texture coords
      // -Z face
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // (-,-,-) 0
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // (+,-,-) 1
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // (+,+,-) 2
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   // (-,+,-) 3

      // +Z face
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   // (-,-,+) 4
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // (+,-,+) 5
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // (+,+,+) 6
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   // (-,+,+) 7
  };
  unsigned int indices[] = {  
      // -Z face
      0, 1, 2,
      2, 3, 0,
      
      // +Z face
      4, 5, 6,
      6, 7, 4,

      // -X face
      7, 3, 0,
      0, 4, 7,

      // +X face
      6, 2, 1,
      1, 5, 6,

      // -Y face
      0, 1, 5,
      5, 4, 0,

      // +Y face
      3, 2, 6,
      6, 7, 3
  };
  #endif  /* RENDER_EBO==1 */
  #endif  /* RENDER_3D==1 */
  ```
  

  
* I also implemented a Camera header which _LearnOpenGL.com_ provides, containing a camera class that allows us to control the camera's position, yaw, and pitch. For some reason, I can't use my laptop's touchpad and the keyboard at the same time to move the camera in OpenGL, however, when using a separate mouse, I can use the keyboard simultaneously. 

* The _LearnOpenGL.com_ Camera chapter uses a weird if statement in the mouse callback function to check if this is the first use of the callback function and sets the ``xpos`` and ``ypos`` of the cursor to the centre of the screen, the reason being to avoid the whiplash cursor jump effect you would otherwise get, as the cursor is not initialized to the centre of the screen. A much cleaner alternative as mentioned by someone in the comments is to just use the GLFW function ``glfwSetCursorPos(window, lastX,lastY);`` before the while/render loop,[^61] where ``lastX`` and ``lastY`` are set to ``SCR_WIDTH / 2.0f`` and ``SCR_HEIGHT / 2.0f`` respectively, and remove the if statement in the mouse callback function. Another important point is to make sure ``YAW`` is initialized to ``-90.0f``.

[^61]: Andy. "Camera" _LearnOpenGL.com_, 8 June 2017, [disq.us/p/1jdygzu](http://disq.us/p/1jdygzu).

* I also clamped ``YAW`` to between ``0.0f`` and ``360.0f`` using the ``fmodf`` function and using ``Yaw + 360.0f`` rather than just ``Yaw``.
 
   ```cpp
   Yaw = fmodf(Yaw + 360.0f, 360.0f);
   ```

* Another comment suggests using quaternions instead of matrices to control the camera in 3D. They linked to their [article](https://chanhaeng.blogspot.com/2018/09/quaternion-camera-implementation.html)[^60] which includes the code and a nice [article](https://graphics.stanford.edu/courses/cs348a-17-winter/Papers/quaternion.pdf) from Stanford on 'Quaternions and Rotations'. The main benefits of using quaternions over matrices are that they are more compact (4 numbers instead of 9 with 3x3 rotation matrices), efficient (16 multiplications and 12 additions instead of 27 multiplications and 18 additions), numerically stable (avoids the gimbal lock problem[^61]), and the geometric meaning is apparently clearer too.[^60] Although some mention that quaternions are better when chaining multiple rotations but not better when actually computing the rotating vectors,[^61] requiring ~30 operations rather than 15 operations like with 3D rotation matrix-vector multiplication.[^60] So "multiplication of matrices is ~2 times slower than quaternions. [But] matrix-vector multiplication is ~2 times faster [than quaternions]."[^61]

[^60]: Chan Haeng Lee. "FPS Quaternion Camera Implementation (쿼터니언 카메라 구현)" _chanhaeng.blogspot.com_, 24 Sep. 2018, [chanhaeng.blogspot.com/2018/09/quaternion-camera-implementation.html](https://chanhaeng.blogspot.com/2018/09/quaternion-camera-implementation.html).
[^61]: Henry Cohn. "The gimbal lock shows up in my quaternions" _Math Overflow_, 3 May 2012, [mathoverflow.net/a/95908](https://mathoverflow.net/a/95908).
[^60]: Yan-Bin Jia. "Quaternions and Rotations Com S 477/577 Notes" _Stanford_, 10 Sep. 2013, [graphics.stanford.edu/courses/cs348a-17-winter/Papers/quaternion.pdf](https://graphics.stanford.edu/courses/cs348a-17-winter/Papers/quaternion.pdf).
[^61]: Muphrid. "why is representing rotations through quaternions more compact and quicker than using matrices??" _Math Stack Exchange_, 9 July 2015, [math.stackexchange.com/a/1355206](https://math.stackexchange.com/a/1355206).
[^60]: "Quaternions and spatial rotation - Performance comparisons" _Wikipedia_, 10 Jan. 2025, [en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Performance_comparisons](https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Performance_comparisons).
[^61]: Kevin Kostlan. "Why are quaternions used for rotations?" _Stack Overflow_, 26 Apr. 2013, [stackoverflow.com/a/16246782](https://stackoverflow.com/a/16246782).

* [This](https://lukearcamo.github.io/articles/coding/glfw-prevent-freezing-on-window-update) (nicely coded) article explains a way to stop OpenGL from pausing when resizing/moving the window, involving multithreading.[^62]

[^62]: Luke Arcamo. "GLFW: Prevent freezing on window update" _lukearcamo.github.io_, 22 July 2021, [lukearcamo.github.io/articles/coding/glfw-prevent-freezing-on-window-update](https://lukearcamo.github.io/articles/coding/glfw-prevent-freezing-on-window-update).

* I only just now learned about #include/header guards, which are used in headers to prevent creating duplicate copies of the header when unintentionally including the header more than once (this can happen sneakily when you include headers that include other headers you may already have included in your main.cpp file). This can be done two ways, using ``#ifndef``; ``#define``; and ``#endif``, or ``#pragma once``.[^63][^64] You will notice most header files use one or the other, all C++ standard library files have them, making it safe to include libraries in multiple files without the worry of duplicate code being generated.[^65]

  [^63]: Brian R. Bondy. "#pragma once vs include guards?" _Stack Overflow_, 17 July 2009, [stackoverflow.com/a/1143958/7875204](https://stackoverflow.com/a/1143958/7875204).
  [^64]: Alex Kremer. "What does #pragma once mean in C?" _Stack Overflow_, 25 Apr. 2011, [stackoverflow.com/a/5777009](https://stackoverflow.com/a/5777009).
  [^65]: eerorika, Kon. "Including a library in both the header file and the cpp file" _Stack Overflow_, 23 Feb. 2019, [stackoverflow.com/questions/54837486/including-a-library-in-both-the-header-file-and-the-cpp-file](https://stackoverflow.com/questions/54837486/including-a-library-in-both-the-header-file-and-the-cpp-file).

  ```cpp
  // top of my_header.h file
  #ifndef MY_HEADER_H
  #define MY_HEADER_H

  // ...

  #endif /* MY_HEADER_H */
  // end of file
  ```

  ```cpp
  // top of my_header.h file
  #pragma once

  // ...

  // end of file
  ```

* You can use headers to separate code into multiple files, the header file allows you to access functions and variables from other cpp files.[^66] The main benefit of separating code into multiple files is it makes the project cleaner and more readable, along with other benefits.[^67]

    [^66]: Puppy. "Using multiple .cpp files in c++ program?" _Stack Overflow_, 9 Aug. 2011, [stackoverflow.com/a/6995599](https://stackoverflow.com/a/6995599).
    [^67]: Dietrich Epp. "Why is it recommended to use multiple files when making a C++ project?" _Stack Overflow_, 20 Feb. 2023, [stackoverflow.com/a/75505133](https://stackoverflow.com/a/75505133).

* I added ``#define`` and ``#if``, ``#endif`` blocks to my code to allow for different blocks of code to be built at compile-time. This differs from using ``if`` statements as the code will still be built but evaluated at runtime.[^68] For example:

  ```cpp
  #define  RENDER_3D     1
  #define  RENDER_EBO    0

  int main() {
      #if RENDER_3D==0
      // this won't be compiled
      #endif /* RENDER_3D==0 */
  
      #if RENDER_3D==1
  
      #if RENDER_EBO==0
      // this will be compiled
      #endif /* RENDER_EBO==0 */
  
      #if RENDER_EBO==1
      // this won't be compiled
      #endif /* RENDER_EBO==1 */

      #endif /* RENDER_3D==1 */
      return 0;
  }
  ```
  
[^68]: T.J. Crowder. ""if " and " #if "; which one is better to use" _Stack Overflow_, 8 May 2013, [stackoverflow.com/a/16438775/7875204](https://stackoverflow.com/a/16438775/7875204).

* I made it so that in windowed mode, the Viewport always stays in the centre of the window with the original aspect ratio no matter how the window is resized. However, this only works nicely when screen recording is OFF as it changes the size of the viewport.

  ![opengl4](https://github.com/user-attachments/assets/85b4b0eb-ceff-4824-b40d-cffad1a3937e)


  ```cpp
  // scale viewport to always be inside window with correct aspect ratio
  // -------------------------------------------------------------------
  if ((float)height/(float)width > (float)SCR_HEIGHT/(float)SCR_WIDTH)
      glViewport(0, 0.5*((float)height - (float)SCR_HEIGHT*(float)width/(float)SCR_WIDTH), width, width*(float)SCR_HEIGHT/(float)SCR_WIDTH);
  else
      glViewport(0.5*((float)width - (float)SCR_WIDTH*(float)height/(float)SCR_HEIGHT), 0, height*(float)SCR_WIDTH/(float)SCR_HEIGHT, height);
  ```

* The alternative option is setting the viewport size to be fixed at the initial size of ``SCR_WIDTH`` and ``SCR_HEIGHT`` which doesn't mess up screen recording. I spent a lot of time trying to get the viewport to be in the centre of the window, and I finally got it to work. It turns out you need to adjust the position you want the viewport to be displayed in the window via ``glBlitFramebuffer()``, not ``glViewport()``. 

  Global variables:
  ```cpp
  const unsigned int  SCR_WIDTH       = 800; //2560;  // 800;
  const unsigned int  SCR_HEIGHT      = 600;
  int lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY;
  ```
  
  Inside the framebuffer size callback function:
  ```cpp
  // keep viewport size fixed (recording doesn't get messed up)
  // ----------------------------------------------------------
  lowerLeftCornerOfViewportX = 0.5*(width-static_cast<int>(SCR_WIDTH));    // need to convert SCR_WIDTH/HEIGHT from unsigned int->int!
  lowerLeftCornerOfViewportY = 0.5*(height-static_cast<int>(SCR_HEIGHT));
  lowerLeftCornerOfViewportX = std::max(lowerLeftCornerOfViewportX, 0);
  lowerLeftCornerOfViewportY = std::max(lowerLeftCornerOfViewportY, 0);

  // for debugging
  // std::cout << "viewportX: " << lowerLeftCornerOfViewportX << " "
  //           << "viewportY: " << lowerLeftCornerOfViewportY << std::endl;
  
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  ```
  
  Inside the render loop:
  ```cpp
  // Step 1: Render the scene to the MSAA FBO
  // ----------------------------------------
  glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
  // Render scene
  
  // Step 2: Resolve MSAA FBO to standard non-MSAA FBO
  // -------------------------------------------------
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
  glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                    0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                            GL_COLOR_BUFFER_BIT,           // buffer mask
                                     GL_LINEAR);           // scale filter

  // Render text, texture atlas, etc.

  // Step 3: Render the scene for on-screen rendering using Blitting: https://stackoverflow.com/a/31487085
  // -----------------------------------------------------------------------------------------------------
  // Bind the target FBO to read
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                    lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, lowerLeftCornerOfViewportX+SCR_WIDTH, lowerLeftCornerOfViewportY+SCR_HEIGHT, // dst rect
                            GL_COLOR_BUFFER_BIT,           // buffer mask
                                    GL_NEAREST);           // scale filter

  // Step 4: Read pixels from the resolved FBO for off-screen encoding (without PBOs)
  // --------------------------------------------------------------------------------
  glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame);
  // ...
  ```
  
  You'll notice in the render loop that when blitting from the MSAA FBO (source) to the standard non-MSAA (resolved) FBO (destination) we keep the rectangle bounds (X0, Y0, X1, Y1) set to ``(0, 0, SCR_WIDTH, SCR_HEIGHT)``. But when blitting from the resolved FBO to the default framebuffer (on-screen), we change the destination rectangle's bounds to ``(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, lowerLeftCornerOfViewportX+SCR_WIDTH, lowerLeftCornerOfViewportY+SCR_HEIGHT)``.

  Also notice how in the framebuffer size callback function, we don't change glViewport's inputs (X0, Y0,	width, height), ``glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);``, the viewport will mess up if you set it to ``glViewport(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, SCR_WIDTH, SCR_HEIGHT);``, this took me so long to figure out.
  
  It is very important we set ``lowerLeftCornerOfViewportX = std::max(lowerLeftCornerOfViewportX, 0);`` and ``lowerLeftCornerOfViewportY = std::max(lowerLeftCornerOfViewportY, 0);``, otherwise if the window ``(width, height)`` is smaller than our fixed framebuffer size ``(SCR_WIDTH, SCR_HEIGHT)``, ``lowerLeftCornerOfViewportX = 0.5*(width-static_cast<int>(SCR_WIDTH));`` and ``lowerLeftCornerOfViewportY = 0.5*(height-static_cast<int>(SCR_HEIGHT));`` will be negative which will cause the viewport to glitch out.

  <!-- SHOW VIDEO/PIC OG GLITCH -->

  It is also important to typecast ``SCR_WIDTH`` and ``SCR_HEIGHT`` from ``unsigned int`` to ``int`` when calculating ``lowerLeftCornerOfViewportX`` and ``lowerLeftCornerOfViewportY``. This gave me a lot of headaches trying to debug until I added some print statements to check the values and realized they weren't correct.

  ViewportX (``lowerLeftCornerOfViewportX``) not producing the correct value:
  
  ![cmd4](https://github.com/user-attachments/assets/b8d156e9-e49b-4a14-b369-470a0b7d0595)

  ViewportX (``lowerLeftCornerOfViewportX``) producing the correct negative value after correctly typecasting ``SCR_WIDTH``. Now when resizing the window to be smaller than the framebuffer size, ``lowerLeftCornerOfViewportX`` and ``lowerLeftCornerOfViewportX`` will be negative, and will be correctly set to ``0`` when passed into the ``std::max()`` function.

  ![cmd3](https://github.com/user-attachments/assets/bca04e83-57b2-40ce-80fb-89b7918bb184)

  Here's the output with the black background outside the rendered viewport, but not centred:
  
  ![opengl-black-bars](https://github.com/user-attachments/assets/39201d7e-2063-427b-a645-51f7ab0701f0)
  
  And here's the final result with the viewport centred and a black background. The viewport crops when the window is smaller than the framebuffer size as intended. This ensures the framebuffer is a fixed size so screen recording is unaffected while allowing the window to be resized however the user wants without the viewport changing size:

  ![opengl9](https://github.com/user-attachments/assets/c7ed0b8a-4db5-4cbd-9662-fc64e18ab55a)

* Another option is setting the viewport to be equal to the size of the window, which leads to the viewport being stretched and scaled to fit the resized window.

  ```cpp
  // viewport stretches (recording gets messed up)
  // ---------------------------------------------
  glViewport(0, 0, width, height);
  ```

* [This blog post](https://diegomacario.github.io/2021/04/23/how-to-keep-the-aspect-ratio-of-an-opengl-window-constant.html) by Diego Macario (who has a really cool [2D physics simulator](https://diegomacario.github.io/about/) project) explains a way to make the viewport centred which seems similar to how I did it, however, when implementing their method, it didn't centre the viewport. Their method of getting the background outside the viewport to be black did work, which helps distinguish between the viewport and the window. The code to do so uses ``GL_SCISSOR_TEST`` and goes like this:

  ```cpp
  // Set the clear color to black
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  // Clear the current framebuffer
  // At this point the entire framebuffer is black
  glClear(GL_COLOR_BUFFER_BIT);
  // Set the clear color to the background color of choice
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // background color
  // Clear the area within the scissor box, that is, the viewport
  glEnable(GL_SCISSOR_TEST);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_SCISSOR_TEST);
  ```

  However, upon further testing, it seems you don't even need to add this code and it will give a black background anyway.

  [^69]: Diego Macario. "How to keep the aspect ratio of an OpenGL window constant when it's resized" _diegomacario.github.io_, 23 Apr. 2021, [diegomacario.github.io/2021/04/23/how-to-keep-the-aspect-ratio-of-an-opengl-window-constant.html](https://diegomacario.github.io/2021/04/23/how-to-keep-the-aspect-ratio-of-an-opengl-window-constant.html).

* Every time I switched from fullscreen mode back to windowed mode, the window seemed to lose its title bar. It turns out this was just due to me setting the window position to be ``(0, 0)``.[^70]

  ![opengl-no-title-bar](https://github.com/user-attachments/assets/331e459b-76d2-4c3b-890d-9cc3928b45f1)
  The solution is as simple as saving the last position and size of the window before switching to fullscreen,

  ```cpp
  glfwGetWindowPos(window, &window_xPos, &window_yPos);
  glfwGetWindowSize(window, &window_width, &window_height);
  ```

  then when the user switches back to windowed mode, setting the window back to the last saved position and size again.[^70]
  
  ```cpp
  glfwSetWindowMonitor(window,
                       nullptr,
                       window_xPos, window_yPos,
                       window_width, window_height,
                       0);
  ```

  The full solution:
  
  ```cpp
  // Fullscreen
  // ----------
  if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
  {
      bool last_pause_state = paused;
      float lastYaw = camera.Yaw;
      float lastPitch = camera.Pitch;
      if (last_pause_state==0)
      {
          // lock the mouse (stop YAW drift)
          glfwSetCursorPos(window, SCR_WIDTH/2.0f, SCR_HEIGHT/2.0f);  // set cursor in centre of screen to remove whiplash cursor jump
          // tell GLFW to uncapture our mouse
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
          paused = 1;
      }

      if (fullscreen)
      {
          glfwSetWindowMonitor(window,
                              nullptr,
                              window_xPos, window_yPos,
                              window_width, window_height,
                              0
          );
          fullscreen = 0;
      }
      else
      {
          glfwGetWindowPos(window, &window_xPos, &window_yPos);
          glfwGetWindowSize(window, &window_width, &window_height);
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
      if (last_pause_state==0)
      {
          // unlock the mouse
          glfwSetCursorPos(window, lastX, lastY);  // set cursor in centre of screen to remove whiplash cursor jump
          // tell GLFW to capture our mouse
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
          paused = 0;
          camera.Yaw = lastYaw;
          camera.Pitch = lastPitch;
      }
  }
  ```

[^70]: Shane McPhillips, immuv. "glfw window with no title bar" _Stack Overflow_, 23 Mar. 2022, 11 Jan 2022, [stackoverflow.com/questions/70582321/glfw-window-with-no-title-bar](https://stackoverflow.com/questions/70582321/glfw-window-with-no-title-bar).


<!-- ADD BIBLIOGRAPHY -->
<!-- ADD CODE SHOWING FBO, RBO, PBO, etc. -->
<!-- FINALLY SHOW RESULTS WITH TEXTURES -->
## License
GNU General Public License v3.0
