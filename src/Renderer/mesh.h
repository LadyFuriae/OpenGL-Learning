#ifndef GPU_MESH_H
#define GPU_MESH_H
#include <texture.h>
#include <vertex.h>
#include <material.h>
typedef struct
{
    unsigned int* Indices;
    MeshVertex* Vertices;
    unsigned int VerticesCount;
    unsigned int numIndices;
    Material Mat;
    unsigned int VAO;
}Mesh;

#endif //GPU_MESH_H