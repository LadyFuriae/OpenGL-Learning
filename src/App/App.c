#include "App.h"
#include <WinCfg.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include "camera.h"
#include <cglm/cglm.h>
#include <assert.h>

bool AppInit(App* app)
{
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("\nFailed to initialize SDL device %s\n", SDL_GetError());
        return false;
    }

    printf("SDL device ready!");

    const WinCfg WindowCfg = SetWindow();
    app->Window = SDL_CreateWindow(WindowCfg.Title, WindowCfg.Width, WindowCfg.Height, WindowCfg.Flags);

    if(!app->Window)
    {
        SDL_Log("\nFailed to create SDL window: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void BindOpenGLToSDL()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

bool BindGLContext(App* app)
{
    app->GL_Context = SDL_GL_CreateContext(app->Window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_Log("GLAD initilization failed");
        SDL_Quit();
        return false;
    }
    return true;
}

