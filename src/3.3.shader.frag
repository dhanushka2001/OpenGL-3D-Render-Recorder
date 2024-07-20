#version 430 core
layout (location = 0) out vec4 FragColor;

//layout (location = 2) in vec4 ourPosition;
layout (location = 3) in vec3 ourColor;

void main()
{
    //FragColor = ourPosition;
    FragColor = vec4(ourColor, 1.0f);
}