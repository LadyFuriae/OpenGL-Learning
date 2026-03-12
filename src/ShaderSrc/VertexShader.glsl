#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

//layout (location = 1) in vec3 aColor;
uniform mat4 Trans;
uniform float Timer;
uniform float xCoordinate;
uniform float yCoordinate;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


out vec3 FinalColor;
out vec2 aTexPos;

void main()
{
    //gl_Position = Trans * vec4(aPos, 1.0f);

    gl_Position = Projection * View * Model * Trans * vec4(aPos, 1.0);
                  //Diff  //ChangeFreq   //Normalizer
    float r = sin(aPos.x + Timer * 1.3) * 0.5 + 0.5;
    float g = cos(aPos.y + Timer * 1.8) * 0.5 + 0.5;
    float b = sin(aPos.z + Timer * 1.7) * 0.5 + 0.5; 
    FinalColor = vec3(r,g,b);
    aTexPos = aTexCoord;
}