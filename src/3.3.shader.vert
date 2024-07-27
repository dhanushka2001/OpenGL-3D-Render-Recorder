#version 430 core
layout (location = 0) in vec3 aPos;
//layout (location = 0) uniform float xOffset;
layout (location = 1) in vec3 aColor;

//layout (location = 2) out vec4 ourPosition;
layout (location = 3) out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
    //ourPosition = gl_Position;
}