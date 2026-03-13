#version 460 core 

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
//in vec3 FinalColor;
//in vec2 aTexPos;

//uniform sampler2D Texture1;
//uniform sampler2D Texture2;

uniform vec3 ViewPos;


uniform vec3 ObjColor;
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

    float spec = pow(max(dot(ViewDir, reflectDir), 0.0), 256);
    vec3 specular = SpecularStr * spec * LightColor;


    vec3 result = vec3(ambientStregt + diffuse + spec);
    FragColor = vec4(result * ObjColor, 1.0);
}