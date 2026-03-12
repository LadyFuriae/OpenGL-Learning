#ifndef GPU_CONTEXT_H
#define GPU_CONTEXT_H
#include <SDL3/SDL.h>

typedef struct
{
    SDL_Window* Window;
    SDL_GLContext GL_Context;
} App;

bool AppInit(App* app);
void BindOpenGLToSDL();
bool BindGLContext(App* app);
void ManageMouse(float* MouseX, float* MouseY);
//void ManageKeyboard(const bool* State);
#endif // GPU_CONTEXT_H