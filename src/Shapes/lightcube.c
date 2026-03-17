#include<lightcube.h>
#include <glad/glad.h>
#include <Renderer/model.h>
#define BUFFER_OBJECT_COUNT 1
#define aPOS_ID 0
#define aNORMAL_ID 1
#define aColor_ID 1
#define aTexPos_ID 2
#define FIGURE_COMPONENT_NUM 3
#define TEX_COMPONENT_NUM 2
#define NORMAL_COMPONENTS_NUM 3

Vertex CubeVertices[] =
{
    {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
    { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
    { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
    { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
    {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
    {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},

    {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
    { 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
    {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
    {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},

    {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
    {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
    {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
    {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},

    { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
    { 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
    { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
    { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
    { 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
    { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},

    {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
    { 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
    { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
    { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
    {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
    {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},

    {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
    { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
    { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
    {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
    {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f}
};


unsigned int PrepareLightCube(Vertex CubeVertices[], int VerticesCount)
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
    //glVertexAttribPointer(aColor_ID, FIGURE_FIGURE_COMPONENT_NUM, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(aColor_ID);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return ID_VAO;
}

