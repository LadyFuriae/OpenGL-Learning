#include<glad/glad.h>

typedef struct
{
    unsigned char* data;
    int Width;
    int Height;
    int nrChannels;
    unsigned int ID_Texture;
    int Type;
    int Format;
    int SamplerID;
}Texture;

void LoadTexture(Texture* texture, const char* path);
void CreateSample(int Program_ID, const char* SamplerName, int SamplerNumber);
void CreateTexture(Texture* texture, int Type, int SamplerNumber, int NumberOfTextures, int Format);
void SetTextureParams(const Texture* texture);
void GenerateTexture(Texture* texture);
