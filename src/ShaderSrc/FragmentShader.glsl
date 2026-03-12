#version 460 core 

out vec4 FragColor;
in vec3 FinalColor;
in vec2 aTexPos;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main()
{
    //FragColor = vec4(fColor.r, fColor.g, 1.0, 1.0);

    //FragColor = mix(texture(Texture1, aTexPos), texture(Texture2, aTexPos) * vec4(FinalColor, 1.0), 0.4); 
    FragColor = mix(texture(Texture1, aTexPos), texture(Texture2, aTexPos) * vec4(FinalColor, 1.0), texture(Texture2, aTexPos).a * 0.5);
}