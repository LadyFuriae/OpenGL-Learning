#version 460 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;

uniform float Timer;
uniform float xCoordinate;
uniform float yCoordinate;

out vec3 FinalColor;

void main()
{

    gl_Position = vec4(aPos.x + xCoordinate, aPos.y + yCoordinate, aPos.z, 1.0);
                  //Diff  //ChangeFreq   //Normalizer
    float r = sin(aPos.x + Timer * 1.3) * 0.5 + 0.5;
    float g = cos(aPos.y + Timer * 1.8) * 0.5 + 0.5;
    float b = sin(aPos.z + Timer * 1.7)  * 0.5 + 0.5; 
    FinalColor = vec3(r,g,b);
}