#version 430 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
// in vec3 LightPos;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader


out vec4 FragColor;

// texture sampler
uniform sampler2D texture1;	// crate
uniform sampler2D texture2;	// awesome face
uniform float mixValue;

uniform vec3 lightPos;	// world-space
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;	// world-space

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
    vec3 viewDir = normalize(viewPos - FragPos); // world-space
    // vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
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
