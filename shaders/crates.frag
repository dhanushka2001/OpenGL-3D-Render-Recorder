#version 430 core
#define RENDER_3D	1

#if RENDER_3D==0
in vec3 ourColor;
#endif
in vec2 TexCoord;

out vec4 FragColor;

uniform float mixValue;
// texture sampler
uniform sampler2D texture1;	// crate
uniform sampler2D texture2;	// awesome face

uniform vec3 lightColor;

void main()
{
	FragColor = mix(texture(texture1, vec2(TexCoord.x, TexCoord.y)),
					texture(texture2, vec2(TexCoord.x, TexCoord.y)),
					mixValue)
				* vec4(lightColor, 1.0);// * vec4(ourColor, 1.0);
					
	// FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Output solid red to test shader
}