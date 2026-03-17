#ifndef TEXTURE_H
#define TEXTURE_H
#include<glad/glad.h>

enum TextureType
{
    NONE,
    ALBEDO,
    SPECULAR,
    TEXUNKOWN,
};

typedef struct
{
    unsigned char* data;
    unsigned int ID;
    int SamplerID;
    int Width;
    int Height;
    int nrChannels;
    GLint Type;
    GLint InternType;
    GLint Format;
    enum TextureType CHANGE_THIS_SHIT;
    char path[256];
}Texture;

void LoadTexture(Texture* texture, const char* path);
void GetTexturePath(const char* ModelPath, const char* TexturePath, char* FinalPath);
void CreateSample(int Program_ID, const char* SamplerName, int SamplerNumber);
void CreateTexture(Texture* texture, int Type, int SamplerNumber, int NumberOfTextures);
void SetTextureParams(const Texture* texture, GLint Mode);
void GenerateTexture(Texture* texture);

#endif //TEXTURE_H