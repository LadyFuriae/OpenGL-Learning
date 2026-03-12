#define STB_IMAGE_IMPLEMENTATION
#include <texture.h>
#include<stb-master/stb_image.h>
#include<SDL3/SDL.h>
#define BASE_MIPMAP_LEVEL 0
#define LEGACY_VALUE  0

void LoadTexture(Texture* texture, const char* path)
{
    stbi_set_flip_vertically_on_load(true);
    texture->data = stbi_load(path, &texture->Width, &texture->Height, &texture->nrChannels, 0);
    if(!texture->data)
    {
        SDL_Log("Failed loading texture");
    }
}

void CreateSample(const int Program_ID, const char* SamplerName,const int SamplerNumber)
{
    glUniform1i(glGetUniformLocation(Program_ID, SamplerName), SamplerNumber);
}

void CreateTexture(Texture* texture ,const int Type, const int SamplerNumber, const int NumberOfTextures, const int Format)
{
    glGenTextures(NumberOfTextures, &texture->ID_Texture);
    glActiveTexture(GL_TEXTURE0 + SamplerNumber);
    glBindTexture(Type, texture->ID_Texture);
    texture->Type = Type;
    texture->SamplerID = SamplerNumber;
    texture->Format = Format;
}

void SetTextureParams(const Texture* texture)
{
    glTexParameteri(texture->Type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(texture->Type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(texture->Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texture->Type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void GenerateTexture(Texture* texture)
{
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    //glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    //glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    glTexImage2D(texture->Type, BASE_MIPMAP_LEVEL, texture->Format, texture->Width, texture->Height, LEGACY_VALUE, texture->Format, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(texture->Type);
    stbi_image_free(texture->data);
    texture->data = nullptr;
}