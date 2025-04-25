#version 430 core
#define RENDER_3D	1

#if RENDER_3D==0
in vec3 ourColor;
#endif

#if RENDER_3D==1
in vec3 FragPos;
in vec3 Normal;
#endif

in vec2 TexCoord;

out vec4 FragColor;

uniform float mixValue;
// texture sampler
uniform sampler2D texture1;	// crate
uniform sampler2D texture2;	// awesome face

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
				* vec4(result, 1.0);// * vec4(ourColor, 1.0);
					
	// FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Output solid red to test shader
}
