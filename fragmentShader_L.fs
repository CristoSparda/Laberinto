#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoords;
in float UseTexture;

uniform sampler2D texture_diffuse1;

//codigo para las luces en el material
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 Texture;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    if (int(UseTexture) == 0)
    {
        FragColor = Color;
    }
    else
    {
        //FragColor = texture(texture_diffuse1, TexCoords);

        // ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
}