#ifndef GPU_LIGHCUBE_H
#define GPU_LIGHCUBE_H

typedef struct
{
    float x, y, z;
    // float r, g, b;
    //float t, s;
    float nx, ny, nz;
} Vertex;

extern Vertex CubeVertices[];

unsigned int PrepareLightCube(Vertex CubeVertices[], int VerticesCount);




#endif //GPU_LIGHCUBE_H