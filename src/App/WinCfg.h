#ifndef GPU_WINDOW_H
#define GPU_WINDOW_H
#include<SDL3/SDL.h>
#define WIDTH  1920
#define HEIGHT  1080

typedef struct
{
    const char* Title;
    int Width;
    int Height;
    SDL_WindowFlags Flags;
} WinCfg;

WinCfg SetWindow();

#endif //GPU_WINDOW_H