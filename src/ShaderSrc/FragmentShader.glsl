#version 460 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
//in vec3 FinalColor;
in vec2 aTexPos;

layout (binding = 0) uniform sampler2D TexDiff;
layout (binding = 1) uniform sampler2D TexSpecular;

uniform vec3 ViewPos;

uniform vec3 ObjColor;
uniform vec3 Specular;
uniform float Shininess;
uniform float alphaCutOff;

uniform vec3 LightColor;
uniform vec3 LightPosition;
void main()
{
    //FragColor = vec4(fColor.r, fColor.g, 1.0, 1.0);

    //FragColor = mix(texture(Texture1, aTexPos), texture(Texture2, aTexPos) * vec4(FinalColor, 1.0), 0.4);
    //FragColor = mix(texture(Texture1, aTexPos), texture(Texture2, aTexPos) * vec4(FinalColor, 1.0), texture(Texture2, aTexPos).a * 0.5);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * LightColor;
    float ambientStregt = 0.2;

    float SpecularStr = 0.5;
    vec3 ViewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(ViewDir, reflectDir), 0.0), Shininess);
    float SpecMask = texture(TexSpecular, aTexPos).r;
    vec3 FinalSpec = Specular * spec * LightColor * SpecMask;

    vec3 result = vec3((ambientStregt * LightColor) + diffuse + FinalSpec);
    vec4 texColor = texture(TexDiff, aTexPos);

    if(texColor.a < 0.3)
    {
        discard;
    }

    FragColor = vec4(texColor.rgb * result , texColor.a);
}