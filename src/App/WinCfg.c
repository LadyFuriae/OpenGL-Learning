#include "WinCfg.h"

WinCfg SetWindow()
{
    const WinCfg Properties =
    {
        .Title  = "OpenGL Learning",
        .Width  = WIDTH,
        .Height = HEIGHT,
        .Flags  = SDL_WINDOW_OPENGL
       };

    return Properties;
}
