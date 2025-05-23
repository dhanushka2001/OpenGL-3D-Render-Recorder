#version 430 core
#define RENDER_3D	1

#if RENDER_3D==0
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
#endif

#if RENDER_3D==1
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out vec3 FragPos;
out vec3 Normal;
// out vec3 LightPos;	// view-space
// uniform vec3 lightPos; // we now define the uniform in the vertex shader and pass the 'view space' lightpos to the fragment shader. lightPos is currently in world space.
#endif

out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
	
	#if RENDER_3D==0
	ourColor = aColor;
	#endif

	#if RENDER_3D==1
	FragPos = vec3(model * transform * vec4(aPos, 1.0)); // world-space
	// FragPos = vec3(view * model * transform * vec4(aPos, 1.0)); // view-space
	// Normal = mat3(model) * aNormal;
	// Normal = aNormal;
	Normal = mat3(transpose(inverse(model))) * aNormal;  // inversing matrices is a costly operation
	// Normal = mat3(transpose(inverse(view * model))) * aNormal;  // inversing matrices is a costly operation (view-space)
	// LightPos = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
    #endif
	
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
