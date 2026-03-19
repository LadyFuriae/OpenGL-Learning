#ifndef GPU_NORMALCUBE_H
#define GPU_NORMALCUBE_H
#include <Shapes/NCVertex.h>

extern Vertex NormalCubeVertices[];

unsigned int PrepareNormalCube(Vertex CubeVertices[], int VerticesCount);
int SetNormalCubeShader();
#endif //GPU_NORMALCUBE_H