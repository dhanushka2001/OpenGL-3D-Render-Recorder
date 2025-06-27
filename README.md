*Learning OpenGL for High Performance Scientific Computing and Modelling on the GPU*

[![Build Status](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/actions/workflows/cmake-multi-platform.yml)
[![GitHub Release](https://img.shields.io/github/v/release/dhanushka2001/OpenGL-3D-Render-Recorder)](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/releases)
[![GitHub License](https://img.shields.io/github/license/dhanushka2001/OpenGL-3D-Render-Recorder)](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/blob/main/LICENSE)
[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/dhanushka2001/OpenGL-3D-Render-Recorder)](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/commits/main/)
[![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/dhanushka2001/OpenGL-3D-Render-Recorder/total)](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/releases)
[![GitHub Repo stars](https://img.shields.io/github/stars/dhanushka2001/OpenGL-3D-Render-Recorder)](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/stargazers)

<!--
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&&logo=opengl)
![FFmpeg](https://shields.io/badge/FFmpeg-%23171717.svg?logo=ffmpeg&style=for-the-badge&labelColor=171717&logoColor=5cb85c)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
-->

![C++ new](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)
[![OpenGL new](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=flat&logo=opengl)](https://github.com/glfw/glfw)
[![FFmpeg new](https://shields.io/badge/FFmpeg-%23171717.svg?logo=ffmpeg&style=flat&labelColor=171717&logoColor=5cb85c)](https://github.com/FFmpeg/FFmpeg)
[![Windows new](https://img.shields.io/badge/Windows-0078D6?style=flat&logo=windows&logoColor=white)](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/releases/tag/v1.2.0)
[![Arch Linux](https://img.shields.io/badge/Arch_Linux-1793D1?style=flat&logo=arch-linux&logoColor=white)](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/releases/tag/v1.2.0)
<!--
![Linux new](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
-->

# OpenGL-3D-Render-Recorder

https://github.com/user-attachments/assets/fc8763bd-dcda-4dbf-9c60-3e322aa1a2ce

Real-time 3D renderer in C++ with OpenGL (GLFW).
* [Phong lighting](https://en.m.wikipedia.org/wiki/Phong_reflection_model)
* Custom text tendering using [FreeType](https://freetype.org/index.html) to rasterize TrueType (.ttf) vector fonts onto a texture atlas
* GUI and real-time plots using [Dear ImGui](https://github.com/ocornut/imgui) and [ImPlot](https://github.com/epezent/implot)
* Screen recording using [FFmpeg](https://github.com/FFmpeg/FFmpeg), [x264](https://www.videolan.org/developers/x264.html) encoder, multithreading, and [asynchronous read-back with 2 PBOs](https://www.songho.ca/opengl/gl_pbo.html#pack). (Video heavily compressed to fit GitHub's 10MB filesize limit.)

## How to run

<details><summary>Beginner tutorial</summary>

* If you want to run the program quickly, just download and extract the latest [release](https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder/releases) ZIP which contains the precompiled executable. Or if you'd like to compile the source code yourself, follow the instructions below.
* Install [MSYS2](https://www.msys2.org/) (install it directly to ``C:\msys64``)
* Run the MSYS2 <ins>**MINGW64**</ins> shell and install the required packages (``git``, ``gcc``, ``cmake``, ``ninja``):

  ```console
  pacman -S --needed git mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
  ```
* Just to be safe, check that the packages are up to date, run this twice:

  ```console
  pacman -Syu
  ```

* Add this to your [System PATH](https://www.eukhost.com/kb/how-to-add-to-the-path-on-windows-10-and-windows-11/) (I highly recommend moving it up to the top of the list):

  ```console
  C:\msys64\mingw64\bin
  ```

* Now, continue with the rest of the steps inside any terminal (MINGW64, cmd, PowerShell, VSCode's terminal, etc.). If, for whatever reason, when you run ``cmake ..``, it builds for ``make`` instead of ``Ninja``, run this: ``cmake -G Ninja ..``

</details>

```console
git clone https://github.com/dhanushka2001/OpenGL-3D-Render-Recorder
```
```console
cd OpenGL-3D-Render-Recorder
mkdir build
cd build
cmake ..
```

Windows seems to default to building for ``ninja`` when running ``cmake ..``, which is good as ``ninja`` tries to compile files in parallel. However on Linux it seems to default to building for ``make``, which is slower. To specify building for ``ninja``, clear the ``/build/`` folder and run:

```console
cmake .. -G Ninja
```
Finally, to build the executable, just run:

```console
ninja
```

``ninja`` can sometimes give out-of-memory errors for machines with low RAM, like my laptop, in which case you should use:

```console
ninja -j1
```

which limits ``ninja`` to 1 job at a time.

The executable will be inside ``/build/bin/``, and screen recordings will be inside ``/build/output/``.

Ensure your version of GCC is up to date; see `lib_build_info.txt` for details about the GCC version and environment used to build the static libraries. Check your GCC version by running:

```console
g++ --version
```

If, when running ``ninja``, it gets stuck in an infinite loop "Rechecking globbed directories...", just press <kbd>Ctrl</kbd>+<kbd>C</kbd> to stop the loop, run:

```console
cmake --build . --target clear
```

or manually delete ``CMakeCache.txt`` in ``/build/``, and rerun ``ninja``.

## Introduction
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
  
* Using www.learnopengl.com as my main resource.

## Progress update 1 - Hello Triangle - 30/06/24

<details><summary> Progress update 1 - Hello Triangle - 30/06/24 </summary>
  
  * [This blog](https://medium.com/@vivekjha92/setup-opengl-with-vs-code-82852c653c43#:~:text=The%20Setup,clone%20it%20and%20get%20started.) helped a lot with setting up OpenGL in VSCode.
  * I understand how VBOs (vertex buffer objects), VAOs (vertex array objects), and EBOs (element buffer objects) work and why we need them.
  * The VAO essentially stores the pointers (memory addresses) for all the vertex attributes (position, colour, etc.) in a specific VBO, which stores the actual values of those vertex attributes.
  * An EBO fixes the issue of duplicate vertices when they overlap by storing only the unique vertices and an index to render them in the correct order.
  * Was able to render a static 3D RGB cube using 3 fragment shaders and 3 VBOs, 3 VAOs, and 3 EBOs.
  * I feel like 3 fragment shaders, 3 VBOs, 3 VAOs, and 3 EBOs are overkill just to render 3 different colours for 3 sets of 2 triangles. There's probably a way to have all 6 triangles in one VBO with their colour attributes also included.
    This will probably be explained in the next chapters.
      
    ![OpenGL_EBO](https://github.com/user-attachments/assets/80c63661-a410-4a59-b655-37b9d6629856)
  
    ![learnopengl](https://github.com/user-attachments/assets/a78223b7-9662-49d5-9329-c6b9733cc4b4)
  
    ![learnopengl2](https://github.com/user-attachments/assets/658d3a26-cea8-4b47-9e1c-a6d4d3d27650)
</details>


## Progress update 2 - Shaders - 20/07/24

<details><summary> Progress update 2 - Shaders - 20/07/24 </summary>
    
  * I understand the render pipeline for OpenGL and what all the shaders in the pipeline do.
  
    ![OpenGL_graphics-shader-pipeline](https://github.com/user-attachments/assets/9030f814-770a-4185-b193-948204d5b5fb)
  
  * Gave each vertex a colour attribute as well as a position attribute, so instead of having 3 VBOs, 3 VAOs, and 3 EBOs, we just have 1 of each and 2 vertex attributes. One for each object makes sense (I assume when we get to rendering 1000s of identical objects we will encounter "instancing").
  * Removed the vertex shader and fragment shader GLSL code from the main.cpp file and put them into separate ``shader.vert`` and ``shader.frag`` text files (I recommend installing a GLSL syntax highlighter extension).
  * Made a shader class and header file that can handle retrieving source code; compiling, linking, deleting, and activating shaders; defining utility functions; and handling errors.
  * I understand what uniform variables are (basically global variables that all shaders can access), you need to define them in the main.cpp file and give them to the shader (using utility uniform functions if you have a separate shader header file, or just by giving the uniform location with ``glUniform[](...)``).
  * Switched from OpenGL 3.3 to 4.3 so that I can specify the ``layout (location=...)`` for uniforms, for some reason in OpenGL 3.3 that didn't work. This is a really nice video that helped me understand the ``layout (location=...)`` specifier. At [21:11](https://youtu.be/yrFo1_Izlk0?si=YLplgGlBy1hqR0u2&t=1271) it talks about the specifier and how you can omit the specifier but if you don't then you can change the variable name in the next shader, and vertex attributes and uniforms are stored in different arrays so you can have seemingly two different variables stored in ``location=0`` but they are actually in different arrays.
    [![Watch the video](https://img.youtube.com/vi/yrFo1_Izlk0/maxresdefault.jpg)](https://www.youtube.com/watch?v=yrFo1_Izlk0)
  
  * This animation was done by storing a green colour uniform variable in the render loop that changes over time, this uniform variable can be accessed by the fragment shader to render the triangle with changing colour.
  
    https://github.com/user-attachments/assets/e2bdee00-6a7d-4f6b-a29b-513f5611c3d3
  
  * This rainbow triangle was done by storing a red, green, and blue colour value as a second vertex attribute for each of the 3 vertices. "Fragment interpolation" occurs, where a linear combination of the colours is used for all the fragments (can think of them as pixels) between the 3 vertices.
  
    ![rainbow-shader](https://github.com/user-attachments/assets/55060b30-221c-4074-a7fc-3fae7eac14cf)
  
  
  * This flipped offset gradient colour triangle was done by setting an offset float variable in the ``main.cpp`` file, then setting it as a uniform in the render loop using the utility uniform functions created in the shader header file. The weird colours were done by defining the ``FragColor`` to be the positions of the 3 vertices rather than the colour (Exercise 3), and fragment interpolation causes the gradient effect. (One thing to note is that if you input a negative float into one of the RGB channels it will be clamped to 0.0f (black)).
  
    ![rainbow-shader2](https://github.com/user-attachments/assets/0d31a4d3-1974-40c2-8a77-805ab5110d83)
</details>

## Progress update 3 - Textures - 22/08/24

<details><summary> Progress update 3 - Textures - 22/08/24 </summary>
  
  * I now understand how textures work, and I was able to render this rectangle with a crate PNG texture with an RGB overlay and the "Awesome face" PNG. I still need to do the exercises.
  
    ![textures](https://github.com/user-attachments/assets/7b44f9ef-dfd1-4c75-83a5-1d4279429f0c)
  
  * Alongside learning how to use textures, I wanted to do a mini-project rendering a rotating animated RGB triangle and learning how to render off-screen frames as images to memory rather than to a window on-screen.
  * The texture section introduced me to the [stb](https://github.com/nothings/stb) repo by Sean Barrett, in particular the ``stb_image.h`` header, a single header image loading library used to load an image into a texture; and the ``stb_image_write.h`` header, used for image writing from OpenGL to disk (PNG).
  * In order to make the triangle spin I used the standard 2D [rotation matrix](https://en.wikipedia.org/wiki/Rotation_matrix).
  
    ![opengl-rotation-matrix](https://github.com/user-attachments/assets/139fdad8-0540-44e4-ad01-f3da638340f9)
  
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
  
    ![HSV-vs-Sinebow](https://github.com/user-attachments/assets/a6292e62-082b-457b-980b-2303232562f0)
  
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
  
    ![opengl5 3](https://github.com/user-attachments/assets/14549a3d-faf1-4c0f-aed8-77dca57257f1)
  
  * Learning about FBOs naturally led me to learn about [RBOs (renderbuffer objects)](https://www.khronos.org/opengl/wiki/Renderbuffer_Object) which are specifically used by FBOs as a render target. Textures can alternatively be used if you want to re-use the pixels on-screen (e.g. a naive "security camera" in a game)[^20] or sample the pixels for post-processing,[^24] however since we just want to read-back the pixels and render off-screen, RBOs are the logical choice.
  * I have decided to keep the code with the PBO as even though it doesn't make any performative difference and just adds more lines of code, I may need them in the future. I ended up using 2 PBOs, while one has pixel data from the FBO written into it, the other is being mapped for reading. This has no performative benefit to just reading and writing to system memory from the FBO directly on every cycle since on every cycle I am still invoking ``glReadPixels`` which stalls the pipeline "because it has to safely map the data from the read buffer to the PBO and to a block of memory in main RAM", and I am mapping the other PBO which also stalls the pipeline until the pixel data has been converted to a png and saved to system memory.[^20]
  * I have tried to implement multiple PBOs to delay the writing to system memory step till every n<sup>th</sup> cycle, however, it doesn't seem to work properly. For some reason after every every n<sup>th</sup> cycle the rendered frames jump ahead, seemingly skipping multiple frames, is mapping the data not stalling the pipeline?
  * Implemented [off-screen MSAA](https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing) (multisample anti-aliasing) which is an advanced topic but I skipped ahead. Needed to use 2 FBOs now since MSAA requires one to be multisample storage and the other to be a normal FBO to downsample the result to a single-sample image using glBlitFramebuffer(), as we cannot directly use the result from MSAA FBO (see: https://www.songho.ca/opengl/gl_fbo.html#msaa). Also required creating a depthbuffer alongside the colorbuffer for the MSAA FBO.
  
    ![gl_fbo04](https://github.com/user-attachments/assets/05ef80e9-9876-4693-93b9-4d63282c269f)
  
  * The video below was rendered using the FFmpeg command below after running off-screen rendering for around 1 minute, converting 300 frames to a 10s video at 30fps. In the future, I'd like to make it so that the program feeds the frames into ffmpeg and continually builds the video at runtime rather than generating losslessly compressed frames which is costly for memory, especially for longer videos. For reference, the 300 frames total 35.3MB while the video rendered using those frames is just 856KB, ~40x less memory used.                                               
  
    ```cmd
    ffmpeg -framerate 30 -start_number 2 -i "frame%03d.png" -c:v libx264 -pix_fmt yuv420p out.mp4
    ```
    
    https://github.com/user-attachments/assets/66e9a3a9-d633-4e1d-adf6-1e6356896643
  
  * The triangle isn't cycling through the Sinebow properly so it isn't true RGB, I'm not sure why yet, however, I prefer the colour scheme it's cycling through so I just kept it.
  
  [^20]: KillianDS. "How to render offscreen on OpenGL?" _Stack Overflow_, 28 Aug. 2012, [stackoverflow.com/a/12159293](https://stackoverflow.com/a/12159293).
  [^21]: Ciro Santilli OurBigBook.com. "How to use GLUT/OpenGL to render to a file?" _Stack Overflow_, 14 Jan. 2013, [stackoverflow.com/a/14324292](https://stackoverflow.com/a/14324292).
  [^22]: Charlie Loyd. "On rainbows" _Env - basecase.org_, 25 Dec. 2011, [basecase.org/env/on-rainbows](https://basecase.org/env/on-rainbows).
  [^23]: Lencerf. "Save the OpenGL rendering to an image file" _Lencerf's Walk_, 20 Sep. 2019, [lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/](https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/).
  [^24]: Song Ho Ahn (안성호). "OpenGL Pixel Buffer Object (PBO)" _songho.ca_, [songho.ca/opengl/gl_pbo.html#pack](https://www.songho.ca/opengl/gl_pbo.html#pack).
</details>

## Progress update 4 - Off-screen recording - 18/10/24

<details><summary> Progress update 4 - Off-screen recording - 18/10/24 </summary>
  
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
</details>

## Progress update 5 - Simultaneous on-screen and off-screen rendering - 16/11/24

<details><summary> Progress update 5 - Simultaneous on-screen and off-screen rendering - 16/11/24 </summary>
  
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
  
  [^25]: genpfault. "OpenGL rendering from FBO to screen" _Stack Overflow_, 1 May 2012, [stackoverflow.com/a/10400048](https://stackoverflow.com/a/10400048).
  [^26]: Reto Koradi. "OpenGL - Is there an easier way to fill window with a texture, instead using VBO,etc?" _Stack Overflow_, 18 July 2015, [stackoverflow.com/a/31487085](https://stackoverflow.com/a/31487085).
  [^27]: Song Ho Ahn (안성호). "OpenGL Frame Buffer Object (FBO)" _songho.ca_, [songho.ca/opengl/gl_fbo.html#example](http://www.songho.ca/opengl/gl_fbo.html#example).
</details>

## Progress update 6 - Text rendering - 29/11/24

<details><summary> Progress update 6 - Text rendering - 29/11/24 </summary>
  
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
    ```glsl
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
    ```glsl
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
      ```glsl
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
      ```glsl
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
  
  * [Here](https://youtu.be/qcMuyHzhvpI?si=WnrzZSOz15sXxyao&t=343) is a nice video[^39] which talks about all the different methods of text rendering, from bitmap fonts to vector/TrueType fonts rasterized onto a bitmap font texture atlas (which I am currently doing), and also talks about the disadvantages of the latter method as the "glyph textures are stored with a fixed font size, so a significant amount of scaling may introduce jagged edges"[^40] when rendering text in a 3D playable world where the text needs to scale and deform every frame to match the user's camera angle, referring to newer more advanced methods like SDF (signed distance field) fonts, best known from a paper released by _Valve_ in 2007,[^41] and MSDF (multi-channel distance field) fonts, developed by Viktor Chlumský in his Master's thesis.[^42] However, in the comments, SebastianLague mentions that rendering directly from curves may not be impractically slow with some optimizations, that he says is referenced in his video on text rendering.[^43]
  
    [![Watch the video](https://img.youtube.com/vi/qcMuyHzhvpI/maxresdefault.jpg)](https://youtu.be/qcMuyHzhvpI?si=WnrzZSOz15sXxyao&t=343)
  
    [^39]: VoxelRifts. "A Brief look at Text Rendering" _YouTube_, 27 Dec. 2024, [youtube.com/watch?v=qcMuyHzhvpI&t=343s](https://www.youtube.com/watch?v=qcMuyHzhvpI&t=343s).
    [^40]: Joey de Vries. "Text Rendering" _LearnOpenGL.com_, [learnopengl.com/In-Practice/Text-Rendering](https://learnopengl.com/In-Practice/Text-Rendering).
    [^41]: Chris Green. "Improved Alpha-Tested Magnification for Vector Textures and Special Effects" _Valve_, 5 Aug. 2007, [steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf](https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf).
    [^42]: Viktor Chlumský. "Shape Decomposition for Multi-channel
  Distance Fields" _Czech Technical University in Prague_, 5 May 2015, [github.com/Chlumsky/msdfgen?tab=readme-ov-file](https://github.com/Chlumsky/msdfgen?tab=readme-ov-file).
    [^43]: SebastianLague. "Coding Adventure: Rendering Text" _YouTube_, 13 Apr. 2024, [youtube.com/watch?v=SO83KQuuZvg](https://www.youtube.com/watch?v=SO83KQuuZvg).
  
  * Screen recording is slightly complicated when V-Sync is OFF as this means the FPS can change, so you have to choose carefully which frames to use. In order to get a smooth 60fps screen recording of a window with variable FPS you will need to take a frame every 60th of a second, which might mean discarding excess frames if the FPS>60, or reusing frames if the FPS<60. My current code just takes every next frame and feeds it to the FFmpeg pipe to encode a video and forces it to 60fps regardless of the FPS the frames were displayed at. This is why I was having the problem where high FPS led to a "slowed" screen recording, and low FPS led to a "sped-up" screen recording, as when there is high FPS my program is "slowing" the frames down to 60fps, and vice versa for low FPS. For now I am just going to keep V-Sync ON, so I don't have to deal with this problem, but maybe in the future I can tackle it. V-Sync being ON is just generally a good idea anyway as it eliminates screen-tearing. Would be also nice to implement "last x minutes" screen recording, which would probably involve saving the last N frames (x minutes) into a buffer and continuously updating the buffer, removing the oldest frames and feeding new frames in, and encoding the video with FFmpeg once the program ends.
  
  * I was able to enable V-Sync with the help of this [Stack Overflow answer](https://stackoverflow.com/a/589232), using the ``WGL_EXT_swap_control`` extension function ``wglSwapIntervalEXT()``. Unfortunately, you will need to include the ``wglext.h`` and ``Windows.h`` header files, thus making the program tied to Windows, and as an insightful comment on that answer mentions, "That is not an extension to OpenGL, but rather to WGL (the Microsoft Windows window system API for OpenGL). Buffer swapping is by its very nature a window system specific operation. As far as GL is concerned it just draws to a front/back left/right buffer or some arbitrary FBO. The window system is the only thing with enough knowledge of the underlying host system to synchronize the presentation of drawn buffers to some event (in this case the monitor's vertical retrace)".[^44] You can use ``wglGetSwapIntervalEXT()`` to get the current swap interval,[^45] and change it to V-Sync ON using ``wglSwapIntervalEXT(1)``, V-Sync OFF using ``wglSwapIntervalEXT(0)``, and Adaptive V-Sync using ``wglSwapIntervalEXT(-1)``, "adaptive vsync enables v-blank synchronisation when the frame rate is higher than the sync rate, but disables synchronisation when the frame rate drops below the sync rate".[^46]
  * You will need to copy the ``wglext.h`` header file, which should be located in the ``C:\msys64\mingw64\include\GL`` folder, into your project's ``include`` folder.
  ```bash
    YourProject
    └── include
        ├── wglext.h
        ├── ...
  ```
  * You will also need to link to the library by adding ``"-lopengl32",`` to your compiler flags in your ``tasks.json`` file.
  
  [^44]: eugensk. "OpenGL rendering from FBO to screen" _Stack Overflow_, 26 Feb. 2009, [stackoverflow.com/a/589232](https://stackoverflow.com/a/589232).
  [^45]: Andon M. Coleman. "how to enable vertical sync in opengl?" _Stack Overflow_, 6 June 2014, [stackoverflow.com/questions/589064/how-to-enable-vertical-sync-in-opengl#comment37124053_589232](https://stackoverflow.com/questions/589064/how-to-enable-vertical-sync-in-opengl#comment37124053_589232).
  [^46]: Alfonse, et al. "Swap Interval" _Khronos_, 17 June 2017, [khronos.org/opengl/wiki/Swap_Interval](https://www.khronos.org/opengl/wiki/Swap_Interval).
  
  * I moved the ``myprogram.exe`` file to the ``src`` folder so now it is runnable (I should have done this a long time ago...). To ensure the ``main.cpp`` file saves the ``.exe`` file in the correct location you have to change the compiler output file flag to: ``-o "${workspaceFolder}/src/myprogram.exe"`` in your ``tasks.json`` file. If I wanted to give the project to someone else, all I would have to do is copy the project folder to them and they would just need to click ``myprogram.exe`` to run the program without needing MinGW-w64 (GCC) to compile the ``main.cpp`` code.
    ```bash
    YourProject
    └── src
        ├── myprogram.exe
        ├── ...
    ```
  * I made the program rendering independent of the framerate by calculating the time between consecutive frames, ``timeDiff``, and multiplying this with every movement update calculation.[^47]
    
    [^47]: TravisG. "Framerate independence" _Game Development Stack Exchange_, 11 June 2011, [gamedev.stackexchange.com/a/13486](https://gamedev.stackexchange.com/a/13486).
  
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
  * You'll notice above I did ``if`` statements without curly brackets/braces, this isn't recommended,[^48] but for simple one-line code it makes it look cleaner, just be careful.
  
    [^48]: clee, doynax. "Is it a bad practice to use an if-statement without curly braces?" _Stack Overflow_, 23 Jan. 2010, [stackoverflow.com/questions/2125066/is-it-a-bad-practice-to-use-an-if-statement-without-curly-braces](https://stackoverflow.com/questions/2125066/is-it-a-bad-practice-to-use-an-if-statement-without-curly-braces).
  
  * I added another flag to the FFmpegCommand to control the encoded video quality, lowering the CRF and using a slower encoding preset will give better video quality but may make the program laggy.[^49]
    ```cpp
    "-crf 17 -preset slow "     // Constant rate factor (18=visually lossless, 23=default)
    ```
    
  [^49]: llogan, et al. "H.264 Video Encoding Guide" _FFmpeg_, 12 Dec. 2024, [trac.ffmpeg.org/wiki/Encode/H.264](https://trac.ffmpeg.org/wiki/Encode/H.264).
  
  
  <!-- FONT LOADER AND TEXTURE ATLAS -->
  <!-- QUAD TEXT SHADERS (TALK ABOUT OPENGL OLD vs NEW (FIXED FUNCTION vs SHADER BASED) -->
  <!-- FINALLY SHOW RESULTS WITH TEXTURES -->
</details>

## Progress update 7 - Transformations - 29/12/24

<details><summary> Progress update 7 - Transformations - 29/12/24 </summary>
  
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
    ```glsl
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
    ```glsl
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
  * For some reason ``new`` seems to allocate more memory than expected, I need to store 800x600 RGB (0-255) pixel data, but I can get away with subtracting 2415 before getting a heap buffer overflow causing a segfault. When initializing a pointer, people recommended to set it to ``NULL``.[^50]
    
    [^50]: Dmitry. "Proper Way To Initialize Unsigned Char*" _Stack Overflow_, 2 Feb. 2011, [stackoverflow.com/a/4876907](https://stackoverflow.com/a/4876907).
  
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
  * [This website](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-10-transparency/) has a nice tutorial on Transparency in OpenGL and how to render multiple transparent triangles. The steps are essentially to draw everything that is opaque first so the depth buffer can discard hidden transparent triangles, sort the transparent triangles from furthest to closest, and render them in that order. They mention that needing to switch textures, or even worse, switching shader programs from triangle to triangle is very bad for performance,[^51] that's why games like _Minecraft_ use a texture atlas.
  * Here is a nice video that covers Transparency/Blending in their OpenGL _Minecraft_ clone:[^52]
    [![Watch the video](https://img.youtube.com/vi/_rPRVk75Y6Q/maxresdefault.jpg)](https://www.youtube.com/watch?v=_rPRVk75Y6Q&t=34s)
  * Here's another good video on Transparency & Blending:[^53]
    [![Watch the video](https://img.youtube.com/vi/crOfyWiWxmc/maxresdefault.jpg)](https://www.youtube.com/watch?v=crOfyWiWxmc&t=190s)
    
  
    [^51]: GraphicsTut. "Tutorial 10 : Transparency" _opengl-tutorial.org_, [opengl-tutorial.org/intermediate-tutorials/tutorial-10-transparency/](http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-10-transparency/).
    [^52]: Low Level Game Dev. "How the rendering pipeline of a Minecraft-like game looks like? OpenGL C++" _YouTube_, 4 Sep. 2024, [youtube.com/watch?v=_rPRVk75Y6Q](https://www.youtube.com/watch?v=_rPRVk75Y6Q).
    [^53]: Victor Gordan. "OpenGL Tutorial 17 - Transparency & Blending" _YouTube_, 28 May 2021, [youtube.com/watch?v=crOfyWiWxmc](https://www.youtube.com/watch?v=crOfyWiWxmc).
</details>

## Progress update 8 - 3D, Coordinate Systems, Camera, and Dear ImGui - 19/01/25

<details><summary> Progress update 8 - 3D, Coordinate Systems, Camera, and Dear ImGui - 19/01/25 </summary>

  * For some reason the program would display a white window for a brief second and immediately close with no error message, this happened when I increased the dimensions of the window. It turned out the issue was due to a segfault,[^54] I didn't allocate enough memory for the FFmpeg frame buffer, I was experimenting with it earlier as mentioned above noticing that I could allocate less than the number of pixels and it would work just fine, however just to be safe I put it back to normal and it works fine again.
  
    [^54]: swalog. "OpenGL GLFW window closes as soon as it opens" _Stack Overflow_, 26 Aug. 2014, [stackoverflow.com/a/25499922](https://stackoverflow.com/a/25499922).
  
  * I can now render in 3D using transform, model, view, and projection matrices. The diagram below shows the pipeline. The reason we break it into these steps is that making changes can be easier in different parts of the process. I switch from orthographic to perspective projection for 3D rendering.
  
    ![coordinate_systems](https://github.com/user-attachments/assets/8200e64e-22af-42fd-be4e-5fe1247f3698)
  
  * For the crate which is now a 3D cuboid, we will need 36 vertices (6 faces * 2 triangles * 3 vertices per triangle). At first I thought this was inefficient as a cuboid only has 8 vertices, however, in order for each face to have its own unique texture, each of the 3 vertices for the two triangles for every face will need to be stored, along with their texture coords. We could use EBOs to bring the number of vertices down to 8, but then different faces will be sharing vertices and so also sharing texture coords, so the faces can't all be assigned unique textures.
  
    ![ebo2](https://github.com/user-attachments/assets/ba763f3f-19d6-43ee-a823-d5d74ef54673)

    For context, there was very little performance difference with and without using an EBO. Others have explained that the main benefit of EBOs comes when dealing with meshes of models that contain hundreds of vertices, and in those situations faces sharing vertices (and thus texture coords) with neighboring faces is fine as they are so close to each other that their texture coords will be approximately the same.[^55]
  
    [^55]: AngusMcBurger. "Beginner questions on EBOs and when to use them" _Reddit_, 6 Sep. 2017, [reddit.com/r/opengl/comments/6yfav6/comment/dmnqog2/](https://www.reddit.com/r/opengl/comments/6yfav6/comment/dmnqog2/).
  
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
  
  * The _LearnOpenGL.com_ Camera chapter uses a weird if statement in the mouse callback function to check if this is the first use of the callback function and sets the ``xpos`` and ``ypos`` of the cursor to the centre of the screen, the reason being to avoid the whiplash cursor jump effect you would otherwise get, as the cursor is not initialized to the centre of the screen. A much cleaner alternative as mentioned by someone in the comments is to just use the GLFW function ``glfwSetCursorPos(window, lastX,lastY);`` before the while/render loop,[^56] where ``lastX`` and ``lastY`` are set to ``SCR_WIDTH / 2.0f`` and ``SCR_HEIGHT / 2.0f`` respectively, and remove the if statement in the mouse callback function. Another important point is to make sure ``YAW`` is initialized to ``-90.0f``.
  
    [^56]: Andy. "Camera" _LearnOpenGL.com_, 8 June 2017, [disq.us/p/1jdygzu](http://disq.us/p/1jdygzu).
  
  * I also clamped ``YAW`` to between ``0.0f`` and ``360.0f`` using the ``fmodf`` function and using ``Yaw + 360.0f`` rather than just ``Yaw``.
   
     ```cpp
     Yaw = fmodf(Yaw + 360.0f, 360.0f);
     ```
  
  * Another comment suggests using quaternions instead of matrices to control the camera in 3D. They linked to their [article](https://chanhaeng.blogspot.com/2018/09/quaternion-camera-implementation.html)[^57] which includes the code and a nice [article](https://graphics.stanford.edu/courses/cs348a-17-winter/Papers/quaternion.pdf) from Stanford on 'Quaternions and Rotations'. The main benefits of using quaternions over matrices are that they are more compact (4 numbers instead of 9 with 3x3 rotation matrices), efficient (16 multiplications and 12 additions instead of 27 multiplications and 18 additions), numerically stable (avoids the gimbal lock problem[^58]), and the geometric meaning is apparently clearer too.[^59] Although some mention that quaternions are better when chaining multiple rotations but not better when actually computing the rotating vectors,[^60] requiring ~30 operations rather than 15 operations like with 3D rotation matrix-vector multiplication.[^61] So "multiplication of matrices is ~2 times slower than quaternions. [But] matrix-vector multiplication is ~2 times faster [than quaternions]."[^62]
  
    [^57]: Chan Haeng Lee. "FPS Quaternion Camera Implementation (쿼터니언 카메라 구현)" _chanhaeng.blogspot.com_, 24 Sep. 2018, [chanhaeng.blogspot.com/2018/09/quaternion-camera-implementation.html](https://chanhaeng.blogspot.com/2018/09/quaternion-camera-implementation.html).
    [^58]: Henry Cohn. "The gimbal lock shows up in my quaternions" _Math Overflow_, 3 May 2012, [mathoverflow.net/a/95908](https://mathoverflow.net/a/95908).
    [^59]: Yan-Bin Jia. "Quaternions and Rotations Com S 477/577 Notes" _Stanford_, 10 Sep. 2013, [graphics.stanford.edu/courses/cs348a-17-winter/Papers/quaternion.pdf](https://graphics.stanford.edu/courses/cs348a-17-winter/Papers/quaternion.pdf).
    [^60]: Muphrid. "why is representing rotations through quaternions more compact and quicker than using matrices??" _Math Stack Exchange_, 9 July 2015, [math.stackexchange.com/a/1355206](https://math.stackexchange.com/a/1355206).
    [^61]: "Quaternions and spatial rotation - Performance comparisons" _Wikipedia_, 10 Jan. 2025, [en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Performance_comparisons](https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Performance_comparisons).
    [^62]: Kevin Kostlan. "Why are quaternions used for rotations?" _Stack Overflow_, 26 Apr. 2013, [stackoverflow.com/a/16246782](https://stackoverflow.com/a/16246782).
  
  * [This](https://lukearcamo.github.io/articles/coding/glfw-prevent-freezing-on-window-update) (nicely coded) article explains a way to stop OpenGL from pausing when resizing/moving the window, involving multithreading.[^63]
  
    [^63]: Luke Arcamo. "GLFW: Prevent freezing on window update" _lukearcamo.github.io_, 22 July 2021, [lukearcamo.github.io/articles/coding/glfw-prevent-freezing-on-window-update](https://lukearcamo.github.io/articles/coding/glfw-prevent-freezing-on-window-update).
  
  * I only just now learned about #include/header guards, which are used in headers to prevent creating duplicate copies of the header when unintentionally including the header more than once (this can happen sneakily when you include headers that include other headers you may already have included in your main.cpp file). This can be done two ways, using ``#ifndef``; ``#define``; and ``#endif``, or ``#pragma once``.[^64][^65] You will notice most header files use one or the other, all C++ standard library files have them, making it safe to include libraries in multiple files without the worry of duplicate code being generated.[^66]
  
    [^64]: Brian R. Bondy. "#pragma once vs include guards?" _Stack Overflow_, 17 July 2009, [stackoverflow.com/a/1143958/7875204](https://stackoverflow.com/a/1143958/7875204).
    [^65]: Alex Kremer. "What does #pragma once mean in C?" _Stack Overflow_, 25 Apr. 2011, [stackoverflow.com/a/5777009](https://stackoverflow.com/a/5777009).
    [^66]: eerorika, Kon. "Including a library in both the header file and the cpp file" _Stack Overflow_, 23 Feb. 2019, [stackoverflow.com/questions/54837486/including-a-library-in-both-the-header-file-and-the-cpp-file](https://stackoverflow.com/questions/54837486/including-a-library-in-both-the-header-file-and-the-cpp-file).
  
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
  
  * You can use headers to separate code into multiple files, the header file allows you to access functions and variables from other cpp files.[^67] The main benefit of separating code into multiple files is it makes the project cleaner and more readable, along with other benefits.[^68]
  
      [^67]: Puppy. "Using multiple .cpp files in c++ program?" _Stack Overflow_, 9 Aug. 2011, [stackoverflow.com/a/6995599](https://stackoverflow.com/a/6995599).
      [^68]: Dietrich Epp. "Why is it recommended to use multiple files when making a C++ project?" _Stack Overflow_, 20 Feb. 2023, [stackoverflow.com/a/75505133](https://stackoverflow.com/a/75505133).
  
  * I added ``#define`` and ``#if``, ``#endif`` blocks to my code to allow for different blocks of code to be built at compile-time. This differs from using ``if`` statements as the code will still be built but evaluated at runtime.[^69] For example:
  
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
    
    [^69]: T.J. Crowder. ""if " and " #if "; which one is better to use" _Stack Overflow_, 8 May 2013, [stackoverflow.com/a/16438775/7875204](https://stackoverflow.com/a/16438775/7875204).
  
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
  
  * [This blog post](https://diegomacario.github.io/2021/04/23/how-to-keep-the-aspect-ratio-of-an-opengl-window-constant.html)[^70] by Diego Macario (who has a really cool [2D physics simulator](https://diegomacario.github.io/about/) project) explains a way to make the viewport centred which seems similar to how I did it, however, when implementing their method, it didn't centre the viewport. Their method of getting the background outside the viewport to be black did work, which helps distinguish between the viewport and the window. The code to do so uses ``GL_SCISSOR_TEST`` and goes like this:
  
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
  
    [^70]: Diego Macario. "How to keep the aspect ratio of an OpenGL window constant when it's resized" _diegomacario.github.io_, 23 Apr. 2021, [diegomacario.github.io/2021/04/23/how-to-keep-the-aspect-ratio-of-an-opengl-window-constant.html](https://diegomacario.github.io/2021/04/23/how-to-keep-the-aspect-ratio-of-an-opengl-window-constant.html).
  
  * I implemented a PAUSE button that allows the user to gain access to the mouse, which I felt would be needed in the future as I would like to also add a GUI.
  
    Global variables:
    ```cpp
    bool    paused      = 0;
    float   lastX       = SCR_WIDTH / 2.0f;
    float   lastY       = SCR_HEIGHT / 2.0f;
    ```
  
    The ``mouse_callback(GLFWwindow* window, double xposIn, double yposIn)`` function:
    ```cpp
    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {
        // only update when unpaused
        if (!paused)
        {
            float xpos = static_cast<float>(xposIn);
            float ypos = static_cast<float>(yposIn);
    
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    
            lastX = xpos;
            lastY = ypos;
    
            camera.ProcessMouseMovement(xoffset, yoffset);
        }
    }
    ```
    
    Inside the ``processInput()`` callback function:
    ```cpp
    // Pause
    // -----
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        // unpausing
        if (paused)
        {
            glfwSetCursorPos(window, lastX, lastY);  // set cursor in last position before pausing to remove whiplash cursor jump
            // tell GLFW to capture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            paused = 0;
        }
        // pausing
        else
        {
            glfwSetCursorPos(window, lowerLeftCornerOfViewportX + SCR_WIDTH/2.0f, lowerLeftCornerOfViewportY + SCR_HEIGHT/2.0f);  // set cursor in centre of viewport
              // tell GLFW to uncapture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            paused = 1;
        }
    }
    ```
  
    Notice that the ``mouse_callback()`` function only updates the position of the cursor when the window is unpaused.
  
    Also notice that when unpausing, I call ``glfwSetCursorPos(window, lastX, lastY);`` to set the cursor to be in ``(lastX, lastY)``, the last position the cursor was in just before pausing, this prevents the camera from jumping to where the cursor is while paused. This solution was given in the comments of the **Camera** chapter of _LearnOpenGL.com_ which I have mentioned already above.[^56] More info on ``glfwSetCursorPos()`` and ``glfwSetInputMode()`` can be found in the [GLFW Input guide](https://www.glfw.org/docs/latest/input_guide.html#input_mouse).
    
  * Every time I switched from fullscreen mode back to windowed mode, the window seemed to lose its title bar. It turns out this was just due to me setting the window position to be ``(0, 0)``.[^71]
  
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
  
    Global variable:
    ```cpp
    bool fullscreen = 0;
    ```
    
    The full solution inside the ``processInput()`` callback function:
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
  
    [^71]: Shane McPhillips, immuv. "glfw window with no title bar" _Stack Overflow_, 23 Mar. 2022, 11 Jan 2022, [stackoverflow.com/questions/70582321/glfw-window-with-no-title-bar](https://stackoverflow.com/questions/70582321/glfw-window-with-no-title-bar).
  
  * I wanted to add a GUI to make the program more functional and interactive. I ended up going with Dear ImGui as it seems to be bloat-free, minimal, and easy to use. People have also recommended it as a good GUI to use with OpenGL that works right out of the box. I was wondering why it's called "Dear ImGui", at first I thought "ImGui" stood for "I'm GUI", but it actually stands for "Immediate Mode GUI", as for "Dear", [here](https://github.com/ocornut/imgui/discussions/4041#discussioncomment-5409937)'s a slightly underwhelming explanation from the creator, [here](https://news.ycombinator.com/item?id=24986908#24987826)'s a more amusing guess involving Santa. [Here](https://www.youtube.com/watch?v=VRwhNKoxUtk)'s a nice tutorial, but I will explain in detail the steps. First, you need to download the ZIP file containing the Dear ImGui code, [here](https://github.com/ocornut/imgui)'s the link to the repository. Next, you need to extract these files:
  
    ```bash
    imgui-master/
    ├── imconfig.h
    ├── imgui.cpp
    ├── imgui.h
    ├── imgui_draw.cpp
    ├── imgui_internal.h
    ├── imgui_tables.cpp
    ├── imgui_widgets.cpp
    ├── imstb_rectpack.h
    ├── imstb_textedit.h
    ├── imstb_truetype.h
    ├── imgui_demo.cpp
    └── backends/
        ├── imgui_impl_opengl3.cpp
        ├── imgui_impl_opengl3.h
        ├── imgui_impl_opengl3_loader.h
        ├── imgui_impl_glfw.cpp
        └── imgui_impl_glfw.h 
    ```
  
    and put them into a new folder, ``imgui``, inside your local project's ``include`` folder:
  
    ```bash
    YourProject/
    └── include/
        └── imgui/
            ├── imconfig.h
            ├── imgui.cpp
            ├── imgui.h
            ├── imgui_draw.cpp
            ├── imgui_internal.h
            ├── imgui_tables.cpp
            ├── imgui_widgets.cpp
            ├── imstb_rectpack.h
            ├── imstb_textedit.h
            ├── imstb_truetype.h
            ├── imgui_demo.cpp
            ├── imgui_impl_opengl3.cpp
            ├── imgui_impl_opengl3.h
            ├── imgui_impl_opengl3_loader.h
            ├── imgui_impl_glfw.cpp
            └── imgui_impl_glfw.h
    ```
  
    Now what I initially did was compile all of these files at compile-time, which looked like this in ``tasks.json``'s "args":
  
    ```json
    // IMGUI
    "${workspaceFolder}/include/imgui/imgui.h",
    "${workspaceFolder}/include/imgui/imconfig.h",
    "${workspaceFolder}/include/imgui/imgui_impl_glfw.h",
    "${workspaceFolder}/include/imgui/imgui_impl_opengl3.h",
    "${workspaceFolder}/include/imgui/imgui_impl_opengl3_loader.h",
    "${workspaceFolder}/include/imgui/imgui_internal.h",
    "${workspaceFolder}/include/imgui/imstb_rectpack.h",
    "${workspaceFolder}/include/imgui/imstb_textedit.h",
    "${workspaceFolder}/include/imgui/imstb_truetype.h",
    "${workspaceFolder}/include/imgui/imgui.cpp",
    "${workspaceFolder}/include/imgui/imgui_draw.cpp",
    "${workspaceFolder}/include/imgui/imgui_impl_glfw.cpp",
    "${workspaceFolder}/include/imgui/imgui_impl_opengl3.cpp",
    "${workspaceFolder}/include/imgui/imgui_tables.cpp",
    "${workspaceFolder}/include/imgui/imgui_widgets.cpp",
    ```
  
    Not only does this look unwieldly, compiling all of these files made the compile-time go from 23s to 1m25s. What you should do instead is pre-compile the files into a static library, and link to that library at compile-time. The steps to do that are as follows:
  
    Step 1: Open MSYS2 (using the MINGW64 environment), and navigate to the ``imgui`` folder in your local project's ``include`` folder (replace ``YourProject`` in the following command with the name of your repository!)
    
    ```bash
    cd C:/YourProject/include/imgui
    ```
  
    Step 2: Compile the Source Files into Object Files (``-c``: compiles the .cpp files into .o object files) (replace ``YourProject`` in the following command with the name of your repository!)
    
    ```bash
    g++ -c -std=c++17 -O2 -I/c/YourProject/include imgui.cpp imgui_draw.cpp imgui_widgets.cpp imgui_tables.cpp imgui_impl_opengl3.cpp imgui_impl_glfw.cpp
    ```
    
    Step 3: Archive Object Files into a Static Library (``ar``: archives the object files into a library. ``rcs``: ``r`` inserts files into the archive; ``c`` creates the archive if it doesn't exist; ``s`` writes an index to the archive for faster linking. ``libimgui.a``: the name of the resulting static library)
    
    ```bash
    ar rcs libimgui.a imgui.o imgui_draw.o imgui_widgets.o imgui_tables.o imgui_impl_opengl3.o imgui_impl_glfw.o
    ```
  
    Step 4: Move the newly created static library ``libimgui.a`` into your local project's ``lib`` folder with the rest of the static libraries
  
    Step 5: Link to the library in ``tasks.json``'s "args"
  
    ```json
    "-limgui",                                  // ImGui library
    ```
  
    All you need to do now is include the imgui headers in main.cpp as well as the initialization code and the code to render a simple window and you're done!
  
    At the top of main.cpp:

    ```cpp
    #include <imgui\imgui.h>
    #include <imgui\imgui_impl_glfw.h>
    #include <imgui\imgui_impl_opengl3.h>
    #define  IMGUI  1
    ```
  
    At initialization, before the render loop:

    ```cpp
    #if IMGUI==1
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    #endif
    ```
  
    Inside the render loop, before you render your scene:

    ```cpp
    #if IMGUI==1
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    #endif
    ```
  
    Inside the render loop. Putting this while drawing to the non-MSAA FBO will mean the GUI will be visible in the screen recording too. Putting this at the very end when drawing to the default framebuffer will allow you to move the GUI outside of the viewport and still be visible, but the GUI won't be visible in the screen recording.

    ```cpp
    #if IMGUI==1
    ImGui::Begin("My name is window, ImGUI window");
    ImGui::Text("Hello there adventurer!");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    #endif
    ```
  
    Just after the render loop, when de-allocating resources:

    ```cpp
    #if IMGUI==1
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    #endif
    ```
  
    In order to keep the Dear ImGui window in the same position relative to the viewport when the user resizes the OpenGL window I add this code to the ``framebuffer_size_callback()`` function:
  
    ```cpp
    #if IMGUI==1
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(100, 100+lowerLeftCornerOfViewportY*2));
    ImGui::Begin("My name is window, ImGUI window");
    ImGui::Text("Hello there adventurer!");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    #endif
    ```
    
    I added ``#define IMGUI 1``, ``#if IMGUI==1``, ``#endif`` statements so that I can choose to build the program with or without a GUI.
  
  * I spent a lot of time trying to get the GUI to work nicely when the OpenGL window was resized, for some reason when resizing the OpenGL window, the GUI's "clickable" region was where it originally was, and not where the viewport and the GUI moved to be in the centre of the resized window. And no matter what I did: changing the source/destination rectangle bounds in ``glBlitFramebuffer()`` when blitting from the MSAA FBO to the non-MSAA FBO, and from the non-MSAA FBO to the default on-screen framebuffer; or adjusting ``glViewport()``, nothing made it work nicely, either the GUI would mess up, or the viewport would mess up and cut off.
  
    So after days of failing to get it to work I've decided to just not centre the viewport, this is so far the only thing that ensures the viewport and GUI don't mess up. Maybe later I can come back and get the viewport to be centred when the window is resized while also not messing up the GUI or the viewport itself, but for now this compromise will have to do.
  
  * Doing this will centre the viewport and doesn't mess up the viewport or the screen recording, but the ImGui window messes up when the OpenGL window is resized (the clickable region is not in the same place as where the ImGui window appears):
    
    ```cpp
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
    
    // render scene
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                      0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                              GL_COLOR_BUFFER_BIT,           // buffer mask
                                       GL_LINEAR);           // scale filter
    
    // render text and atlas
    // render ImGui window
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,                                 // src rect
                       lowerLeftCornerOfViewportX,                                 // dst rect
                       lowerLeftCornerOfViewportY,      
                       lowerLeftCornerOfViewportX + static_cast<int>(SCR_WIDTH),
                       lowerLeftCornerOfViewportY + static_cast<int>(SCR_HEIGHT),
                              GL_COLOR_BUFFER_BIT,                                 // buffer mask
                                      GL_NEAREST);                                 // scale filter
  
    glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame);
    ```
  
  * Doing this doesn't mess up the ImGui window, but the viewport gets cut off and the screen recording messes up:
  
    ```cpp
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
    
    // render scene
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,                                 // src rect
                       lowerLeftCornerOfViewportX,                                 // dst rect
                       lowerLeftCornerOfViewportY,      
                       lowerLeftCornerOfViewportX + static_cast<int>(SCR_WIDTH),
                       lowerLeftCornerOfViewportY + static_cast<int>(SCR_HEIGHT),
                              GL_COLOR_BUFFER_BIT,                                 // buffer mask
                                       GL_LINEAR);                                 // scale filter
    
    // viewport gets cut off with or without adding this
    glViewport(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, SCR_WIDTH, SCR_HEIGHT);
    // render text and atlas
    // render ImGui window
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(lowerLeftCornerOfViewportX,                                  // src rect
                      lowerLeftCornerOfViewportY,      
                      lowerLeftCornerOfViewportX + static_cast<int>(SCR_WIDTH),
                      lowerLeftCornerOfViewportY + static_cast<int>(SCR_HEIGHT),
                      lowerLeftCornerOfViewportX,                                  // dst rect
                      lowerLeftCornerOfViewportY,      
                      lowerLeftCornerOfViewportX + static_cast<int>(SCR_WIDTH),
                      lowerLeftCornerOfViewportY + static_cast<int>(SCR_HEIGHT),
                              GL_COLOR_BUFFER_BIT,                                 // buffer mask
                                      GL_NEAREST);                                 // scale filter
  
    // screen recording gets messed up with either option
    // glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame);
    glReadPixels(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame);
    ```
  
  * One fix is to just render the viewport in the bottom left of the screen and not centre it when the OpenGL window gets resized. This doesn't mess up the Dear ImGui window, the viewport, or the screen recording:
  
    ```cpp
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
    
    // render scene
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,                                 // src rect
                      0, 0, SCR_WIDTH, SCR_HEIGHT,                                 // dst rect
                              GL_COLOR_BUFFER_BIT,                                 // buffer mask
                                       GL_LINEAR);                                 // scale filter
    
    // render text and atlas
    // render ImGui window
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,                                 // src rect
                      0, 0, SCR_WIDTH, SCR_HEIGHT,                                 // dst rect
                              GL_COLOR_BUFFER_BIT,                                 // buffer mask
                                      GL_NEAREST);                                 // scale filter
  
    glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame);
    ```
  
  * Another fix is to render the Dear ImGui window to the default framebuffer rather than the non-MSAA FBO. This also doesn't mess up the Dear ImGui window, the viewport, or the screen recording, and the viewport is centred, however, the Dear ImGui window doesn't show up in the screen recording. One other benefit is that the Dear ImGui window can be dragged outside of the viewport and doesn't disappear unlike the previous fix, as it is not rendered onto the FBO:
  
    ```cpp
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
    
    // render scene
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); 
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,                                 // src rect
                      0, 0, SCR_WIDTH, SCR_HEIGHT,                                 // dst rect
                              GL_COLOR_BUFFER_BIT,                                 // buffer mask
                                       GL_LINEAR);                                 // scale filter
    
    // render text and atlas
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,                                 // src rect
                       lowerLeftCornerOfViewportX,                                 // dst rect
                       lowerLeftCornerOfViewportY,      
                       lowerLeftCornerOfViewportX + static_cast<int>(SCR_WIDTH),
                       lowerLeftCornerOfViewportY + static_cast<int>(SCR_HEIGHT),
                              GL_COLOR_BUFFER_BIT,                                 // buffer mask
                                      GL_NEAREST);                                 // scale filter
  
    glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame);
    
    // render ImGui window
    ```
  
  https://github.com/user-attachments/assets/4be425a8-0235-4756-8a57-1acb1ff23ade
</details>

## Progress update 9 - FFmpeg CLI→API, asynchronous readback with PBOs, and CMake - 30/01/25

<details><summary> FFmpeg CLI→API </summary>
    
  ### FFmpeg CLI→API
  
  * Before getting into the **Lighting** chapter, I finally fixed a major bug in my screen recording implementation. When encoding a video using FFmpeg, you have to decide on a framerate, I went with 60fps, what this meant though was all the encoded frames would be separated by 1/60th of a second, when in reality the framerate is variable and can drop when the program lags. When the FPS was lower than 60, the recording would speed up, as frames that should be further apart were being brought closer together in time, and vice versa when the FPS was higher than 60. My fix as of now was to just use V-Sync to cap the FPS at 60 and hope the FPS didn't drop too low.
  
    Here is an example of the recording "speeding up" as the FPS dropped lower than 60:
    
    https://github.com/user-attachments/assets/dd3861fe-e2b1-432b-bfbe-28bceb96fde9
  
    I was working on fixing this by encoding the frame but duplicated whenever the FPS dropped lower than 60. This worked, but as you can imagine that would involve using another buffer to store the old frame, and another much larger buffer to store all the duplicate frames (which just seems so redundant). Ironically, trying to get the lag to display correctly in the recording just made the program more laggy, basically unusable.
  
    So all hope seemed lost, until I had the idea to search for FFmpeg dynamic framerate video encoding, surely FFmpeg would have some functionality in place for that? And that's when I found [this](https://video.stackexchange.com/a/2595) godsend of a post which explained how to do it. And the funny thing is the solution is just one line of code, it involves telling FFmpeg to use the "system time at time of receipt as the timestamp"[^72] for each frame. "Without embedded timestamps, ffmpeg will assign timestamps in serial order, thus losing the temporal relation between the incoming frames."[^72]
  
    [^72]: Gyan. "FFmpeg encode variable input framerate" _Video Production Stack Exchange_, 11 Feb. 2019, [video.stackexchange.com/a/25953](https://video.stackexchange.com/a/25953).
  
    Here is the one line of code you need to add to the ``FFmpegCommand``:
  
    ```cpp
    "-use_wallclock_as_timestamps 1 "
    ```
  
    And here is the result when adding that one line of code, the recording matches the on-screen program exactly and no drop in performance:
  
    https://github.com/user-attachments/assets/4f44bb9a-8c7f-45c6-80d5-dbf90b83d570
  
  * This works and the code is very minimal, but I felt like the performance could be improved. Remembering from much earlier I mentioned a Stack Overflow post that outlined a method for screen recording using "``libavcodec`` and ``libavformat``, [...] the libraries upon which ``ffmpeg`` is actually built"[^73] rather than using the FFmpeg command-line. I decided to try use the FFmpeg API, which is much more verbose and requires more library linking...
  * I found this [blog](https://friendlyuser.github.io/posts/tech/cpp/Using_FFmpeg_in_C++_A_Comprehensive_Guide)[^74] which has a guide on using the FFmpeg API, although this didn't really help much either as the API is just so verbose and at the end of the day it's just more boilerplate code. I ended up using ChatGPT again as there really aren't any easy-to-understand guides online.
  
   [^73]: Andon M. Coleman. "Saving the openGL context as a video output" _Stack Overflow_, 28 Sep. 2013, [stackoverflow.com/a/19071087/7875204](https://stackoverflow.com/a/19071087/7875204).  
  
   [^74]: David Li. "Using FFmpeg in C++ A Comprehensive Guide" _friendlyuser.github.io_, 3 May 2023, [friendlyuser.github.io/posts/tech/cpp/Using_FFmpeg_in_C++_A_Comprehensive_Guide](https://friendlyuser.github.io/posts/tech/cpp/Using_FFmpeg_in_C++_A_Comprehensive_Guide).
  
  * Step 1: Download FFmpeg Source Code
    * Download the source from: https://github.com/FFmpeg/FFmpeg by clicking on ``Download ZIP``.
    * Or from: https://ffmpeg.org/download.html
    * Then extract the archive
  * Step 2: Configure FFmpeg for Static Compilation
    * Update packages
  
      ```shell
      pacman -Syu
      ```
  
    * Install GNU Make
      
      ```shell
      pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
      ```
  
    * FFmpeg requires NASM (Netwide Assembler) to compile hand-optimized assembly code for x86. Install NASM and verify installation:
  
      ```shell
      pacman -S mingw-w64-x86_64-nasm
      ```
      ```shell
      nasm --version
      ```
  
    * Install ``libiconv`` in MSYS2
  
      ```shell
      pacman -S mingw-w64-x86_64-libiconv
      ```
       
    * Navigate to the extracted FFmpeg folder (replace ``path-to-ffmpeg`` with the actual path)
  
      ```shell
      cd C:/path-to-ffmpeg
      ```
  
    * Run the following command:
   
      ```shell
      ./configure \
          --arch=x86_64 \
          --target-os=mingw32 \
          --prefix=/c/ffmpeg-build \
          --disable-shared \
          --enable-static \
          --disable-debug \
          --disable-doc \
          --disable-ffplay \
          --disable-ffprobe \
          --disable-ffmpeg \
          --disable-network \
          --enable-small \
          --enable-avdevice \
          --cc=x86_64-w64-mingw32-gcc \
          --cxx=x86_64-w64-mingw32-g++ \
          --pkg-config=pkg-config \
          --extra-cflags="-fvisibility=default" \
          --extra-libs="-liconv -llzma -lole32 -luuid -lbcrypt"
      ```
  
      Explanation of flags:
      
      * ``--enable-static`` → Builds static .a libraries.
      * ``--disable-shared`` → Disables dynamic .dll libraries.
      * ``--prefix=/c/ffmpeg-build`` → Where FFmpeg will be compiled after building.
      * ``--disable-ffplay / --disable-ffprobe / --disable-ffmpeg`` → Avoid building unnecessary programs.
      * ``--enable-small`` → Optimized build.
      * ``--cc=x86_64-w64-mingw32-gcc \ --cxx=x86_64-w64-mingw32-g++`` → Manually Set the Compiler 
  
  * Step 3: Compile FFmpeg
    * Once configured, build FFmpeg using GNU Make (each command may take a while to complete):
  
      ```shell
      make clean
      ```
      ```shell
      make -j$(nproc)
      ```
      ```shell
      make install
      ```
  
    * After compilation, you should find the ``.a`` static libraries in ``C:/ffmpeg-build/lib/``:
      * ``libavcodec.a``
      * ``livavdevice.a``
      * ``libavfilter.a``
      * ``libavformat.a``
      * ``libavutil.a``
      * ``libswresample.a``
      * ``libswscale.a``
  
  * Step 4: Move Required FFmpeg Files to Your Project
    * Copy the entire FFmpeg ``include/`` directory to your project's ``include/`` folder: ``C:\ffmpeg-build\include\``  →  ``C:\YourProject\include\``
    * Copy all the ``.a`` static library files from FFmpeg's ``lib/`` folder to your project's ``lib/`` directory: ``C:\ffmpeg-build\lib\``  →  ``C:\YourProject\lib\``
  
    
  * Step 5: Link FFmpeg Statically in Your C++ Project
    * Add these linker flags to ``tasks.json``'s ``args``:
  
      ```cpp
      "-lavformat",   // order matters! (libavformat depends on libavcodec so linker needs to resolve symbols from libavformat before libavcodec is processed)
      "-lavcodec",
      "-lavutil",       // FFmpeg Internal Functions
      "-lswscale",
      "-lswresample",
      "-static",        // Forces static linking (no dependency on shared libraries).
      "-pthread",       // Links the POSIX threading library for multithreading. FFmpeg may use threads internally.
      "-lm",            // Links the math library (libm.a) for mathematical functions (e.g., pow(), sqrt()).
      "-lws2_32",       // Links Windows Sockets 2 library (ws2_32.dll). Required for network functionality in FFmpeg. (We can remove this since we disabled networking)
      "-liconv",        // libiconv (iconv)
      "-llzma",         // liblzma (lzma)
      "-lole32",        // libole32 (ole32 Windows COM Libraries)
      "-lbcrypt",       // Windows Cryptography
      "-luuid",         // Windows COM API
      "-lstrmiids",
      ```
  
  * Step 6: Include the header files in main.cpp
  
      ```cpp
      extern "C" {
      #include <libavformat/avformat.h>
      #include <libavcodec/avcodec.h>
      #include <libavutil/opt.h>
      #include <libswscale/swscale.h>
      #include <libavutil/log.h>
      }
      ```
  
  * Step 7: Add the functions
  
    * Initialize encoder function:
  
      ```cpp
      bool initializeEncoder(const char* filename) {
          // Set up logging to a file
          // FILE* logFile = fopen("ffmpeg_log.txt", "w");
          // av_log_set_flags(AV_LOG_FORCE);  // Force log output to be visible
          // av_log_set_callback([](void* ptr, int level, const char* fmt, va_list vl) {
          //     vfprintf(logFile, fmt, vl);
          // });
      
          avformat_alloc_output_context2(&formatCtx, nullptr, "mp4", filename);
          if (!formatCtx) {
              printf("Could not create output context\n");
              return false;
          }
      
          // Find the H.264 encoder
          const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
          if (!codec) {
              printf("H.264 encoder not found\n");
              return false;
          }
      
          // Create a new stream
          videoStream = avformat_new_stream(formatCtx, codec);
          if (!videoStream) {
              printf("Could not create video stream\n");
              return false;
          }
      
          // Set up the codec context
          int FPS = static_cast<int>(framerate);
          codecCtx = avcodec_alloc_context3(codec);
          codecCtx->width = SCR_WIDTH;
          codecCtx->height = SCR_HEIGHT;
          codecCtx->time_base = (AVRational){1, FPS*1000};  // Frame rate
          codecCtx->framerate = (AVRational){FPS, 1};
          codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
          codecCtx->bit_rate = 60000000;  // 60 Mbps
          codecCtx->gop_size = 10;
          codecCtx->max_b_frames = 1;
          // codecCtx->rc_max_rate = codecCtx->bit_rate;  // Set maximum bitrate limit
          // codecCtx->rc_buffer_size = codecCtx->bit_rate;  // Set buffer size to match bitrate
      
          // Set H.264 options
          av_opt_set(codecCtx->priv_data, "preset", "slow", 0);
          av_opt_set(codecCtx->priv_data, "crf", "18", 0);
          // av_opt_set(codecCtx->priv_data, "bitrate", "30000k", 0);  // Set bitrate to 5000 kbps
          // av_opt_set(codecCtx->priv_data, "maxrate", "30000k", 0);  // Max bitrate
          // av_opt_set(codecCtx->priv_data, "bufsize", "30000k", 0); // Buffer size
          // av_opt_set(codecCtx->priv_data, "profile", "high", 0);  // Set profile to "high" for better quality
          av_opt_set(codecCtx->priv_data, "pix_fmt", "yuv420p", 0);  // Try using yuv422p or yuv444p for better quality
      
      
          // Open the encoder
          if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
              printf("Could not open codec\n");
              return false;
          }
      
          // Assign codec to stream
          videoStream->codecpar->codec_id = AV_CODEC_ID_H264;
          avcodec_parameters_from_context(videoStream->codecpar, codecCtx);
      
          // Make sure that your AVStream has the same time_base
          videoStream->time_base = codecCtx->time_base;
          // Force the output stream to interpret it as CFR (Constant Frame Rate)
          // videoStream->avg_frame_rate = (AVRational){60, 1};  // Force the output to exactly 60 FPS
          // av_dict_set(&formatCtx->metadata, "video-cfr", "1", 0);
      
          // Open output file
          if (!(formatCtx->oformat->flags & AVFMT_NOFILE)) {
              if (avio_open(&formatCtx->pb, filename, AVIO_FLAG_WRITE) < 0) {
                  printf("Could not open output file\n");
                  return false;
              }
          }
      
          // Write header
          if (avformat_write_header(formatCtx, nullptr) < 0) {
              std::cerr << "Error writing header to output file!" << std::endl;
              return false;  // Handle failure
          }
      
          // Allocate frame and conversion context
          frameX = av_frame_alloc();
          frameX->format = AV_PIX_FMT_YUV420P;
          frameX->width = SCR_WIDTH;
          frameX->height = SCR_HEIGHT;
          av_frame_get_buffer(frameX, 32);
      
          swsCtx = sws_getContext(SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_RGB24,
                                  SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_YUV420P,
                                  SWS_BILINEAR, nullptr, nullptr, nullptr);
          return true;
      }
      ```
  
    * Encode frame function:
  
      ```cpp
      bool encodeFrame(const uint8_t* rgbData) {
          // Convert RGB to YUV420P
          uint8_t* inData[1] = {(uint8_t*)rgbData};  // Input RGB data
          int inLinesize[1] = {3 * SCR_WIDTH};          // Input stride
          sws_scale(swsCtx, inData, inLinesize, 0, SCR_HEIGHT, frameX->data, frameX->linesize);
      
          frameX->pts = static_cast<int64_t>(crntTime * 60 * 1000); //* AV_TIME_BASE);
          // frameX->pts = 1000 * frameIndex; // Simple frame count to force 60 FPS
          // frameIndex++;
          
          // Encode the frame
          if (avcodec_send_frame(codecCtx, frameX) < 0) {
              printf("Error sending frame to encoder\n");
              return false;
          }
      
          // Retrieve the encoded packet
          pkt.data = nullptr;
          pkt.size = 0;
          // pkt.dts = (frameX->pts < pkt.dts) ? pkt.dts + 1 : frameX->pts;
          pkt.dts = frameX->pts;
      
          while (avcodec_receive_packet(codecCtx, &pkt) == 0) {
              pkt.stream_index = videoStream->index;
              av_interleaved_write_frame(formatCtx, &pkt);
              av_packet_unref(&pkt);
          }
      
          return true;
      }
      ```
  
    * Finalize encoder function:
   
      ```cpp
      void finalizeEncoder() {
          avcodec_send_frame(codecCtx, nullptr);
          while (avcodec_receive_packet(codecCtx, &pkt) == 0) {
              pkt.stream_index = videoStream->index;
              av_interleaved_write_frame(formatCtx, &pkt);
              av_packet_unref(&pkt);
          }
      
          av_write_trailer(formatCtx);
          avcodec_free_context(&codecCtx);
          avformat_free_context(formatCtx);
          sws_freeContext(swsCtx);
          av_frame_free(&frameX);
      }
      ```
  * With this implementation, using the FFmpeg API rather than the CLI, video recording is MUCH smoother. Which isn't a surprise as this was not what the CLI was meant for, but exactly what the API was meant for.
</details>

  <!--
  ### Dear ImGui
  * You may have noticed I have added a GUI, I am using [Dear ImGui](https://github.com/ocornut/imgui), it is easy to implement, works right out of the box, and bloat-free. In order to set it up I simply put all the ImGui headers in my project's /include/ folder, and the ImGui source files compiled to a static library in my project's /lib/ folder. The next step is to get something like ImPlot so I can do real-time plots. Inside ``main.cpp`` you setup Dear ImGui as follows:

    Include headers
    ```cpp
    #include <imgui/imgui.h>
    #include <imgui/imgui_impl_glfw.h>
    #include <imgui/imgui_impl_opengl3.h>
    ```

    Before the render loop
    ```cpp
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ```

    Inside the render loop at the top
    ```cpp
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ```

    Inside the render loop, render the ImGui window
    ```cpp
    ImGui::Begin("My name is window, ImGUI window");
    ImGui::Text("Hello there adventurer!");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ```

    Outside the render loop at the end of ``main()``
    ```cpp
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ```
  -->

<details><summary> Asynchronous readback with PBOs </summary>
  
  ### Asynchronous readback with PBOs
  * There is something in OpenGL called Pixel Buffer Objects (PBOs), which to my knowledge act as frame buffers storing RGB pixel data after doing ``glReadPixels(...)``. The major benefit of PBOs is "asynchronous DMA (Direct Memory Access) transfer without involving CPU cycles".[^24] With 2 ping-ponging PBOs performing asynchronous glReadPixels(), I get a ~2x performance improvement.

    [![gl_pbo07](https://github.com/user-attachments/assets/266244e1-dfd3-4bb9-8bf1-32c671fd6ed7)](https://www.songho.ca/opengl/gl_pbo.html#pack)

    ```cpp
    // pbo settings
    // ------------
    const unsigned int  PBO_COUNT       =  2;
    GLuint pboIds[PBO_COUNT];
    int index = 0;
    int nextIndex = 1;
    ```

    ```cpp
    // create 2 pixel buffer objects, you need to delete them when program exits.
    // glBufferData() with NULL pointer reserves only memory space.
    glGenBuffers(PBO_COUNT, pboIds);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[1]);
    glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_DYNAMIC_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    ```

    Inside the render loop
    ```cpp
    // Step 4: Read pixels from the resolved FBO for off-screen encoding (with PBOs)
    // -----------------------------------------------------------------------------
    index = (index + 1) % 2;       // "index" is used for GPU to read pixels from framebuffer to a PBO
    nextIndex = (index + 1) % 2;   // "nextIndex" is used for CPU to update/process pixels in the other PBO
    // read pixels from framebuffer to PBO (pack). glReadPixels() should return immediately.
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
    glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // map the PBO to process its data by CPU (unpack)
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[nextIndex]);
    GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
    if (ptr)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT,           // src rect
                          0, 0, SCR_WIDTH, SCR_HEIGHT,           // dst rect
                                  GL_COLOR_BUFFER_BIT,           // buffer mask
                                          GL_NEAREST);           // scale filter
    
        // Flip the frame vertically
        flipFrameVertically(ptr);
        
        // Encode frame using FFmpeg
        encodeFrame(ptr);
        
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    }

    // back to conventional pixel operation
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    ```

    At the end of ``main()``
    ```cpp
    glDeleteBuffers(PBO_COUNT, pboIds);
    ```
</details>

<details><summary> CMake </summary>

  ### CMake
  * And finally, I set up a CMakeLists.txt file in order to build the project with CMake as well as with VSCode.

    ```cmake
    # Minimum CMake version
    cmake_minimum_required(VERSION 3.31)
    
    # Project name and version
    project(myprogram VERSION 1.0)
    
    # Set C++ standard
    set(CMAKE_CXX_STANDARD 		 17)
    set(CMAKE_CXX_STANDARD_REQUIRED YES)
    set(CMAKE_CXX_EXTENSIONS	 ON)
    
    # CMake Build Type (Debug/Release)
    set(CMAKE_BUILD_TYPE Release)
    
    # Specify the source files
    set(SOURCES
        src/main.cpp
        src/glad.c
    )    
    
    # Include headers for FFmpeg and others
    include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/freetype2
        ${CMAKE_SOURCE_DIR}/lib/GLFW
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg
        ${CMAKE_SOURCE_DIR}/lib/freetype
        ${CMAKE_SOURCE_DIR}/lib/imgui
    )
    
    # Executable target
    add_executable(${PROJECT_NAME} ${SOURCES})
    
    # Define the source and destination directories for shaders
    set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/shaders)
    set(SHADER_DEST_DIR   ${CMAKE_BINARY_DIR}/shaders)
    
    # Create a custom command to copy shaders
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR}
        COMMENT "Copying /shaders/ to build directory"
    )
    
    # Define the source and destination directories for textures
    set(TEXTURE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/textures)
    set(TEXTURE_DEST_DIR   ${CMAKE_BINARY_DIR}/textures)
    
    # Create a custom command to copy textures
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${TEXTURE_SOURCE_DIR} ${TEXTURE_DEST_DIR}
        COMMENT "Copying /textures/ to build directory"
    )
    
    # Find all .a files in lib directory recursively
    # file(GLOB_RECURSE LIB_FILES "${CMAKE_SOURCE_DIR}/lib/**/*.a")
    
    # Link all the found libraries
    target_link_libraries(${PROJECT_NAME} #${LIB_FILES}
        ${CMAKE_SOURCE_DIR}/lib/imgui/libimgui.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavformat.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavcodec.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavutil.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libswscale.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libswresample.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libpthread.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libm.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libws2_32.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libiconv.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/liblzma.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libole32.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libbcrypt.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libuuid.a
        ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libstrmiids.a
        ${CMAKE_SOURCE_DIR}/lib/GLFW/libglfw3.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libfreetype.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libharfbuzz.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libgraphite2.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libpng.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libz.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libbz2.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libbrotlidec.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libbrotlienc.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libbrotlicommon.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/librpcrt4.a
        ${CMAKE_SOURCE_DIR}/lib/freetype/libgdi32.a
        ${CMAKE_SOURCE_DIR}/lib/libopengl32.a
    )
        
    
    # Set output directory for the executable
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    ```

</details>
  
  <!-- Replace the old FFmpeg function calls with new functions -->
  <!-- Mention using variable framerate (VFR) rather than constant framerate (CFR) as CRF was messing up with variable framerate content, so encoded video won't be 60fps but will display recording accurately -->
  <!-- Show example video recording and mention how much smoother compared to old method -->
  <!-- Talk about how when calling ffmpeg.exe it's a I/O bottleneck etc. and talk about the stack overflow post that used the FFmpeg API -->
</details>

## Progress update 10 - ImPlot, and making an Encoder namespace - 10/03/25

<details><summary> ImPlot </summary>

  ### ImPlot
  * Delaying doing the **Lighting** chapter again, but I was able to implement ImPlot the same way as with ImGui.
    
      Include headers
      ```cpp
      #include <imgui/imgui.h>
      #include <imgui/imgui_impl_glfw.h>
      #include <imgui/imgui_impl_opengl3.h>
      #include <implot/implot.h>                  // <-- Add for ImPlot
      ```
  
      Before the render loop
      ```cpp
      #if IMGUI==1
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImPlot::CreateContext();                    // <-- Add for ImPlot
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(window, true);
      ImGui_ImplOpenGL3_Init("#version 430");
      ```
  
      Inside the render loop at the top (nothing new to add)
      ```cpp
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      ```
  
      Inside the render loop, render the ImGui and ImPlot windows
      ```cpp
      ImGui::Begin("My name is window, ImGUI window");
      ImGui::Text("Hello there adventurer!");
      ImGui::End();
      
      // Show the ImPlot demo window              // <-- Add for ImPlot
      if (ImGui::Begin("ImPlot Demo")) {          // <-- Add for ImPlot
          ImPlot::ShowDemoWindow();               // <-- Add for ImPlot
      }                                           // <-- Add for ImPlot
      ImGui::End();                               // <-- Add for ImPlot
      
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      ```
  
      Outside the render loop at the end of ``main()``
      ```cpp
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImPlot::DestroyContext();                   // <-- Add for ImPlot
      ImGui::DestroyContext();
      ```

  * I also decided to keep the Dear ImGui and ImPlot source code, leaving it inside a ``/third_party/`` folder, and compiling and linking it at compile-time using CMake, instead of precompiling it beforehand and just linking it at compile-time. I can afford to do this as the Dear ImGui and ImPlot code is not that large and can be compiled in a few seconds using ``ninja``, compared to the FFmpeg source code, which can take a few minutes... The ``CMakeLists.txt`` file now looks like this:
    
      ```cmake
      # Minimum CMake version
      cmake_minimum_required(VERSION 3.31)
      
      # Project name and version
      project(myprogram VERSION 1.0)
      
      # Set C++ standard
      set(CMAKE_CXX_STANDARD 		 17)
      set(CMAKE_CXX_STANDARD_REQUIRED YES)
      set(CMAKE_CXX_EXTENSIONS	 ON)
      
      # CMake Build Type (Debug/Release)
      set(CMAKE_BUILD_TYPE Release)
      
      # Specify the source files
      set(SOURCES
          src/main.cpp
          src/glad.c
      ) 
      
      # Include headers for FFmpeg and others
      include_directories(
          ${CMAKE_SOURCE_DIR}/include
          ${CMAKE_SOURCE_DIR}/include/freetype2
          ${CMAKE_SOURCE_DIR}/lib/GLFW
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg
          ${CMAKE_SOURCE_DIR}/lib/freetype
      )
      
      # Executable target
      add_executable(${PROJECT_NAME} ${SOURCES})
      
      # Define the source and destination directories for shaders
      set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/shaders)
      set(SHADER_DEST_DIR   ${CMAKE_BINARY_DIR}/shaders)
      
      # Create a custom command to copy shaders
      add_custom_command(
          TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR}
          COMMENT "Copying /shaders/ to build directory"
      )
      
      # Define the source and destination directories for textures
      set(TEXTURE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/textures)
      set(TEXTURE_DEST_DIR   ${CMAKE_BINARY_DIR}/textures)
      
      # Create a custom command to copy textures
      add_custom_command(
          TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
              ${TEXTURE_SOURCE_DIR} ${TEXTURE_DEST_DIR}
          COMMENT "Copying /textures/ to build directory"
      )
      
      # Add the ImGui library
      add_library(imgui STATIC
          third_party/imgui/imgui.cpp 
          third_party/imgui/imgui_draw.cpp 
          third_party/imgui/imgui_tables.cpp 
          third_party/imgui/imgui_widgets.cpp
          third_party/imgui/imgui_impl_opengl3.cpp
          third_party/imgui/imgui_impl_glfw.cpp
          third_party/imgui/imgui_demo.cpp
      )
      
      # Set the target properties for imgui (where libimgui.a will be created)
      set_target_properties(imgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/imgui)
      
      # Add the ImPlot library
      add_library(implot STATIC 
          third_party/implot/implot.cpp
          third_party/implot/implot_items.cpp
          third_party/implot/implot_demo.cpp
      )
      
      # Set the target properties for implot (where libimplot.a will be created)
      set_target_properties(implot PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/implot)
      
      # Include headers
      target_include_directories(imgui PUBLIC 
          ${CMAKE_SOURCE_DIR}/include/imgui
      )
      target_include_directories(implot PUBLIC
          ${CMAKE_SOURCE_DIR}/include/implot
      )
      
      # Link ImPlot to ImGui
      target_link_libraries(implot PUBLIC imgui)  # ImPlot depends on ImGui
      
      # Link all the found libraries
      target_link_libraries(${PROJECT_NAME}
          imgui  # ImGui static library from CMake
          implot # ImPlot static library from CMake
      
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavformat.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavcodec.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libavutil.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libswscale.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libswresample.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libpthread.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libm.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libws2_32.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libiconv.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/liblzma.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libole32.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libbcrypt.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libuuid.a
          ${CMAKE_SOURCE_DIR}/lib/ffmpeg/libstrmiids.a
      
          ${CMAKE_SOURCE_DIR}/lib/GLFW/libglfw3.a
          
          ${CMAKE_SOURCE_DIR}/lib/freetype/libfreetype.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/libharfbuzz.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/libgraphite2.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/libpng.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/libz.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/libbz2.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/libbrotlidec.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/libbrotlienc.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/libbrotlicommon.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/librpcrt4.a
          ${CMAKE_SOURCE_DIR}/lib/freetype/libgdi32.a
      
          ${CMAKE_SOURCE_DIR}/lib/libopengl32.a  # OpenGL
      )
          
      
      # Set output directory for the executable
      set_target_properties(${PROJECT_NAME} PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
      )
      ```
</details>

<details><summary> Encoder namespace </summary>

  ### Encoder namespace
  * Moved FFmpeg encoder functions from ``main.cpp`` to a separate ``encoder.cpp`` file, wrapping them in a namespace, and accessing them in ``main.cpp`` via the header ``encoder.h``.

  * Here is ``encoder.cpp``, you can notice I use an anonymous namespace to encapsulate state information, I might decide to change this from a namespace to a class in the future.

    ```cpp
    #include <learnopengl/encoder.h>
    #include <iostream>                 // for std::cin/cout/cerr
    
    // FFmpeg
    // ------
    extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/opt.h>
    #include <libswscale/swscale.h>
    #include <libavutil/log.h>
    }
    
    namespace Encoder {
    
        namespace {     // anonymous namespace (encapsulation)
    
            // FFmpeg
            // ------
            AVFormatContext* formatCtx = nullptr;
            AVCodecContext* codecCtx = nullptr;
            AVStream* videoStream = nullptr;
            SwsContext* swsCtx = nullptr;
            AVFrame* frameX = nullptr;
            AVPacket pkt = {};  // Zero-initialize the struct
            // std::ofstream logFile;
            // std::mutex logMutex;
        }
    
        // Function to initialize FFmpeg encoder
        bool initializeEncoder(const char* filename) {
            // Set up ffmpeg's log callback
            // av_log_set_callback(custom_ffmpeg_log_callback);
    
            // Optional: Set log level (AV_LOG_DEBUG=full logs, AV_LOG_INFO=default, AV_LOG_WARNING=only warnings, AV_LOG_ERROR=only errors)
            // av_log_set_level(AV_LOG_INFO);
    
            avformat_alloc_output_context2(&formatCtx, nullptr, "mp4", filename);
            if (!formatCtx) {
                printf("Could not create output context\n");
                return false;
            }
    
            // Find the H.264 encoder
            const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
            if (!codec) {
                printf("H.264 encoder not found\n");
                return false;
            }
    
            // Create a new stream
            videoStream = avformat_new_stream(formatCtx, codec);
            if (!videoStream) {
                printf("Could not create video stream\n");
                return false;
            }
    
            // Set up the codec context
            int FPS = static_cast<int>(framerate);
            codecCtx = avcodec_alloc_context3(codec);
            codecCtx->width = SCR_WIDTH;
            codecCtx->height = SCR_HEIGHT;
            codecCtx->time_base = (AVRational){1, FPS*1000};  // Frame rate
            codecCtx->framerate = (AVRational){FPS, 1};
            codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
            codecCtx->bit_rate = 60000000;  // 60 Mbps
            codecCtx->gop_size = 10;
            codecCtx->max_b_frames = 1;
            // codecCtx->rc_max_rate = codecCtx->bit_rate;  // Set maximum bitrate limit
            // codecCtx->rc_buffer_size = codecCtx->bit_rate;  // Set buffer size to match bitrate
    
            // Set H.264 options
            av_opt_set(codecCtx->priv_data, "preset", "slow", 0);
            av_opt_set(codecCtx->priv_data, "crf", "18", 0);
            // av_opt_set(codecCtx->priv_data, "bitrate", "30000k", 0);   // Set bitrate to 5000 kbps
            // av_opt_set(codecCtx->priv_data, "maxrate", "30000k", 0);   // Max bitrate
            // av_opt_set(codecCtx->priv_data, "bufsize", "30000k", 0);   // Buffer size
            // av_opt_set(codecCtx->priv_data, "profile", "high", 0);     // Set profile to "high" for better quality
            av_opt_set(codecCtx->priv_data, "pix_fmt", "yuv420p", 0);  // Try using yuv422p or yuv444p for better quality
    
    
            // Open the encoder
            if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
                printf("Could not open codec\n");
                return false;
            }
    
            // Assign codec to stream
            videoStream->codecpar->codec_id = AV_CODEC_ID_H264;
            avcodec_parameters_from_context(videoStream->codecpar, codecCtx);
    
            // Make sure that your AVStream has the same time_base
            videoStream->time_base = codecCtx->time_base;
            // Force the output stream to interpret it as CFR (Constant Frame Rate)
            // videoStream->avg_frame_rate = (AVRational){60, 1};  // Force the output to exactly 60 FPS
            // av_dict_set(&formatCtx->metadata, "video-cfr", "1", 0);
    
            // Open output file
            if (!(formatCtx->oformat->flags & AVFMT_NOFILE)) {
                if (avio_open(&formatCtx->pb, filename, AVIO_FLAG_WRITE) < 0) {
                    printf("Could not open output file\n");
                    return false;
                }
            }
    
            // Write header
            if (avformat_write_header(formatCtx, nullptr) < 0) {
                std::cerr << "Error writing header to output file!" << std::endl;
                return false;  // Handle failure
            }
    
            // Allocate frame and conversion context
            frameX = av_frame_alloc();
            frameX->format = AV_PIX_FMT_YUV420P;
            frameX->width = SCR_WIDTH;
            frameX->height = SCR_HEIGHT;
            av_frame_get_buffer(frameX, 32);
    
            swsCtx = sws_getContext(SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_RGB24,
                                    SCR_WIDTH, SCR_HEIGHT, AV_PIX_FMT_YUV420P,
                                    SWS_BILINEAR, nullptr, nullptr, nullptr);
            return true;
        }
    
        // Encode frame using FFmpeg
        bool encodeFrame(const uint8_t* rgbData, float crntTime) {
            // Convert RGB to YUV420P
            uint8_t* inData[1] = {(uint8_t*)rgbData};  // Input RGB data
            int inLinesize[1] = {3 * SCR_WIDTH};          // Input stride
            sws_scale(swsCtx, inData, inLinesize, 0, SCR_HEIGHT, frameX->data, frameX->linesize);
    
            frameX->pts = static_cast<int64_t>(crntTime * 60 * 1000); //* AV_TIME_BASE);
            // frameX->pts = 1000 * frameIndex; // Simple frame count to force 60 FPS
            // frameIndex++;
            
            // Encode the frame
            if (avcodec_send_frame(codecCtx, frameX) < 0) {
                printf("Error sending frame to encoder\n");
                return false;
            }
    
            // Retrieve the encoded packet
            pkt.data = nullptr;
            pkt.size = 0;
            // pkt.dts = (frameX->pts < pkt.dts) ? pkt.dts + 1 : frameX->pts;
            pkt.dts = frameX->pts;
    
            while (avcodec_receive_packet(codecCtx, &pkt) == 0) {
                pkt.stream_index = videoStream->index;
                av_interleaved_write_frame(formatCtx, &pkt);
                av_packet_unref(&pkt);
            }
    
            return true;
        }
    
        // Finalize encoding and close files
        void finalizeEncoder() {
            avcodec_send_frame(codecCtx, nullptr);
            while (avcodec_receive_packet(codecCtx, &pkt) == 0) {
                pkt.stream_index = videoStream->index;
                av_interleaved_write_frame(formatCtx, &pkt);
                av_packet_unref(&pkt);
            }
    
            // Close log file at the end
            // if (logFile.is_open())
            // {
            //     logFile.close();
            // }
    
            av_write_trailer(formatCtx);
            avcodec_free_context(&codecCtx);
            avformat_free_context(formatCtx);
            sws_freeContext(swsCtx);
            av_frame_free(&frameX);
        }
    
    }
    ```

  * And here is ``encoder.h``:

    ```cpp
    #ifndef ENCODER_H    // #include guard
    #define ENCODER_H
    
    #include <stdint.h>     // for uint8_t
    
    extern const unsigned int  SCR_WIDTH;
    extern const unsigned int  SCR_HEIGHT;
    extern       unsigned int  framerate;
    
    namespace Encoder {
        bool initializeEncoder(const char* filename);
        bool encodeFrame(const uint8_t* rgbData, float crntTime);
        void finalizeEncoder();
    }
    
    #endif /* ENCODER_H */
    ```

  * In order to compile and link the separate ``encoder.cpp`` file and ``encoder.h`` header, add this to ``CMakeLists.txt``:

    ```cmake
    # Add the FFmpeg function library
    add_library(encoder STATIC
        src/encoder.cpp
    )
    # Set the target properties for encoder (where libencoder.a will be created)
    set_target_properties(encoder PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)

    # Link all the found libraries
    target_link_libraries(${PROJECT_NAME}
        encoder
        ...
    )
    ```
</details>

## Progress update 11 - Updated text rendering with Font, FontManager, and TextRenderer classes - 09/04/25

<details><summary> Progress update 11 - Updated text rendering with Font, FontManager, and TextRenderer classes - 09/04/25 </summary>

  * Delaying implementing **Lighting** yet again.
  * Currently I am loading fonts, creating a texture atlas, and rendering the text all inside ``text.h`` using the ``Text`` class. This is not optimal as it means each text instance has its own texture atlas, even though multiple instances may share the same font and thus could share the same texture atlas. A better design would be to have a ``Font`` class which handles loading the font and creating a texture atlas for it, a ``FontManager`` class (only one instance needed) which manages all the fonts created and could act as the interface, and a ``TextRenderer`` class (also only one instance needed) which can take the ``FontManager`` class as an input and act as an interface but for rendering text and texture atlases.

    <details><summary> font.cpp and font.h </summary>
      
      * Here is ``font.cpp``:
    
        ```cpp
        #include <glad/glad.h>
        #include <learnopengl/font.h>
        #include <iostream>
        #include <iomanip>
        #include <map>
        #include <vector>
        #include <cstring>
        
        Font::Font(const std::string& name, int size) {
            loadFont(name, size);
            createTextureAtlas();
        
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }
        
        Font::~Font() {
            glDeleteTextures(1, &textureAtlasID);
        }
        
        void Font::loadFont(const std::string& name, int size) {
            std::cout << "[Font] Constructor called for " << name << ", size " << size << "\n";
            std::cout << "Loading font...\n";
        
            // LOAD FONT
            // ---------
            if (FT_Init_FreeType(&ft)) {
                std::cerr << "Could not initialize FreeType Library" << std::endl;
                return;
            }
            std::string fontPath = "C:/WINDOWS/FONTS/" + name + ".TTF";
            if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
                std::cerr << "Failed to load font face from: " << fontPath << std::endl;
                return;
            }
            if (!face) {
                std::cerr << "Failed to load the font face. Ensure the file path is correct." << std::endl;
                return;
            }
            if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
                std::cerr << "Failed to set Unicode character map." << std::endl;
                return;
            }
            // Set the pixel size for glyphs
            if (FT_Set_Pixel_Sizes(face, 0, size)) {
                std::cerr << "ERROR::FREETYPE: Failed to set pixel size." << std::endl;
                return;
            }
            else {
                std::cout << "FreeType successfully loaded font!" << std::endl;
                // disable byte-alignment restriction
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                return;
            }
        }
        
        void Font::createTextureAtlas() {
            // CREATE TEXTURE ATLAS
            // --------------------
            std::cout << "Creating texture atlas..." << std::endl;
        
            // Calculate texture atlas size (simplified)
            atlasWidth = 512;
            atlasHeight = 512;
        
            // Variables for positioning glyphs in the atlas
            int offsetX         = 0;
            int offsetY         = 0;
            int padding         = 1;
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
                std::cerr << ":D OpenGL Error after glTexImage2D: " << error << std::endl;
                return;
            }
            // Set texture filtering and wrapping
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // Iterate over all printable ASCII characters
            for (unsigned char c = 32; c < 127; ++c) {
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
                if (offsetX + g->bitmap.width > atlasWidth) {
                    std::cerr << "REACHED ATLAS WIDTH LIMIT. STARTING NEW ROW. " << offsetX << " + " << g->bitmap.rows << " = " << offsetX + static_cast<int>(g->bitmap.width) << " >= " << atlasWidth << std::endl;
                    maxWidth = std::max(maxWidth, offsetX);
                    offsetX = 0;
                    offsetY += rowHeight + padding;
                    rowHeight = 0;
                }
                // Check if character doesn't fit in the atlas
                if (offsetY + g->bitmap.rows > atlasHeight) {
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
                // float scale = fontSize / (float) face->units_per_EM;
                // float ascenderPx = face->ascender * scale;
                // float descenderPx = face->descender * scale;
        
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
                // glyphs[c].offsetY = (ascenderPx - g->bitmap_top);// - (g->bitmap.rows - descenderPx);
                // maxAscent = int(face->ascender * (face->size->metrics.y_scale / 65536.0)) >> 6;
                // maxDescent = int(abs(face->descender * (face->size->metrics.y_scale / 65536.0))) >> 6;
                totalglyphArea += static_cast<int>(g->bitmap.width) * static_cast<int>(g->bitmap.rows);
                offsetX += g->bitmap.width + padding;
                rowHeight = std::max(rowHeight, static_cast<int>(g->bitmap.rows));
        
                // std::cout << "Loaded character: " << c << " (" << static_cast<int>(c) << ") Asc: " << ascenderPx << " Desc: " << descenderPx << std::endl;
                // std::cout << "Successfully loaded glyph: " << c << std::endl;
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
            for (const auto& [key, glyph] : glyphs) {
                std::cout.precision(5);
                // std::cout << std::fixed;
                std::cout << " | " << "Glyph: "                     << static_cast<char>(key)   << " | "
                                   << "TextureX: "  << std::setw(9) << glyph.textureX           << " | "
                                   << "TextureY: "  << std::setw(9) << glyph.textureY           << " | "
                                   << "Width: "     << std::setw(2) << glyph.width              << " | "
                                   << "Height: "    << std::setw(2) << glyph.height             << " | "
                                   << "OffsetX: "   << std::setw(2) << glyph.offsetX            << " | "
                                   << "OffsetY: "   << std::setw(2) << glyph.offsetY            << " | "
                                   << "AdvanceX: "  << std::setw(5) << (glyph.advanceX >> 6)    << " | "
                                   << std::endl;
            }
        }
        
        const Glyph& Font::getGlyph(char c) const {
            auto it = glyphs.find(c);
            if (it != glyphs.end()) {
                return it->second;
            }
        
            static Glyph fallback {};  // a default zero glyph
            std::cerr << "Warning: Glyph not found for character '" << c << "' (" << static_cast<int>(c) << ")" << std::endl;
            return fallback;
        }
        ```
    
      * And ``font.h``:
    
        ```cpp
        #ifndef FONT_H
        #define FONT_H
        
        #include <map>
        #include <string>
        #include <glad/glad.h>
        #include <glm/glm.hpp>
        #include <ft2build.h>
        #include FT_FREETYPE_H
        
        struct Glyph {
            float textureX, textureY; // Position in texture atlas
            float width, height;      // Glyph size
            float offsetX, offsetY;   // Offset from baseline
            unsigned int advanceX;    // Advance to next character
        };
        
        class Font {
        private:
            std::map<char, Glyph> glyphs;
            GLuint textureAtlasID;
            int atlasWidth, atlasHeight;
            FT_Library ft;
            FT_Face face;
        
            void loadFont(const std::string& name, int size);
            void createTextureAtlas();
        
        public:
            // constructor and destructor
            Font(const std::string& name, int size);
            ~Font();
        
            // getters
            const Glyph& getGlyph(char c) const;
            GLuint getTextureAtlas() const { return textureAtlasID; }
            int getAtlasWidth() const { return atlasWidth; }
            int getAtlasHeight() const { return atlasHeight; }
        };
        
        #endif // FONT_H
        ```
    </details>

    <details><summary> fontmanager.cpp and fontmanager.h </summary>

      * Here is ``fontmanager.cpp``:
      
        ```cpp
        #include <learnopengl/fontmanager.h>
        #include <learnopengl/shader_s.h>
        #include <iostream>
        
        FontManager::~FontManager() {
            for (auto& pair : fonts) {
                delete pair.second;
            }
        }
        
        bool FontManager::loadFont(const std::string& name, int size) {
            if (fonts.find(name) != fonts.end()) {
                std::cerr << "Font " << name << " already loaded.\n";
                return false;
            }
        
            fonts[name] = new Font(name, size);
            std::cout << "New font created: " << name << ", size " << size << std::endl;
        
            return true;
        }
        
        Font* FontManager::getFont(const std::string& name) {
            if (fonts.find(name) != fonts.end()) {
                // std::cout << "Font found: " << name << std::endl;
                return fonts[name];
            }
            return nullptr;
        }
        ```
    
      * And ``fontmanager.h``:
    
        ```cpp
        #ifndef FONTMANAGER_H
        #define FONTMANAGER_H
        
        #include <learnopengl/font.h>
        #include <map>
        
        class FontManager {
        private:
            std::map<std::string, Font*> fonts;
        public:
            ~FontManager();
            
            bool loadFont(const std::string& name, int size);
            Font* getFont(const std::string& name);
        };
        
        #endif // FONTMANAGER_H
        ```
    </details>

    <details><summary> textrenderer.cpp and textrenderer.h </summary>
    
      * And here is ``textrenderer.cpp``:
      
        ```cpp
        #include <glad/glad.h>
        #include <learnopengl/textrenderer.h>
        #include <learnopengl/shader_s.h>
        #include <iostream>
        
        TextRenderer::TextRenderer(FontManager &fm)
            : fontManager(fm),
              textShader("text_shader.vert", "text_shader.frag"),
              atlasShader("atlas.vert", "atlas.frag")
        {
            // set up vertex data (and buffer(s)) and configure vertex attributes
            glGenVertexArrays(1, &textVAO);
            glGenBuffers(1, &textVBO);
        
            glBindVertexArray(textVAO);
            
            glBindBuffer(GL_ARRAY_BUFFER, textVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            
            // Position and texture attribute
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glEnableVertexAttribArray(0);
        
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
        }
        
        TextRenderer::~TextRenderer()
        {
            textShader.Delete();
            atlasShader.Delete();
            glDeleteVertexArrays(1, &textVAO);
            glDeleteVertexArrays(1, &quadVAO);
            glDeleteBuffers(1, &textVBO);
            glDeleteBuffers(1, &quadVBO);
        }
        
        void TextRenderer::renderText(const std::string &text, float x, float y, float scale, glm::vec3 color, const std::string &fontName) {
            Font *font = fontManager.getFont(fontName);
            if (!font) {
                std::cerr << "Font " << fontName << " not found.\n";
                return;
            }
        
            int atlasWidth = font->getAtlasWidth();
            int atlasHeight = font->getAtlasHeight();
        
            // Use text rendering shader
            textShader.use();
            textShader.setVec3("textColor", color);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas());
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
                // Skip characters that do not exist in the glyph map
                // if (glyphs.find(c) == glyphs.end()) {
                //     continue;
                // }
        
                // Retrieve glyph
                const Glyph &glyph = font->getGlyph(c);
        
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
                // positions          // texture coords
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
                x += (glyph.advanceX >> 6) * scale; // Advance in pixels (1/64th units) to next character
            }
        
            // Cleanup
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
        void TextRenderer::renderAtlas(const std::string &fontName) {
            Font *font = fontManager.getFont(fontName);
            if (!font) {
                std::cerr << "Font " << fontName << " not found.\n";
                return;
            }
        
            atlasShader.use();
            // Enable 2D rendering
            glEnable(GL_BLEND); // enable transparency
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // bind VAO
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        
            // Bind the texture (the texture atlas in this case)
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas()); // atlasTextureId is the texture containing the atlas
            atlasShader.setInt("screenTexture", 0);
            glm::vec3 color(1.0f, 1.0f, 1.0f); // White text
            atlasShader.setVec3("textColor", color);
        
            // Draw the full-screen triangle
            glDrawArrays(GL_TRIANGLES, 0, 3);
        
            // Cleanup
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        ```
    
      * And ``textrenderer.h``:
    
        ```cpp
        #ifndef TEXTRENDERER_H
        #define TEXTRENDERER_H
        
        #include <glad/glad.h>
        #include <learnopengl/fontmanager.h>
        #include <learnopengl/shader_s.h>
        
        extern const unsigned int  SCR_WIDTH;
        extern const unsigned int  SCR_HEIGHT;
        
        class TextRenderer {
        private:
            FontManager& fontManager;
            GLuint textVAO, textVBO;
            GLuint quadVAO, quadVBO;
            Shader textShader;
            Shader atlasShader;
        public:
            TextRenderer(FontManager& fm);
            ~TextRenderer();
            
            void renderText(const std::string& text, float x, float y, float scale, glm::vec3 color, const std::string& fontName);
            void renderAtlas(const std::string &fontName);
        };
        
        #endif // TEXTRENDERER_H
        ```
    </details>

  * And here is what is added to ``CMakeLists.txt``:

    ```cmake
    # Add the custom text function library
    add_library(text STATIC
        src/font.cpp
        src/fontmanager.cpp
        src/textrenderer.cpp
    )
    # Set the target properties for text (where libtext.a will be created)
    set_target_properties(text PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)

    # Link all the found libraries
    target_link_libraries(${PROJECT_NAME}
        text
        ...
    )
    ```

  * Inside ``main.cpp``, the code to render text looks like this:

    Include headers
    ```cpp
    #include <learnopengl/fontmanager.h>    // Custom Font Manager
    #include <learnopengl/textrenderer.h>   // Custom Text Renderer
    ```

    Before the render loop
    ```cpp
    FT_UInt fontSize = 48;
    // Render FPS text at the top-left corner
    float scale = static_cast<float>(SCR_WIDTH)*0.3f/800.0f;
    glm::vec3 color(1.0f, 1.0f, 1.0f); // White text
    FontManager fontManager;
    fontManager.loadFont("ARIAL", 48);
    TextRenderer textRenderer(fontManager);
    ```

    Inside the render loop
    ```cpp
    textRenderer.renderText(fpsText, x, y, scale, color, "ARIAL");
    textRenderer.renderAtlas("ARIAL");
    ```
    
</details>

## Progress update 12 - Phong lighting, render text in single draw call, and Linux compatibility - 25/04/25

<details><summary> Phong lighting </summary>

  ### Phong lighting
  
  * Finally implementing **Lighting**. Phong lighting is a method of implementing lighting in the fragment shader. Phong lighting uses 3 steps: ambient, diffuse, and specular lighting. For a better explanation see [learnopengl.com](https://learnopengl.com/Lighting/Basic-Lighting).

    [![Phong_components_version_4](https://github.com/user-attachments/assets/310edfe3-8a8a-4530-a1c7-aa9b26f60a65)](https://en.wikipedia.org/wiki/Phong_reflection_model)

  * Ambient lighting is the simplest (independent of light objects):

    ```cpp
    void main()
    {
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
    
        vec3 result = ambient * objectColor;
        FragColor = vec4(result, 1.0);
    }  
    ```

    ![ambient_lighting](https://github.com/user-attachments/assets/440c520a-4309-4f8b-8bf1-321cfa261227)

  * Diffuse lighting is what produces directional lighting from light objects, for this we need to update the vertex data array of the cube to contain the normal vectors for each face. And specular lighting is what produces the glints of light you'd get when the light bounces into the camera, for that we need the light position, camera position and view direction.

    Updated vertex data array:

    ```cpp
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // 3D cube without EBO (6 faces * 2 triangles * 3 vertices each = 36 vertices. 8 unique vertices. 78% redundancy) (each face has its own unique texture coords)
    //      3------2
    //     /|  Y  /|
    //    7------6 | X
    //    | 0----|-1
    //    |/ Z   |/
    //    4------5
    float vertices[] = {
      // positions            normal              texture coords
        // -Z face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,   // (-,-,-) 0
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,   // (+,-,-) 1
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,   // (+,+,-) 2
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,   // (+,+,-) 2
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,   // (-,+,-) 3
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,   // (-,-,-) 0

        // +Z face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,   // (-,-,+) 4
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,   // (+,-,+) 5
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,   // (+,+,+) 6
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,   // (+,+,+) 6
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,   // (-,+,+) 7
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,   // (-,-,+) 4

        // -X face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   // (-,+,+) 7
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,   // (-,+,-) 3
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   // (-,-,-) 0
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   // (-,-,-) 0
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,   // (-,-,+) 4
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   // (-,+,+) 7

        // +X face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   // (+,+,+) 6
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,   // (+,+,-) 2
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   // (+,-,-) 1
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   // (+,-,-) 1
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,   // (+,-,+) 5
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   // (+,+,+) 6

        // -Y face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,   // (-,-,-) 0
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,   // (+,-,-) 1
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,   // (+,-,+) 5
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,   // (+,-,+) 5
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,   // (-,-,+) 4
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,   // (-,-,-) 0

        // +Y face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,   // (-,+,-) 3
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,   // (+,+,-) 2
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,   // (+,+,+) 6
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,   // (+,+,+) 6
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,   // (-,+,+) 7
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f    // (-,+,-) 3
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // once a VAO is bound, any subsequent vertex attribute calls will be stored inside that VAO. (only have to make the calls once)
    glBindVertexArray(VAO);

    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    ```

    In the vertex shader:

    ```glsl
    #version 430 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoord;
    out vec3 FragPos;
    out vec3 Normal;

    out vec2 TexCoord;

    uniform mat4 transform;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    void main()
    {
    	gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
    
    	FragPos = vec3(model * transform * vec4(aPos, 1.0));
    	Normal = mat3(model) * aNormal;
    	// Normal = aNormal;
    	// Normal = mat3(transpose(inverse(model))) * aNormal;  // inversing matrices is a costly operation
    }
    ```

    In the fragment shader:

    ```glsl
    #version 430 core
    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoord;

    out vec4 FragColor;
    
    uniform float mixValue;
    
    uniform vec3 lightPos;
    uniform vec3 lightColor;
    uniform vec3 objectColor;
    uniform vec3 viewPos;
    
    void main()
    {
    	// Ambient lighting
    	float ambientStrength = 0.1;
    	vec3 ambient = ambientStrength * lightColor;
    
    	// Diffuse lighting
    	vec3 norm = normalize(Normal);
    	vec3 lightDir = normalize(lightPos - FragPos);
    	float diff = max(dot(norm, lightDir), 0.0);
    	vec3 diffuse = diff * lightColor;
    
      // Specular
      float specularStrength = 1.0;
      vec3 viewDir = normalize(viewPos - FragPos);
      vec3 reflectDir = reflect(-lightDir, norm);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
      vec3 specular = specularStrength * spec * lightColor;
    
    	vec3 result = (ambient + diffuse + specular) * objectColor;
    
    	FragColor = mix(texture(texture1, vec2(TexCoord.x, TexCoord.y)),
    					        texture(texture2, vec2(TexCoord.x, TexCoord.y)),
    					        mixValue)
    				      * vec4(result, 1.0);
    }
    ```
</details>

<details><summary> Render text in single draw call </summary>

  * Batched vertex data for all characters in text and used just one draw call. Went from ~2ms → ~0.2ms, 10x improvement, but doesn't correlate to an actual 10x improvement in FPS as 2ms is quite quick anyway, but when using a slower machine like my laptop, those time savings are very welcome.

    The old way:
    
    ```cpp
    // multiple draw calls for each character in text
    void TextRenderer::renderText(const std::string &text, float x, float y, float scale, glm::vec3 color, const std::string &fontName) {
        Font *font = fontManager.getFont(fontName);
        if (!font) {
            std::cerr << "Font " << fontName << " not found.\n";
            return;
        }
    
        int atlasWidth = font->getAtlasWidth();
        int atlasHeight = font->getAtlasHeight();
    
        // Use text rendering shader
        textShader.use();
        textShader.setVec3("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas());
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
            // Skip characters that do not exist in the glyph map
            // if (glyphs.find(c) == glyphs.end()) {
            //     continue;
            // }
    
            // Retrieve glyph
            const Glyph &glyph = font->getGlyph(c);
    
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
            // positions          // texture coords
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
            x += (glyph.advanceX >> 6) * scale; // Advance in pixels (1/64th units) to next character
        }
    
        // Cleanup
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    ```

    The new way:

    ```cpp
    // single draw call (~10x faster)
    void TextRenderer::renderTextFast(const std::string &text, float x, float y, float scale, glm::vec3 color, const std::string &fontName) {
        Font *font = fontManager.getFont(fontName);
        if (!font) {
            std::cerr << "Font " << fontName << " not found.\n";
            return;
        }
    
        int atlasWidth = font->getAtlasWidth();
        int atlasHeight = font->getAtlasHeight();
    
        // Use text rendering shader
        textShader.use();
        textShader.setVec3("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas());
        textShader.setInt("textTextureAtlas", 0);
    
        // Enable 2D rendering
        glEnable(GL_BLEND); // enable transparency
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        // Set up the transformation matrix for the text position
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT)); // Orthogonal projection for 2D rendering
        textShader.setMat4("projection", projection);
    
        std::vector<float> vertexData;
        vertexData.reserve(text.size() * 6 * 4);
    
        glBindVertexArray(textVAO);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    
        // Iterate through characters
        for (const char &c : text) {
            // Skip characters that do not exist in the glyph map
            // if (glyphs.find(c) == glyphs.end()) {
            //     continue;
            // }
    
            // Retrieve glyph
            const Glyph &glyph = font->getGlyph(c);
    
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
    
            float quad[6][4] = {
            // positions          // texture coords
                { xpos,     ypos + h,   tx,      ty + th }, // Top-left
                { xpos,     ypos,       tx,      ty      }, // Bottom-left
                { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right
    
                { xpos,     ypos + h,   tx,      ty + th }, // Top-left
                { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right
                { xpos + w, ypos + h,   tx + tw, ty + th }  // Top-right
            };
    
            for (auto &v : quad)
                vertexData.insert(vertexData.end(), v, v + 4);
    
            // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);            // ideal for small subset updates
            // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // better for reallocating and initializing large buffers
    
            // Render quad
            // glDrawArrays(GL_TRIANGLES, 0, 6);
    
            // Advance cursor
            x += (glyph.advanceX >> 6) * scale; // Advance in pixels (1/64th units) to next character
        }
    
        // Upload vertex buffer data in one call
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW);
    
        // Draw all characters with a single draw call
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexData.size() / 4));
    
        // Cleanup
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    ```

  * The main takeaway is that instead of calling ``glDrawArrays(GL_TRIANGLES, 0, 6)`` for each character, I am batching the vertex data of all the characters using:
   
    ```cpp
    std::vector<float> vertexData;
    vertexData.reserve(text.size() * 6 * 4);

    // Iterate through characters
    for (const char &c : text) {
        ...
        float quad[6][4] = {
        // positions          // texture coords
            { xpos,     ypos + h,   tx,      ty + th }, // Top-left
            { xpos,     ypos,       tx,      ty      }, // Bottom-left
            { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right

            { xpos,     ypos + h,   tx,      ty + th }, // Top-left
            { xpos + w, ypos,       tx + tw, ty      }, // Bottom-right
            { xpos + w, ypos + h,   tx + tw, ty + th }  // Top-right
        };

        for (auto &v : quad)
            vertexData.insert(vertexData.end(), v, v + 4);
        ...
    }

    // Upload vertex buffer data in one call
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW);

    // Draw all characters with a single draw call
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexData.size() / 4));
    ```

</details>

<details><summary> Linux compatibility </summary>

  ### Linux compatibility

  * Made the program build in Linux, might as well as OpenGL GLFW was designed to be cross-compatible. I can't use the static libraries I precompiled on Windows like FFmpeg and FreeType, so instead I set these as dependencies that CMake will check for. This is what was added to ``CMakeLists.txt``, I found that copying the shader files from project root to ``/build/`` is simpler in Linux with ``rsync`` compared to ``robocopy`` in Windows.

    ```cmake
    # Shader copy target
    if(UNIX)
        add_custom_target(sync_assets ALL
            COMMAND rsync -a --delete ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
            COMMAND rsync -a --delete ${SHADERS_SOURCE_DIR} ${SHADERS_DEST_DIR}
            COMMENT "Syncing assets and shaders using rsync"
        )
    endif(UNIX)

    if(UNIX)
        find_package(OpenGL REQUIRED)
        find_package(glfw3 REQUIRED)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
            libavcodec
            libavformat
            libavutil
            libswscale
        )
        pkg_check_modules(FREETYPE2 REQUIRED IMPORTED_TARGET
            freetype2
        )
        target_link_libraries(${PROJECT_NAME}
            config # don't even need to link as config already linked to encoder and text, but no harm in including again
            imgui  # ImGui static library from CMake (don't even need to link as ImPlot already linked to ImGui, but no harm in including again)
            implot # ImPlot static library from CMake
            encoder
            text
            OpenGL::GL
            glfw
            PkgConfig::FFMPEG
            PkgConfig::FREETYPE2
        )
    endif(UNIX)
    ```
</details>

## Progress update 13 - Multithreading (5x performance improvement), custom Timer namespace, Flip shader, and smooth button toggling - 10/05/25

<details><summary> Multithreading (5x performance improvement) </summary>
  
  * Added an encoder thread that works concurrently with the main (render) thread and a shared Frame queue. Can toggle ON and OFF by pressing <kbd>P</kbd>. With CPU: i7 4790 and GPU: RX550, went from ~100FPS->500FPS. Used std::thread, std::mutex, std::queue, std::condition_variable, std::atomic, std::unique_lock, and std::lock_guard.
  
    Include headers
    ```cpp
    #include <thread>
    #include <mutex>
    #include <queue>
    #include <condition_variable>
    #include <atomic>
    ```
  
    Encoder thread with a [lambda expression](https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-170) that handles the encoding process
    ```cpp
    // FrameData holds a copy of the frame and its timestamp
    struct FrameData {
        // std::vector<uint8_t> pixels;  // Make a deep copy of PBO data
        unsigned char *frame;
        double pts;
        GLuint pboIndex;
    };
  
    // A lock-free queue is overkill here — simple mutex + condition_variable works well
    std::queue<FrameData> frameQueue;
    std::mutex queueMutex;
    std::condition_variable queueCond;
    std::atomic<bool> recordingNew = true;
    const size_t MAX_QUEUE_SIZE = 8;
  
    // std::mutex encoderMutex;
    std::mutex coutMutex;
    Timer::init();
    std::chrono::high_resolution_clock::time_point t;
  
    std::thread encoderThread([&]() {
        glfwMakeContextCurrent(sharedContextWindow);  // Make encoder's context current here
        gladLoadGL(); // Needed again in this thread!
        try {
            // Step 1: Initialize encoder
            if (!Encoder::initializeEncoder("../output/output.mp4")) {
                std::cerr << "Failed to initialize encoder in thread\n";
                return;
            }
    
            // Step 2: Process frame queue
            while (recordingNew || !frameQueue.empty()) {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCond.wait(lock, [&]() {
                    return !frameQueue.empty() || !recordingNew;
                });
    
                while (!frameQueue.empty()) {
                    FrameData frame = std::move(frameQueue.front());
                    frameQueue.pop();
                    
                    if (!pbo) {
                        lock.unlock();  // unlock while processing
                        if (!flip_shader) {
                            Timer::startTimer(t);
                            flipFrameVertically(frame.frame);
                            {
                                std::lock_guard<std::mutex> lock(coutMutex);
                                Timer::endTimer(Timer::FLIP_FUNCTION, t);
                            }
                        }
                        // lock.lock(); // relock for logging
                        {
                            std::lock_guard<std::mutex> encoderLock(Encoder::encoderMutex);
                            Timer::startTimer(t);
                            // Encoder::encodeFrame(frame.pixels.data(), frame.pts);
                            Encoder::encodeFrame(frame.frame, frame.pts);
                            {
                                std::lock_guard<std::mutex> lock(coutMutex);
                                Timer::endTimer(Timer::ENCODE, t);
                            }
                        }
                        // lock.unlock();  // unlock
                        lock.lock(); // relock for queue
                    }
  
                    if (pbo) {
                        lock.unlock();  // unlock while processing
                        // Wait for sync to be ready
                        int index = frame.pboIndex;
                        if (pboFences[index]) {
                            // GLenum waitReturn = glClientWaitSync(pboFences[index], 0, GL_TIMEOUT_IGNORED);
                            GLenum waitReturn = glClientWaitSync(pboFences[index], GL_SYNC_FLUSH_COMMANDS_BIT, 1'000'000'000); // 1s timeout
                            if (waitReturn == GL_ALREADY_SIGNALED || waitReturn == GL_CONDITION_SATISFIED) {
                                glDeleteSync(pboFences[index]);
                                pboFences[index] = nullptr;
                            } else {
                                std::cerr << "[encoderThread] glClientWaitSync timeout or error\n";
                                continue;
                            }
                        }
  
                        // Map and copy pixels from PBO
                        glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
                        GLubyte* ptr = (GLubyte*)glMapBufferRange(
                            GL_PIXEL_PACK_BUFFER, 0, DATA_SIZE,
                            GL_MAP_READ_BIT
                        );
  
                        if (ptr) {
                            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
                            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  
                            if (!flip_shader) {
                                Timer::startTimer(t);
                                flipFrameVertically(ptr);
                                {
                                    std::lock_guard<std::mutex> lock(coutMutex);
                                    Timer::endTimer(Timer::FLIP_FUNCTION, t);
                                }
                            }
                            // lock.lock(); // relock for logging
                            {
                                std::lock_guard<std::mutex> encoderLock(Encoder::encoderMutex);
                                Timer::startTimer(t);
                                // Encoder::encodeFrame(frame.pixels.data(), frame.pts);
                                Encoder::encodeFrame(ptr, frame.pts);
                                {
                                    std::lock_guard<std::mutex> lock(coutMutex);
                                    Timer::endTimer(Timer::ENCODE, t);
                                }
                            }
                            // lock.unlock();  // unlock
                        }
                        lock.lock(); // relock for queue
                    }
                }
            }
  
            {
                std::lock_guard<std::mutex> encoderLock(Encoder::encoderMutex);
                Encoder::finalizeEncoder();
            }
        } catch (const std::exception& e) {
            std::cerr << "Encoder thread crashed: " << e.what() << std::endl;
        }
    });
    ```
</details>

<details><summary> Custom Timer namespace </summary>

  * Made a custom Timer namespace using std::chrono::high_resolution_clock, for debugging.
  * What I didn't realise at the time was that printing to the terminal the time taken of a hot function in the render loop every cycle makes the performance drop a LOT. I commented it out, but it may be useful to enable for debugging. Instead I just aggregate the timings and print the averages at the end of the program.

  * ``timer.cpp``:
    
      ```cpp
      #include <chrono>
      #include <array>
      #include <iostream>                     // for std::cin/cout/cerr
      #ifdef _WIN32
      #include <Windows.h>
      #endif /* _WIN32 */
      #include <learnopengl/timer.h>
      
      namespace Timer {
          namespace {     // anonymous namespace (encapsulation)
              constexpr int NUM_TIMERS = 14;
      
              const char* names[NUM_TIMERS] = {
                  "[encoder] encodeFrame",
                  "[main] Rendering scene",
                  "[main] Blitting MSAA to non-MSAA FBO",
                  "[main] Rendering text",
                  "[main] Rendering ImGui/ImPlot",
                  "[main] Blitting non-MSAA FBO to screen",
                  "[main] Flip shader",
                  "[encoder] flipFrameVertically",
                  "[main] glClientWaitSync",
                  "[main] glMapBufferRange",
                  "[main] glReadPixels (PBO ON)",
                  "[main] glReadPixels (PBO OFF)",
                  "[main] queue push+wait (PBO ON)",
                  "[main] queue push+wait (PBO OFF)"
              };
      
              std::array<double, NUM_TIMERS> totalTimes = {};
              std::array<double, NUM_TIMERS> minTimes = {};
              std::array<double, NUM_TIMERS> minTimeCount = {};
              std::array<double, NUM_TIMERS> maxTimes = {};
              std::array<double, NUM_TIMERS> maxTimeCount = {};
              std::array<int, NUM_TIMERS> counts = {};
          }
      
          void init() {
              minTimes.fill(1e6); // sets every value in the array to 1e6
          }
      
          void startTimer(std::chrono::high_resolution_clock::time_point& tp) {
              tp = std::chrono::high_resolution_clock::now();
          }
          
          void endTimer(TimerType type, const std::chrono::high_resolution_clock::time_point& start) {
              auto end = std::chrono::high_resolution_clock::now();
              std::chrono::duration<double, std::milli> elapsed = end - start;
              totalTimes[type] += elapsed.count();
              counts[type]++;
          
              constexpr int WARMUP_SAMPLES = 5; // avoid sampling at the beginning when encoding is slow
              if (counts[type] >= WARMUP_SAMPLES) {
                  if (elapsed.count() < minTimes[type]) {
                      minTimes[type] = elapsed.count();
                      minTimeCount[type] = counts[type];
                  }
                  if (elapsed.count() > maxTimes[type]) {
                      maxTimes[type] = elapsed.count();
                      maxTimeCount[type] = counts[type];
                  }   
              }
              // print timings every cycle (heavily drops fps and spams terminal, only for debugging)
              // if (type == ENCODE) {
              //     std::cout << names[type] << " took " << elapsed.count() << " ms\n";
              // }
          }
          
          void printAverages() {
              for (int i = 0; i < NUM_TIMERS; ++i) {
                  if (counts[i] > 0) {
                      #ifdef _WIN32
                      SetConsoleOutputCP(CP_UTF8);
                      #endif /* _WIN32 */
                      if (i == ENCODE) {
                          std::cout << names[i] << " max: " << maxTimes[i] << "ms (" << maxTimeCount[i] << ") min: " << minTimes[i] << "ms (" << minTimeCount[i] << ") (" << counts[i] << " samples)\n";
                      }
                      // std::cout << names[i] << " avg: " << (totalTimes[i] / counts[i]) <<  " ms ± maxtime: " << maxTimes[i] << " (" << maxTimeCount[i] << ") mintime: " << minTimes[i] << " (" << minTimeCount[i] << ") (" << counts[i] << " samples)\n";
                      std::cout << names[i] << " took avg: " << (totalTimes[i] / counts[i]) <<  " ms ± " << (maxTimes[i] - minTimes[i]) / 2.0 << "ms (" << counts[i] << " samples)\n";
                  }
              }
          }
      }
      ```

  * ``timer.h``:
    
      ```cpp
      #ifndef TIMER_H    // #include guard
      #define TIMER_H
      
      #include <chrono>
      
      namespace Timer {
          enum TimerType {
              ENCODE = 0,
              RENDER_SCENE = 1,
              BLIT_MSAA = 2,
              RENDER_TEXT = 3,
              RENDER_GUI = 4,
              BLIT_TO_SCREEN = 5,
              FLIP_SHADER = 6,
              FLIP_FUNCTION = 7,
              GL_CLIENT_WAIT_SYNC = 8,
              GL_MAP_BUFFER_RANGE = 9,
              GLREADPIXELS_PBO_ON = 10,
              GLREADPIXELS_PBO_OFF = 11,
              QUEUE_PUSH_WAIT_PBO_ON = 12,
              QUEUE_PUSH_WAIT_PBO_OFF = 13,
          };
      
          void init();
          void startTimer(std::chrono::high_resolution_clock::time_point& tp);
          void endTimer(TimerType type, const std::chrono::high_resolution_clock::time_point& start);
          void printAverages();
      }
      
      #endif /* TIMER_H */
      ```

  * Example of using my custom Timer namespace:
 
    * Before the render loop:
      
      ```cpp
      Timer::init();
      std::chrono::high_resolution_clock::time_point t;
      ```

    * Inside the render loop, wrapping the function(s) you want to time:
   
      ```cpp
      Timer::startTimer(t);
      textRenderer.renderText(fpsText, x, y, scale, color, "Arial");
      textRenderer.renderAtlas("Arial");
      {
          // std::lock_guard<std::mutex> lock(coutMutex);  // only needed if you're printing to the terminal at run-time.
          Timer::endTimer(Timer::RENDER_TEXT, t);
      }
      ```

    * At the end, outside the render loop:

      ```cpp
      Timer::printAverages();
      ```
    
</details>

<details><summary> Flip shader </summary>

  * Added a flip shader to flip the framebuffer for video encoding on the GPU. Using the custom Timer namespace, I can see that the flip function (CPU) takes ~0.5ms, while the flip shader (GPU) takes ~0.006ms. On the surface, this is an 83x improvement, but it leads to no visible improvement in FPS, as 0.5ms is still very fast. However, for lower-end machines like my laptop, and especially when using battery-saver mode, using the flip function can lead to jittery video recording, which the flip shader fixes.
  * ``flip.vert``:
    
    ```glsl
    #version 430 core
    
    out vec2 TexCoord;                          // Pass texture coordinates to fragment shader
    
    void main()
    {
        // full-screen triangle: https://stackoverflow.com/a/59739538/7875204
        vec2 vertices[3] = vec2[3](vec2(-1,-1), vec2(3,-1), vec2(-1,3));
        gl_Position = vec4(vertices[gl_VertexID], 0, 1);
        TexCoord = 0.5 * gl_Position.xy + vec2(0.5);
    }
    ```
  * ``flip.frag``:
    
    ```glsl
    #version 430 core
    in vec2 TexCoord;
    out vec4 FragColor;
    
    uniform sampler2D screenTexture;
    
    void main() {
        // Flip vertically
        FragColor = texture(screenTexture, vec2(TexCoord.x, 1.0 - TexCoord.y));
    }
    ```
  * Inside ``main.cpp``, before the render loop:
    
    ```cpp
    // flip shader
    // -----------
    GLuint fboFlip, fboFlipTexture, dummyVAO;
    glGenFramebuffers(1, &fboFlip);
    glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);

    glGenVertexArrays(1, &dummyVAO);

    // Create the texture to attach to fboFlip
    glGenTextures(1, &fboFlipTexture);
    glBindTexture(GL_TEXTURE_2D, fboFlipTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboFlipTexture, 0);

    // Check framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR: fboFlip is not complete!\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader flipShader("flip.vert", "flip.frag");
    ```
  * Inside the render loop, just after rendering on-screen and before glReadPixels():
    
    ```cpp
    if (flip_shader) {
        Timer::startTimer(t);
        glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
        flipShader.use();
        glBindVertexArray(dummyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fboTexture);
        flipShader.setInt("screenTexture", 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            Timer::endTimer(Timer::FLIP_SHADER, t);
        }
    }
    ```

</details>

<details><summary> CMake fixes </summary>

  * Fixed a CMake GLOB_RECURSE issue using CONFIGURE_DEPENDS
 
    ```cmake
    # Recursively find all files to track for rebuilds
    file(GLOB_RECURSE ASSET_FILES CONFIGURE_DEPENDS RELATIVE ${ASSETS_SOURCE_DIR} "${ASSETS_SOURCE_DIR}/*")
    file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS RELATIVE ${SHADERS_SOURCE_DIR} "${SHADERS_SOURCE_DIR}/*")
    ```

</details>

<details><summary> Smoother button toggling </summary>

  * Smoother button press toggling using a simple bool if(pressed) check.
  * Top of ``main.cpp``:
 
    ```cpp
    bool press = 0;
    ```
    
  * In ``processInput()``:

    ```cpp
    // PBO
    // ---
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && (!press))
    {
        press = 1;
        Config::TogglePBO();
        pbo = Config::GetPBO();
        // std::cout << "Toggled PBO: " << !pbo << "->" << pbo << std::endl;
    }
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) && (press))
    {
        press = 0;
    }
    // Flip Shader
    // -----------
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        Config::ToggleFlipShader();
        flip_shader = Config::GetFlipShader();
        // std::cout << "Toggled PBO: " << !pbo << "->" << pbo << std::endl;
    }
    ```

  * This works well for one key, but doesn't work nicely when implementing it for other keys at the same time.

</details>

## Progress update 14 - Getters/setters→inline functions, flip shader fix, and made light move - 26/05/25

<details><summary> Getters/setters→inline functions </summary>

  * I was previously using getters/setters in ``Config.cpp`` like so:

    ```cpp
    namespace {     // anonymous namespace (encapsulation)
        bool                pbo             =  0;
        ...
    }
    namespace Config {
        void TogglePBO() {
            if (pbo)    { pbo = 0; }
            else        { pbo = 1; }
        }
        ...
        bool                GetPBO() { return pbo; }
        ...
    }
    ```

    however, this just meant ugly verbose boilerplate bloat code every time I wanted to set and access the variable in ``main.cpp``, on top of keeping track of a duplicate set of variables local to ``main.cpp``:

    ```cpp
    namespace {     // anonymous namespace (encapsulation)
        bool                pbo;
        ...
    }

    void ProcessInput(GLFWwindow *window) {
        if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && (!pboPressed)) {
            pboPressed = true;
            Config::TogglePBO();      // updating Config's pbo variable
            pbo = Config::GetPBO();   // updating main.cpp's pbo variable
            ...
        }
        ...
    }
    ```

  * Also I was watching this video and he explains it well (albiet a little angrily, but understandably):

    [![Watch the video](https://img.youtube.com/vi/_xLgr6Ng4qQ/maxresdefault.jpg)](https://www.youtube.com/watch?v=_xLgr6Ng4qQ)

  * So I switched to using simple inline functions in ``Settings.h``:

    ```cpp
    namespace Settings {
        extern bool                     pbo;
        ...
        inline void TogglePBO() { pbo = !pbo; }
        ...
    }
    ```

    defining the variable in ``Settings.cpp``:

    ```cpp
    namespace Settings {
        bool                    pbo             =  0;
        ...
    }
    ```

    setting the variable in ``main.cpp`` by doing:

    ```cpp
    void ProcessInput(GLFWwindow *window) {
        if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && (!pboPressed)) {
            pboPressed = true;
            Settings::TogglePBO();    // updating Settings's pbo variable
            ...
        }
        ...
    }
    ```

    and getting the variable in ``main.cpp`` by simply doing:

    ```cpp
    std::cout << Settings::pbo << "\n";
    ```

    or:

    ```cpp
    using namespace Settings;
    std::cout << pbo << "\n";
    ```

  * The reason I define the variables in ``Settings.cpp`` and not ``Settings.h`` is that when I want to change the value of the variable, only ``Settings.cpp`` needs to be recompiled, rather than changing the value in ``Settings.h`` leading to **every** cpp file that includes ``Settings.h`` needing to be recompiled.
    
</details>

<details><summary> Made light move </summary>

* Made the light cube move, quite simple, just moved the cube around in spherical coordinates, combined with some modulation to produce interesting behaviour.

  ```cpp
  // light
  // -----
  float radius = 8.0f;
  float theta = crntTime * 0.5f; // slower rotation (Y-axis)
  float phi = glm::radians(90.0f + sin(crntTime * 0.6f) * 90.0f); // varies between 0° and 180°
  
  lightPos.x = radius * sin(phi) * cos(theta);
  // lightPos.y = radius * cos(phi);
  lightPos.y = 1.5f + glm::sin(crntTime * 0.7f) * 2.0f; // modulate the height
  lightPos.z = radius * sin(phi) * sin(theta);
  
  // simple circle in XZ plane
  // lightPos.x = glm::cos(crntTime) * 8.0f;
  // lightPos.y = 1.5f;
  // lightPos.z = glm::sin(crntTime) * 8.0f;
  
  ourShader.setVec3("lightPos", lightPos);
  ```

</details>

## Progress update 15 - Minimal FreeType and FFmpeg with libx264, more input keys, and no longer using /.vscode/tasks.json - 30/05/25

<details><summary> Minimal FreeType </summary>

  * IN MSYS2 MINGW64 console (first go to ``C:/``) or on Linux, run:

    ```shell
    mkdir freetype_build
    cd freetype_build
    git clone https://gitlab.freedesktop.org/freetype/freetype.git
    cd freetype
    ```
        
    The next 2 commands take a while, you will see ``cat: config.h: No such file or directory`` after running ``make -j$(nproc)``, this is normal.

    ```shell
    ./autogen.sh
    ./configure --prefix=$(cd .. && pwd) \
                --without-zlib \
                --without-bzip2 \
                --without-png \
                --without-harfbuzz \
                --without-brotli \
                --with-pic
    make -j
    make install
    ```

    this will create two folders, ``/lib/`` and ``/include/`` in ``/freetype_build/``, which contain the static library (.a) and headers (.h) for x264.

</details>

<details><summary> Minimal FFmpeg with libx264 </summary>

  * IN MSYS2 MINGW64 console (first go back to ``C:\``) or on Linux, run:

    ```shell
    mkdir ffmpeg_build
    cd ffmpeg_build
    git clone --depth 1 https://code.videolan.org/videolan/x264.git
    cd x264
    ```
    
    The next 2 commands take a while, you will see ``cat: config.h: No such file or directory`` after running ``make -j$(nproc)``, this is normal.

    ```shell
    ./configure --prefix="$(cd .. && pwd)" --enable-static --disable-opencl
    make -j$(nproc)
    make install
    ```

    you may get errors on low RAM machines, if so, run:

    ```shell
    make clean
    make V=1
    make install
    ```
    
    this will create two folders, ``/lib/`` and ``/include/`` in ``/ffmpeg_build``, which contain the static library (.a) and headers (.h) for x264.

  * Now compile a minimal FFmpeg static library that supports x264:

    ```shell
    cd ..
    git clone https://git.ffmpeg.org/ffmpeg.git ffmpeg
    cd ffmpeg
    ```

    Next command takes a while, (minimal FFmpeg build, without zlib, bzlib, lzma, iconv):

    ```shell
    PKG_CONFIG_PATH="$(cd .. && pwd)/lib/pkgconfig" \
    ./configure \
      --disable-everything \
      \
      --prefix="$(cd .. && pwd)" \
      --pkg-config-flags="--static" \
      --extra-cflags="-I$(cd .. && pwd)/include" \
      --extra-ldflags="-L$(cd .. && pwd)/lib" \
      \
      --enable-gpl \
      --enable-nonfree \
      --enable-libx264 \
      --enable-encoder=libx264 \
      --enable-encoder=h264_mf \
      \
      --enable-muxer=mp4 \
      --enable-protocol=file \
      \
      --enable-avcodec \
      --enable-avformat \
      --enable-avutil \
      --enable-swscale \
      \
      --enable-static \
      --disable-shared \
      --disable-programs \
      --disable-doc \
      --disable-debug \
      \
      --disable-zlib \
      --disable-bzlib \
      --disable-lzma \
      --disable-iconv \
      \
      --disable-sdl2 \
      --disable-schannel \
      --disable-avdevice \
      --disable-avfilter \
      --disable-swresample
    ```

    Note: ``--disable-everything`` NEEDS to be at the top, flags are read in order.
    
    Then build (this can take a while):
    
    ```shell
    make -j$(nproc)
    ```
    
    and compile:
    
    ```shell
    make install
    ```

    The ``/include/`` and ``/lib/`` folders will be inside ``/ffmpeg_build/``. Copy the contents into the project's ``/include/`` and ``/lib/`` folders.

  * There is just 3 more static libraries we need to include for the program to run on Windows: ``libstrmiids.a``, ``libws2_32.a``, and ``libbcrypt.a``. When you install GCC using ``pacman -S mingw-w64-x86_64-gcc``, these 3 libraries will be installed as well, and can be found in ``C:\msys64\mingw64\lib``. Just copy the 3 static libraries into the project ``/lib/ffmpeg/`` folder and you're done.

  * I was getting an error when trying to compile the program on my laptop, ``undefined reference to clock_gettime64``, ``undefined reference to nanosleep64``. It took me a while to figure out what the issue was. My PC had GCC **15**.1.0 (2025, recent, up to date), but my laptop was using GCC **14**.2.0 (2024), my FFmpeg static libraries (.a files) were compiled on my PC (GCC 15.1.0), and I was trying to link those .a files on my laptop (GCC 14.2.0). ``libavutil.a`` (and possibly others) contain symbols that GCC 14.2.0 doesn't recognise, particularly ``clock_gettime64`` and ``nanosleep64``. Using an older version of GCC, in particular an older major version, can lead to errors when compiling and linking static libraries that were compiled with a different version of GCC. Which is why I have decided to add a ``lib_build_info.txt`` file that shows the GCC version that was used to compile the FFmpeg and FreeType source files to static libraries, as well as the operating system that was used. To see what version of GCC you are using, just run:

    ```shell
    g++ --version
    ``` 

</details>

<details><summary> More input keys </summary>

  * Was finally able to get smooth button toggling for multiple input keys to work. I had to give each input key its own bool checking if the key was pressed, for some reason using a shared ``pressed`` bool didn't work.

    Global pressed bools:
    
    ```cpp
    // button press
    // ------------
    bool pboPressed = false;
    bool flipPressed = false;
    bool wireframePressed   = false;
    bool imguiPressed       = false;
    bool atlasPressed       = false;
    ...
    ```

    Inside ``processInput()``:
    
    ```cpp
    // PBO
    // ---
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && (!pboPressed)) {
        pboPressed = true;
        TogglePBO();
        std::cout << "[main] P key pressed. Press = " << pboPressed << " Time: " << crntTime << "\n";
        // std::cout << "P key pressed: " << !pbo << "->" << pbo << " Press: " << !pboPressed << "->" << pboPressed << " Time: " << crntTime << "\n";
    }
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) && (pboPressed)) {
        pboPressed = false;
        std::cout << "[main] P key released. Press = " << pboPressed << " Time: " << crntTime << "\n";
    }
    // Flip Shader
    // -----------
    if ((glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) && (!flipPressed))
    {
        flipPressed = true;
        ToggleFlipShader();
        std::cout << "[main] F key pressed. Press = " << flipPressed << " Time: " << crntTime << "\n";
        // std::cout << "Toggled PBO: " << !pbo << "->" << pbo << "\n";
    }
    if ((glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) && (flipPressed)) {
        flipPressed = false;
        std::cout << "[main] F key released. Press = " << flipPressed << " Time: " << crntTime << "\n";
    }
    // Wireframe
    // ---------
    if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) && (!wireframePressed))
    {
        wireframePressed = true;
        ToggleWireframe();
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // glLineWidth(2.0f); // Only affects non-core-profile backends
        } else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        std::cout << "[main] M key pressed. Press = " << wireframePressed << " Time: " << crntTime << "\n";
    }
    if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) && (wireframePressed)) {
        wireframePressed = false;
        std::cout << "[main] M key released. Press = " << wireframePressed << " Time: " << crntTime << "\n";
    }
    // ImGui
    // -----
    if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) && (!imguiPressed))
    {
        imguiPressed = true;
        ToggleImGui();
        std::cout << "[main] N key pressed. Press = " << imguiPressed << " Time: " << crntTime << "\n";
    }
    if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) && (imguiPressed)) {
        imguiPressed = false;
        std::cout << "[main] N key released. Press = " << imguiPressed << " Time: " << crntTime << "\n";
    }
    // Atlas
    // -----
    if ((glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) && (!atlasPressed))
    {
        atlasPressed = true;
        cycleTriState(currentTextMode);
        std::cout << "[main] B key pressed. Press = " << atlasPressed << " Time: " << crntTime << "\n";
    }
    if ((glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) && (atlasPressed)) {
        atlasPressed = false;
        std::cout << "[main] B key released. Press = " << atlasPressed << " Time: " << crntTime << "\n";
    }
    ...
    ```

    * You will notice that the <kbd>B</kbd> key toggles the font texture atlas, but in a tri-state. What this means is that it will cycle between the text at the top (FPS, etc.) and the font texture atlas both ON, just the text at the top ON, and both OFF.

      In ``Settings.cpp``:
      
      ```cpp
      namespace Settings {
          TextTriState            currentTextMode = TextTriState::TextAndAtlasON;
          ...
      }
      ```

      In ``Settings.h``:
      
      ```cpp
      namespace Settings {
          enum class TextTriState : uint8_t {
              TextAndAtlasOFF    = 0,
              TextAndAtlasON     = 1,
              TextONAtlasOFF     = 2
          };
          inline void cycleTriState(TextTriState& state) {
              state = static_cast<TextTriState>((static_cast<uint8_t>(state) + 1) % 3);
          }
          extern TextTriState currentTextMode;
          ...
      }
      ```

      In the render loop in ``main.cpp``:

      ```cpp
      if (currentTextMode == TextTriState::TextAndAtlasON || currentTextMode == TextTriState::TextONAtlasOFF)
          // textRenderer.renderText(fpsText, x, y, scale, color, font);
          textRenderer.renderTextFast(fpsText, x, y, scale, color, font);
      if (currentTextMode == TextTriState::TextAndAtlasON)
          textRenderer.renderAtlas(font);
      ```

</details>

<details><summary> No longer using /.vscode/tasks.json </summary>

  * Stopped maintaining ``tasks.json``. It's been a long road, at the very start trying to get VSCode to work, and the pain of linking libraries inside ``tasks.json``, but now finally I have decided to stop using VSCode's ``tasks.json`` to build the program, as the program has become too complicated, and using ``CMakeLists.txt`` is just so much better. With VSCode's ``tasks.json``, I would have to precompile the ImGui and ImPlot source files, which I was using CMake to do anyway. CMake can compile source files to static libraries and link them to the executable, all in one command. So that means I can remove the ``./vsocde/`` folder from the repository.
  
</details>

## Progress update 16 - Toggleable screen recording, GUI namespace, multithreading and PBOs separate toggles, flip shader fix, and CMake custom targets (cache clear and zip) - 03/06/25

<details><summary> Toggleable screen recording </summary>

  * Changed encoder.cpp from an Encoder namespace to a class, which handles instancing.
  * Can now toggle recording ON and OFF with the R key, and can record multiple times. Recorded videos will have the date and time as the title. Needed to change the encoder thread structure to allow this.

    In ``main.cpp``:

    ```cpp
    bool recordPressed      = false;

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window, float timeDiff, float crntTime, std::unique_ptr<Encoder> &encoder) {
        // Recording
        // ---------
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && (!recordPressed))
        {
            recordPressed = true;
            recording = !recording;
            queueCond.notify_one();  // Wake encoder thread if asleep
            std::cout << "[main] R key pressed. Press = " << recordPressed << " Time: " << crntTime << "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) && (recordPressed)) {
            recordPressed = false;
            std::cout << "[main] R key released. Press = " << recordPressed << " Time: " << crntTime << "\n";
        }
        ...
    }
    ```
    
    In ``encoder.cpp``, to get date and time as the title:
    
    ```cpp
    std::string Encoder::getTimestampedFilename() {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime;
        #ifdef _WIN32
        localtime_s(&localTime, &nowTime);
        #else
        localtime_r(&nowTime, &localTime);
        #endif
    
        std::ostringstream oss;
        oss << "../output/"
            << std::put_time(&localTime, "%Y.%m.%d - %H.%M.%S")
            << ".mp4";
        return oss.str();
    }
    ```
    
  * Added a shuttingDown bool which is used in the outer while loop in the encoder thread, this gets turned to true at the very end when the program is closed by the user (clicking ESC instead of gracefully turning recording off first, then closing the program).
  
</details>

<details><summary> GUI namespace </summary>

  * Moved all the code that handles ImGui and ImPlot into a separate ``gui.cpp`` file. It's a bit too big to paste here, but the general outline is as follows:

    ```cpp
    // ImGui
    // -----
    #include <imgui/imgui.h>
    #include <imgui/imgui_impl_glfw.h>
    #include <imgui/imgui_impl_opengl3.h>
    // ImPlot
    // ------
    #include <implot/implot.h>
    // Settings
    // --------
    #include <learnopengl/Settings.h>
    #include <learnopengl/encoder.h>
    
    namespace GUI {
        // initialization: text, window position, etc.
        namespace { // anonymous namespace (encapsulation) evaluated once at program startup
            ...
        }

        // before the render loop
        void Init(GLFWwindow *window) {
            ...
        }

        // at the top of the render loop
        void NewFrame() {
            ...
        }
    
        // hot function, used in render loop
        void Render(Encoder *encoder) {
            ...
        }

        // after the render loop, at the end of main()
        void Exit() {
            ...
        }
    }
    ```

  * This is much better than cluttering ``main.cpp`` with functions that could be in their own cpp file. It makes files smaller, self-contained, quicker to compile, and easier to read.
 
  * Now in ``main.cpp`` just have to include ``gui.h`` and use the ``GUI`` namespace functions like so:

    Include ``gui.h``:
    
    ```cpp
    #include <learnopengl/gui.h>            // GUI
    ```

    Before the render loop:

    ```cpp
    GUI::Init(window);
    ```

    Inside the render loop at the top:

    ```cpp
    GUI::NewFrame();
    ```

    Inside the render loop, render the GUI:

    ```cpp
    GUI::Render(encoder.get());
    ```

    After the render loop, at the end of ``main()``:

    ```cpp
    GUI::Exit();
    ```

  * You may notice I am passing a raw pointer of the ``Encoder`` class to ``GUI``'s ``Render()`` function.
    * Pass a raw pointer (``Encoder *encoder``) when just accessing, not owning.
    * Pass a ``unique_ptr``/``shared_ptr`` (``std::unqiue_ptr<Encoder>``) when transferring ownership.
  * As I am just needing to access the value of the ``isEncoding`` bool in the ``Encoder`` class instance, and not needing to transfer ownership of the instance, it's better to just pass a raw pointer, using ``encoder.get()``.
    
  * Can now select the encoder settings (crf, preset, framerate, etc.) when recording is OFF with drop-down menus, sliders, and checkboxes. The menu becomes disabled when recording is ON.
 
    Inside ``Render()`` in ``gui.cpp``:

    ```cpp
    if (isEncoding) ImGui::BeginDisabled();
    {
        bool isLibx264 = Settings::libx264;
        bool isH264mf = !Settings::libx264;
        // Custom checkbox logic for h264_mf
        ImGui::Bullet();
        ImGui::SameLine();
        bool h264mfClicked = ImGui::Checkbox("h264_mf", &isH264mf);
        if (h264mfClicked && !Settings::recording && Settings::libx264) {
            Settings::libx264 = false;
            encoderChanged = true;
        }
        // Custom checkbox logic for libx264
        ImGui::Bullet();
        ImGui::SameLine();
        bool libx264Clicked = ImGui::Checkbox("libx264", &isLibx264);
        if (libx264Clicked && !Settings::recording && !Settings::libx264) {
            Settings::libx264 = true;
            encoderChanged = true;
        }

        // --- Encoder-specific settings ---
        if (Settings::libx264) {
            // --- libx264 settings ---

            // Match g_preset to index
            // static int presetIdx = 0;
            for (int i = 0; i < IM_ARRAYSIZE(presets); ++i) {
                if (Settings::g_preset == presets[i]) {
                    presetIdx = i;
                    break;
                }
            }

            ImGui::PushItemWidth(100.0f); // Set desired width in pixels
            if (ImGui::Combo("Preset", &presetIdx, presets, IM_ARRAYSIZE(presets))) {
                Settings::g_preset = std::string(presets[presetIdx]);
            }

            // Convert crf string to int (only once unless changed)
            static int crf = std::stoi(Settings::g_crf);
            if (ImGui::SliderInt("CRF", &crf, 17, 30)) {
                Settings::g_crf = std::to_string(crf);
            }

        } else {
            // h264_mf settings
            static int gopSize = Settings::g_gop_size; // e.g. 10–120 // smaller GOP=>more I-frames: better seeking/error recovery, faster previewing, but larger file size. for real-time/streaming: GOP = 2 x framerate.
            static int maxBFrames = Settings::g_max_b_frames; // 0–5
            
            ImGui::PushItemWidth(100.0f); // Set desired width in pixels
            int mbps = static_cast<int>(Settings::g_bit_rate / 1'000'000); // Convert to Mbps
            if (ImGui::SliderInt("Bitrate (Mbps)", &mbps, 10, 100, "%d", ImGuiSliderFlags_AlwaysClamp)) {
                Settings::g_bit_rate = static_cast<int64_t>(mbps) * 1'000'000; // Convert back to bps
            }

            if (ImGui::SliderInt("GOP Size", &gopSize, 10, 120)) {
                Settings::g_gop_size = gopSize;
            }

            if (ImGui::SliderInt("Max B-frames", &maxBFrames, 0, 5)) {
                Settings::g_max_b_frames = maxBFrames;
            }
        }

        // --- Framerate dropdown (shared between encoders) ---
        if (ImGui::Combo("Framerate", &selectedFramerateIdx, framerates, IM_ARRAYSIZE(framerates))) {
            Settings::framerate = (selectedFramerateIdx == 0) ? 30 : 60;
        }
        
        // bool isLaptopMode = Settings::laptop_mode;
        ImGui::Checkbox("Laptop mode (deep copy frames, thread-safe)", &Settings::laptop_mode);
    }
    if (isEncoding) ImGui::EndDisabled();
    ```

</details>

<details><summary> Multithreading and PBOs separate toggles </summary>

  * Multithreading (encoder thread) and asynchronous PBO readback are now separate toggles. I didn't do this earlier as I had trouble separating them before.

    Global pressed bools in ``main.cpp``:

    ```cpp
    // button press
    // ------------
    bool pboPressed         = false;
    bool encoderPressed     = false;
    ...
    ```

    ``processInput()`` in ``main.cpp``:

    ```cpp
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window, float timeDiff, float crntTime, std::unique_ptr<Encoder> &encoder) {
        using namespace Settings; // compile-time instruction (no runtime overhead)
        std::ostringstream oss;
        // PBO
        // ---
        if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && (!pboPressed)) {
            pboPressed = true;
            TogglePBO();
            std::lock_guard<std::mutex> coutLock(coutMutex);
            std::cout << "[main] P key pressed. Press = " << pboPressed << " Time: " << crntTime << "\n";
            // std::cout << "P key pressed: " << !pbo << "->" << pbo << " Press: " << !pboPressed << "->" << pboPressed << " Time: " << crntTime << "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) && (pboPressed)) {
            pboPressed = false;
            std::cout << "[main] P key released. Press = " << pboPressed << " Time: " << crntTime << "\n";
        }
        // Encoder thread
        // --------------
        if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) && (!encoderPressed))
        {
            encoderPressed = true;
            encoder_thread.store(!encoder_thread.load(std::memory_order_acquire), std::memory_order_release);
            encoder->queueCond.notify_all();  // Wake encoder thread if asleep
            std::lock_guard<std::mutex> coutLock(coutMutex);
            oss << "[main] T key pressed. Press = " << encoderPressed << " Time: " << crntTime << "\n";
        }
        if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) && (encoderPressed)) {
            encoderPressed = false;
            std::lock_guard<std::mutex> coutLock(coutMutex);
            oss << "[main] T key released. Press = " << encoderPressed << " Time: " << crntTime << "\n";
        }
        ...
    }
    ```
    

</details>

<details><summary> Flip shader fix </summary>

  * I just realised I was calling the flip shader for **every** cycle of the render loop, when I only needed to call it when actually encoding a frame (every 30th or 60th of a second, depending on if encoding at 30fps or 60fps). I had a similar experience when I realised I was encoding frames for every cycle of the render loop... This was a simple fix, just putting the flip shader code inside Step 4 of the render loop, just before encoding.

    Previously, I was doing the flip shader code before Step 4, outside of ``if (vsync || (encodeDiff >= 1.0 / framerate)) { ... }``, but now I am doing it inside Step 4, calling it only when it's needed.
  
    ```cpp
    // Step 4: Read pixels from the resolved FBO for off-screen encoding
    // -----------------------------------------------------------------
    encodeDiff = crntTime - encodeTime;
    if (vsync || (encodeDiff >= 1.0 / framerate)) {
        // fbo first needs to be flipped for encoding
        if (flip_shader) {
            Timer::startTimer(t);
            glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
            flipShader.use();
            glBindVertexArray(quadVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, fboTexture);
            flipShader.setInt("screenTexture", 0);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
            {
                // std::lock_guard<std::mutex> lock(coutMutex);
                Timer::endTimer(Timer::FLIP_SHADER, t);
            }
        }
        ...
    }
    ```

    * Fixed an issue where the screen went black in the recording when flip shader was ON and wireframe mode was enabled, this was due to the fact that the flip shader draws the non-MSAA FBO texture as a fullscreen triangle, and wireframe is a global OpenGL rasterization stage, so it turned the fullscreen triangle hollow.

    ```cpp
    // Step 4: Read pixels from the resolved FBO for off-screen encoding
    // -----------------------------------------------------------------
    encodeDiff = crntTime - encodeTime;
    if (vsync || (encodeDiff >= 1.0 / framerate)) {
        // fbo first needs to be flipped for encoding
        if (flip_shader) {
            if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Disable wireframe for this pass
            Timer::startTimer(t);
            glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
            flipShader.use();
            glBindVertexArray(quadVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, fboTexture);
            flipShader.setInt("screenTexture", 0);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            glBindFramebuffer(GL_FRAMEBUFFER, fboFlip);
            {
                // std::lock_guard<std::mutex> lock(coutMutex);
                Timer::endTimer(Timer::FLIP_SHADER, t);
            }
            if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Restore wireframe
        }
        ...
    }
    ```
    
</details>

<details><summary> CMake custom targets (cache clear and zip) </summary>

  * Made a CMake custom target that you can use to delete ``CMakeCache.txt`` which seems to fix the issue of the "Rechecking globbed directories..." infinite loop when running ``ninja`` sometimes. And another custom target that compresses only the essentials (``/assets/``, ``/shaders/``, ``/bin/``) into a zip folder for Release.

    ```cmake
    # Delete the CMakeCache.txt file (fixes "Rechecking globbed directories..." infinite loop issue when running "ninja" sometimes)
    # Run inside build/: cmake --build . --target clear
    add_custom_target(clear
        COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_BINARY_DIR}/CMakeCache.txt"
        COMMENT "Clearing CMake cache..."
        VERBATIM
    )
    
    # Run inside build/: ninja zip OR cmake --build . --target zip
    set(PACKAGE_NAME "OpenGL-3D-Render-Recorder-v1.2.0-Windows-x86_64")
    # set(PACKAGE_NAME "OpenGL-3D-Render-Recorder-v1.2.0-Arch-Linux-x86_64")
    set(PACKAGE_DIR "${CMAKE_BINARY_DIR}/${PACKAGE_NAME}")
    set(PACKAGE_ZIP "${CMAKE_BINARY_DIR}/${PACKAGE_NAME}.zip")
    # Uncomment ALL to make CMake run this target by default (always makes ZIP file)
    add_custom_target(zip #ALL
        # Clean previous packaging folder if it exists
        COMMAND ${CMAKE_COMMAND} -E rm -rf "${PACKAGE_DIR}"
    
        # Create directory structure
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/shaders"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/assets"
    
        # Copy binaries except imgui.ini
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/bin" "${PACKAGE_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E rm -f "${PACKAGE_DIR}/bin/imgui.ini"
    
        # Copy shaders and assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "${PACKAGE_DIR}/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${PACKAGE_DIR}/assets"
    
        # Create the ZIP archive (bin, shaders, assets under PACKAGE_NAME/)
        COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${PACKAGE_ZIP}" --format=zip -- "${PACKAGE_NAME}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    
        COMMENT "Packaging into ${PACKAGE_NAME}.zip with top-level folder"
    
        # Delete unzipped packaging folder
        COMMAND ${CMAKE_COMMAND} -E rm -rf "${PACKAGE_DIR}"
    
        VERBATIM
    )
    ```

</details>

## Progress update 17 - Merge branch 'encoder-feature' - 21/06/25

<details><summary> Merge branch 'encoder-feature' </summary>

  * In the spirit of moving all code to separate cpp files like with the GUI code, I have tasked myself with moving all encoder logic that is still in ``main.cpp``, like the encoder thread, to ``encoder.cpp``.
  * I also wanted to do all commits on a separate branch, and only when finally finished, merge to main, which is what Git was intended for. This also means I can switch back to the working main branch whenever I want.

  <ins>How to use git to do temp branches</ins>

  1. Make sure you're on <ins>``main``</ins>

     ```console
     git checkout main
     ```

     (Make sure work is committed or stashed before switching branches, Git will warn you anyway)
     
  2. Create and switch to a temporary branch

     ```console
     git checkout -b temp-feature
     ```

  3. Work and commit incrementally

     As you make changes
     
     ```console
     git add .
     git commit -m "Write commit message here"
     ```

     or just use ``git commit`` to open the text editor for longer commit messages

  4. Switch back to ``main`` anytime

     ```console
     git checkout main
     ```

     And go back:

     ```console
     git checkout temp-feature
     ```

  6. Push branch to GitHub (remote)

     ```console
     git push -u origin temp-feature
     ```

     ``-u`` stands for ``--set-upstream``. For the very first push to remote, need to add ``-u``, it:

       * Pushes branch to remote
       * Links local branch to remote branch, so can just type ``git pull``/``git push`` next time.
    
     Remember when you're on another branch to ``push``/``pull`` to that branch, **NOT** to ``main``.

     Do this:

     ```console
     git pull origin temp-feature
     ```

     **NOT** this:

     ```console
     git pull origin main
     ```

     If you accidentally make a wrong commit, do:

     ```console
     git log --oneline -n 5
     ```

     to get the ID (e.g. ``abcd123``) of the last commit you want to undo.

     Then do:

     ```console
     git reset --hard abcd123
     ```

     to remove that commit.

  8. To switch to a remote branch from another machine, do:

     Fetch all remote branches
     
     ```console
     git fetch
     ```

     Confirm it's available remotely
     
     ```console
     git branch -r
     ```

     Checkout the remote branch and track it locally
    
     ```console
     git checkout -b temp-feature origin temp-feature
     ```

       * Creates a local branch called ``temp-feature``
       * Sets it to track ``origin/temp-feature`` (remote)

     Notice you don't need to add ``-u`` (``--set-upstream``), as that is only when creating a **new** branch locally and you want to set it to track a remote branch when pushing for the first time.

     This will list all **local** branches

     ```console
     git branch
     ```

     Use this to get more details about all the local branches

     ```console
     git branch -vv
     ```
     
    
  8. If you have half-changes that you don't want to commit, but want to save so you can switch to another branch, then use:

     ```console
     git stash
     ```

     to stash the changes. You can see your list of stashes using:

     ```console
     git stash list
     ```

     When you switch to another branch and come back, your stashed changes won't appear immediately, it will display your last commit to that branch. To make your stashed changes visible again, use:

     ```console
     git stash pop    // reapply and remove stash
     ```

     or

     ```console
     git stash apply  // reapply but keep stash 
     ```

     I prefer to do ``git stash pop`` as it promotes not cluttering the stash list, and forces you to remember to use ``git stash`` again when you want to switch branches again.

     Make sure to run ``git stash list`` and ``git stash pop`` if there is a stash saved. Then later ``git stash`` to save.
     
  10. When the feature is complete and tested

      ```console
      git commit -m "Fished temp-feature"          // final commit
      git checkout main
      git pull origin main                         // pull any changes to main from remote
      git merge --no-commit --no-ff temp-feature 
      ```

      ``git merge --no-commit --no-ff temp-feature`` is better than ``git merge temp-feature``, ``--no-commit`` means it doesn't commit with the merge, it will just do a dry run to show what merge conflicts there are, and gives you the option to commit or abort. And ``--no-ff`` prevents git from fast forward merging, if main didn't diverge from the feature branch, git will fast forward merge, merging as if there never was a branch.

      If there are any merge conflicts, manually fix them by keeping/removing the code you want/don't want. Git will show where the conflicts are with markers showing which is from main and which is from the temp-branch. All you have to do is delete the code you don't want, as well as the markers that Git adds, and keep the code you want.

      Once you're done and there are no more merge conflicts, commit your changes

      ```console
      git commit
      ```

      or abort

      ```console
      git merge --abort
      ```

      This will commit the merge to main or abort the merge if you chose that option. If you commited with the merge, all that's left is to push the merge to remote, and optionally add a tag with a version number

      ```console
      git push origin main
      git tag -a v1.2.0 -m "..."
      git push origin v1.2.0
      ``` 

      And optionally delete the branch

      ```console
      git branch -d temp-feature             // delete local branch
      git push origin --delete temp-feature  // delete remote branch
      ```
      
</details>

<details><summary> Fix messy terminal output </summary>

  * I noticed that lines printed to the terminal would sometimes get messed up, appearing at the end or in the middle of other lines rather than on their own separate line.
  * The cause is that ``std::cout`` is buffered but not thread-safe for <ins>interleaved</ins> output from multiple threads. Even with a ``std::lock_guard``, if one thread writes a long log in multiple statements or prints without a newline, it can clash with other threads.
  * The fix is to build your full message in a local string first. Instead of:

    ```cpp
    std::lock_guard<std::mutex> coutLock(coutMutex);
    std::cout << "[Timer] encodeFrame avg: " << time << "\n";
    ```

    Do this:

    ```cpp
    std::ostringstream oss;
    oss << "[Timer] encodeFrame avg: << time << "\n";
    {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << oss.str();
    }
    ```

    This ensures ``std::cout`` is only touched <ins>once</ins> under the lock.

</details>

<details><summary> Mysterious crashing issues </summary>

  * The program was mysteriously crashing when hard exiting from the program (clicking <kbd>Esc</kbd>) with the encoder thread ON.
    * The issue, which I figured out, was that at the end of the program I was deleting the VAO, PBO, buffers, etc., and **then** stopping encoding.
    * The fix was simply to stop encoding first, and delete the buffers at the very end.

  * The program was also crashing when PBOs was ON. This was partly fixed by moving ``encoder->stop()`` in ``main.cpp`` <ins>before</ins> VBO, VAO, etc. deletion (as mentioned before), but the program still stalled despite encoding the video properly, which I did not like as it used to exit cleanly and immediately before. Any crash means there was an issue.
    * This took me a while to figure out the issue, and this is something that no LLM can help with. The issue was calling ``delete[] frame;`` at the end of ``main()``. For some reason this was causing the issue. Likely the encoder thread was still using the items that frame pointed to. I wasn't having this issue on my PC, but on my laptop with a slower CPU I had this issue, which supports the idea that the encoder thread was still using the data in ``frame`` which had been deleted. But I joined the encoder thread with the main thread before deleting the frame data, so I'm not sure why that would be an issue.
    * I am slightly concerned about writing to a ``frame`` buffer in the main thread, and reading/encoding from it in the encoder thread at the same time. Seems like a race condition waiting to happen. I think I got lucky as the time between each frame being rendered, 16.6ms (60fps), was longer than ``glReadPixels()`` and ``encodeFrame()``, the time it took to read the pixel data and encode the frame, which was ~10ms max on my PC. However, on my laptop ``glReadPixels()`` and ``encodeFrame()`` are not as fast, and that would explain why I was getting glitchy/jittery video recordings on my laptop, as the encoder thread could be reading from the frame buffer while the main thread was writing new data to it.
    * To fix this, I added a circular/ring buffer.

      Instead of doing this:

      ```cpp
      unsigned char* frame = NULL;
      frame = new unsigned char[SCR_WIDTH * SCR_HEIGHT * 3];
      ```

      I do this:

      In ``Settings.h``:
   
      ```cpp
      namespace Settings {
          constexpr std::size_t           BUFFER_COUNT    =  5;
          ...
      }
      ```
   
      In ``main.cpp``:
      
      ```cpp
      #include <learnopengl/Settings.h>       // Settings

      int main() {
          ...
          std::array<std::unique_ptr<uint8_t[]>, BUFFER_COUNT> frameBuffers;
          int currentWriteIndex = 0;
          for (int i = 0; i < static_cast<int>(BUFFER_COUNT); ++i) {
              frameBuffers[i] = std::make_unique<uint8_t[]>(DATA_SIZE);
          }
      ```
   
      In the render loop, to get the raw pointer:
   
      ```cpp
      uint8_t *buffer = frameBuffers[currentWriteIndex].get(); // get raw pointer
      ```

      and to cycle between each frame buffer:

      ```cpp
      currentWriteIndex = (currentWriteIndex + 1) % BUFFER_COUNT;
      ```

</details>

<!-- ADD BIBLIOGRAPHY -->
<!-- ADD CODE SHOWING FBO, RBO, PBO, etc. -->
<!-- FINALLY SHOW RESULTS WITH TEXTURES -->
<!-- EXPLAIN BETTER GOING FROM 2D to 3D WITH TRANSFORMATIONS AND SHADERS -->
## License
GNU General Public License v3.0
