#version 430 core

struct Material {
    // vec3 ambient;
    // vec3 diffuse;
    sampler2D diffuse;
    // vec3 specular;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
// in vec3 LightPos;   // extra in variable, since we need the light position in
// view space we calculate this in the vertex shader

out vec4 FragColor;

// texture sampler
uniform sampler2D texture1; // crate
uniform sampler2D texture2; // awesome face
uniform float mixValue;

// uniform vec3 lightPos; // world-space
// uniform vec3 lightColor;
// uniform vec3 objectColor;
uniform vec3 viewPos; // world-space
uniform float time;

// function declarations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // Calculate interpolated textures based on mixValue
    vec4 currentTexture =
        mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);

    // Ambient
    // -------
    // float ambientStrength = 0.1;
    // vec3 ambient = lightColor * ambientStrength;
    // vec3 ambient = light.ambient * material.ambient;
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    // vec3 ambient = light.ambient * vec3(currentTexture);

    // Diffuse
    // -------
    vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(lightPos - FragPos);
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = lightColor * diff;
    // vec3 diffuse = light.diffuse * (diff * material.diffuse);
    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    // vec3 diffuse = light.diffuse * diff * vec3(currentTexture);

    // Specular
    // --------
    // float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos); // world-space
    // vec3 viewDir = normalize(-FragPos); // view-space: the viewer is always
    // at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    // vec3 reflectDir = reflect(-lightDir, norm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    // vec3 specular = lightColor * spec * specularStrength;
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * (spec * material.specular);
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    // Emission
    // --------
    vec3 emission = vec3(0.0);
    /*rough check for blackbox inside spec texture */
    if (texture(material.specular, TexCoord).r == 0.0) {
        /*apply emission texture */
        emission = vec3(texture(material.emission, TexCoord));

        /*some extra fun stuff with "time uniform" */
        emission =
            vec3(texture(material.emission, TexCoord + vec2(0.0, time))); /* moving */
        // emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;              /* fading */
    }

    // Directional light
    // -----------------
    vec3 dir_light = vec3(0.0);
    // vec3 dir_light = CalcDirLight(dirLight, norm, viewDir);

    // phase 2: Point lights
    vec3 point_lights = vec3(0.0);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        point_lights += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // phase 3: Spot light
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    // Result
    // ------
    // vec3 result = (ambient + diffuse + specular) * objectColor;
    // vec3 result = ambient + diffuse + specular;
    // vec3 result = ambient + diffuse + specular + emission;
    // vec3 result = ambient + diffuse + specular + emission + dir_light;
    vec3 result = emission + dir_light + point_lights;

    // FragColor = mix(texture(texture1, vec2(TexCoord.x, TexCoord.y)),
    //                 texture(texture2, vec2(TexCoord.x, TexCoord.y)), mixValue) *
    //             vec4(result, 1.0); // * vec4(ourColor, 1.0);

    FragColor = currentTexture * vec4(result, 1.0);

    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Output solid red to test shader
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    // Calculate interpolated textures based on mixValue
    vec4 currentTexture =
        mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 ambient = light.ambient * vec3(currentTexture);
    vec3 diffuse = light.diffuse * diff * vec3(currentTexture);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // Calculate interpolated textures based on mixValue
    vec4 currentTexture =
        mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));
    // combine results
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 ambient = light.ambient * vec3(currentTexture);
    vec3 diffuse = light.diffuse * diff * vec3(currentTexture);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
