#ifndef GPU_SHADER_H
#define GPU_SHADER_H
#include <stdio.h>
#include "glad/glad.h"

typedef struct
{
    int ID;
    GLenum Type;
} Shader;

bool CreateShader(Shader* src, const char* FileName, GLenum type);
bool CheckShaderComp(const Shader* src);
bool LinkShaderProgram(int ID_Program);
int ManageShaders();

//void AddShadersToProgram(int ShProgram, int ShaderIDs[], int Count);




#endif //GPU_SHADER_H