#version 460 core

//struct Material
//{
//    vec3 Ambient;
//    vec3 Diffuse;
//    vec3 Specular;
//    float Shininess;
//};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec2 TexCoords;

struct Light
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 LightColor;
uniform vec3 ViewPos;
uniform vec3 LightPos;
uniform vec3 ObjColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 aTexPos;

uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{

    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


        float distance = length(light.position - FragPos);
        float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance * distance));

        vec3 ambient = light.ambient * vec3(texture(material.diffuse, aTexPos));

        vec3 norm = normalize(Normal);
        //vec3 LightDir = normalize(light.position - FragPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, aTexPos));

        vec3 ViewDir = normalize(ViewPos - FragPos);
        vec3 ReflectDir = reflect(light.direction, norm);

        float spec = pow(max(dot(ViewDir, ReflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, aTexPos));

        //vec3 Result = ambient + diffuse + specular;

        //ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        diffuse *= intensity;
        specular *= intensity;

        FragColor = vec4(ambient + diffuse + specular, 1.0);

}
