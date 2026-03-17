#ifndef GPU_MATERIAL_H
#define GPU_MATERIAL_H
#include <texture.h>

typedef struct
{
    Texture* Textures;
    unsigned int TexCount;
    float DiffCol[3];
    float SpecCol[3];
    float Shininess;
    float AlphaCutoff;
    bool HasBlend;
    GLint TexMode;
} Material;
#endif //GPU_MATERIAL_H