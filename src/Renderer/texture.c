#define STB_IMAGE_IMPLEMENTATION
#include <texture.h>
#include<stb-master/stb_image.h>
#include<SDL3/SDL.h>
#define BASE_MIPMAP_LEVEL 0
#define LEGACY_VALUE  0

void LoadTexture(Texture* texture, const char* path)
{
    //stbi_set_flip_vertically_on_load(true);
    texture->data = stbi_load(path, &texture->Width, &texture->Height, &texture->nrChannels, 0);
    if(!texture->data)
    {
        SDL_Log("Failed loading texture");
    }
}
void GetTexturePath(const char* ModelPath, const char* TexturePath, char* FinalPath) {
    char* name = strrchr(TexturePath, '\\');
    if (!name)
    {
        name = strrchr(TexturePath, '/');
    }
    name++;

    strncpy(FinalPath, ModelPath, strlen(ModelPath));

    char* ptr = strrchr(FinalPath, '\\');
    if (!ptr)
    {
        ptr = strrchr(FinalPath, '/');
    }
    ptr++;
    for (; *name != '\0'; name++, ptr++)
    {
        *ptr = *name;
    }
    *ptr = '\0';
}

void CreateSample(const int Program_ID, const char* SamplerName,const int SamplerNumber)
{
    glUniform1i(glGetUniformLocation(Program_ID, SamplerName), SamplerNumber);
}

void CreateTexture(Texture* texture ,const int Type, const int SamplerNumber, const int NumberOfTextures)
{
    glGenTextures(NumberOfTextures, &texture->ID);
    glActiveTexture(GL_TEXTURE0 + SamplerNumber);
    glBindTexture(Type, texture->ID);
    texture->Type = Type;
    texture->SamplerID = SamplerNumber;
}

void SetTextureParams(const Texture* texture, const GLint Mode)
{
    glTexParameteri(texture->Type, GL_TEXTURE_WRAP_S, Mode);
    glTexParameteri(texture->Type, GL_TEXTURE_WRAP_T, Mode);
    glTexParameteri(texture->Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texture->Type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void GenerateTexture(Texture* texture)
{
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    //glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    //glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(texture->Type, BASE_MIPMAP_LEVEL, texture->InternType, texture->Width, texture->Height, LEGACY_VALUE, texture->Format, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(texture->Type);
    stbi_image_free(texture->data);
    texture->data = nullptr;
}