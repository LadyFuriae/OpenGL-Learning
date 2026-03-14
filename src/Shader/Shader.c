#include "Shader.h"
#include <SDL3/SDL.h>
#include <assert.h>

#define INFO_LOG_SIZE 512

bool CreateShader(Shader* src, const char* FileName, const GLenum type)
{
    FILE* file = fopen(FileName, "r");

    if (!file)
    {
        SDL_Log("Error opening shader %s", FileName);
        assert(file);
        return false;
    }

    fseek(file, 0, SEEK_END);

    char Data[ftell(file)+1];
    memset(Data, 0, sizeof(Data));

    rewind(file);
    fread(Data,  1, sizeof(Data), file);
    fclose(file);

    //printf("\nShader:\n %s", Data);

    src->Type = type;
    const int ShaderID = glCreateShader(src->Type);

    glShaderSource(ShaderID, 1, (const GLchar*[]){ Data }, nullptr);
    src->ID = ShaderID;
    return true;
}

bool CheckShaderComp(const Shader* src)
{
    char infoLog[INFO_LOG_SIZE];
    int Comp;
    glGetShaderiv(src->ID, GL_COMPILE_STATUS, &Comp);
    if(!Comp)
    {
        glGetShaderInfoLog(src->ID, INFO_LOG_SIZE, nullptr, infoLog);
        SDL_Log("Shader compilation failed\nShader ID: %d\nLog: %s\n", src->ID, infoLog);
        assert(Comp);
        return false;
    }
    SDL_Log("Shader created succesfully!\n");

    return true;
}


bool LinkShaderProgram(const int ID_Program)
{
    int Linked;
    char infoLog[512];
    glLinkProgram(ID_Program);
    glGetProgramiv(ID_Program, GL_LINK_STATUS, &Linked);

    if(!Linked)
    {
        glGetProgramInfoLog(ID_Program, INFO_LOG_SIZE, nullptr, infoLog);
        SDL_Log("Shader Program linkage failed.\nProgram ID: %d\nLog: %s\n", ID_Program, infoLog);
        assert(Linked);
        return false;
    }

    return true;
}

int SetCubeShader()
{
    Shader VertexShader;

    if (!CreateShader(&VertexShader, "../src/ShaderSrc/VertexShader.glsl", GL_VERTEX_SHADER))
        return -1;

    Shader FragmentShader;
    if (!CreateShader(&FragmentShader, "../src/ShaderSrc/FragmentShader.glsl", GL_FRAGMENT_SHADER))
        return -1;


    glCompileShader(VertexShader.ID);
    if (!CheckShaderComp(&VertexShader))
        return -1;

    glCompileShader(FragmentShader.ID);
    if (!CheckShaderComp(&FragmentShader))
        return -1;

    int ID_Program = glCreateProgram();
    glAttachShader(ID_Program, VertexShader.ID);
    glAttachShader(ID_Program, FragmentShader.ID);

    LinkShaderProgram(ID_Program);
    glDeleteShader(VertexShader.ID);
    glDeleteShader(FragmentShader.ID);

    return ID_Program;
}

int SetLightShader()
{
    Shader VertexShader;

    if (!CreateShader(&VertexShader, "../src/ShaderSrc/VertexShader.glsl", GL_VERTEX_SHADER))
        return -1;

    Shader FragmentShader;
    if (!CreateShader(&FragmentShader, "../src/ShaderSrc/LightFG.glsl", GL_FRAGMENT_SHADER))
        return -1;

    glCompileShader(VertexShader.ID);
    if (!CheckShaderComp(&VertexShader))
        return -1;

    glCompileShader(FragmentShader.ID);
    if (!CheckShaderComp(&FragmentShader))
        return -1;

    int ID_Program = glCreateProgram();
    glAttachShader(ID_Program, VertexShader.ID);
    glAttachShader(ID_Program, FragmentShader.ID);

    LinkShaderProgram(ID_Program);
    glDeleteShader(VertexShader.ID);
    glDeleteShader(FragmentShader.ID);

    return ID_Program;

}