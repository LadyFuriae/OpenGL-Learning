#version 460 core 

out vec4 FragColor;
in vec3 FinalColor;

void main()
{
    //FragColor = vec4(fColor.r, fColor.g, 1.0, 1.0);

    FragColor = vec4(FinalColor, 1.0);
}