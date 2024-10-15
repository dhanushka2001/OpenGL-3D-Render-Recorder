#version 430 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;	// crate
uniform sampler2D texture2;	// awesome face

void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x, TexCoord.y)), 0.2) * vec4(ourColor, 1.0); 
}