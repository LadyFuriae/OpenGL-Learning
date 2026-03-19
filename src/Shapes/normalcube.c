#include<normalcube.h>
#include <glad/glad.h>
#include <Renderer/model.h>
#include <Shader/Shader.h>
#define BUFFER_OBJECT_COUNT 1
#define aPOS_ID 0
#define aNORMAL_ID 1
#define aColor_ID 1
#define aTexPos_ID 2
#define FIGURE_COMPONENT_NUM 3
#define TEX_COMPONENT_NUM 2
#define NORMAL_COMPONENTS_NUM 3

Vertex NormalCubeVertices[] = {
    // Posiciones (x,y,z)          // Normales (nx,ny,nz)       // UVs (t,s)
    // Cara Trasera
    {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f},
    { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f},
    { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f},
    { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f},
    {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f},
    {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f},

    // Cara Frontal
    {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f},
    { 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f},
    {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f},
    {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f},

    // Cara Izquierda
    {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f},
    {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f},
    {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f},
    {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f},

    // Cara Derecha
    { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f},
    { 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f},
    { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f},
    { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f},
    { 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f},

    // Cara Inferior
    {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f},
    { 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f},
    { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f},
    { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f},
    {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f},

    // Cara Superior
    {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f},
    { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f},
    {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f},
    {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f}
};
unsigned int PrepareNormalCube(Vertex CubeVertices[], int VerticesCount)
{

    unsigned int ID_VAO = GenAndBindVAO();
    //int ID_VBO;
    //int ID_VBO =
    GenAndBindVBO();
    //unsigned int EBO = GenAndBindEBO();
    GenAndBindEBO();
    //Create buffer to the GPU
    glBufferData(GL_ARRAY_BUFFER,(long)(VerticesCount * sizeof(Vertex)), CubeVertices, GL_STATIC_DRAW);
    //Bind EBO to the GL_ARRAY_BUFFER
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesCount, Indices, GL_STATIC_DRAW);
    glVertexAttribPointer(aPOS_ID, FIGURE_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(aPOS_ID);
    //glVertexAttribPointer(aTexPos_ID, TEX_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(aTexPos_ID);
    glVertexAttribPointer(aNORMAL_ID, NORMAL_COMPONENTS_NUM, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(aNORMAL_ID);
    glVertexAttribPointer(aTexPos_ID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(aTexPos_ID);
    //glVertexAttribPointer(aColor_ID, FIGURE_FIGURE_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(aColor_ID);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return ID_VAO;
}

int SetNormalCubeShader()
{
    Shader CubeVS;
    if (!CreateShader(&CubeVS, "../src/ShaderSrc/VertexShader.glsl", GL_VERTEX_SHADER))
        return -1;

    Shader CubeFS;
    if (!CreateShader(&CubeFS, "../src/ShaderSrc/CubeFG.glsl", GL_FRAGMENT_SHADER))
        return -1;

    glCompileShader(CubeVS.ID);
    if (!CheckShaderComp(&CubeVS))
        return -1;

    glCompileShader(CubeFS.ID);
    if (!CheckShaderComp(&CubeFS))
        return -1;

    int ID_Program = glCreateProgram();
    glAttachShader(ID_Program, CubeVS.ID);
    glAttachShader(ID_Program, CubeFS.ID);

    glLinkProgram(ID_Program);
    glDeleteShader(CubeVS.ID);
    glDeleteShader(CubeFS.ID);

    return ID_Program;
}
