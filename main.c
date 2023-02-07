#include <SDL.h>
#include <vulkan/vulkan.h>
#include <termcolour.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char **argv)
{
    int retval = 1;
    bool run = true;

    // SDL Window
    SDL_Window *vulcano_window = NULL;

    // SDL Event
    SDL_Event vulcano_event;

    printf("[vulcano] main: Vulcano 0.1\n");
    printf("[vulcano] main: Compiled on %s %s\n", __DATE__, __TIME__);
    printf("[sdl] main: Initializing SDL...\n");
    
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        vulcano_window = SDL_CreateWindow(
            "Vulcano (SDL2) - Vulkan",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_VULKAN
            );

        if (vulcano_window != NULL)
        {
            while (run)
            {
                while (SDL_PollEvent(&vulcano_event))
                {
                    switch (vulcano_event.type)
                    {
                        case SDL_WINDOWEVENT:
                        {
                            switch (vulcano_event.window.event)
                            {
                                case SDL_WINDOWEVENT_CLOSE:
                                    SDL_Quit();
                                    run = false;
                                    break;
                            }
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            printf(RED "[sdl] main: Failed to create the SDL window, exiting...\n" NORMAL);
        }
    }
    else
    {
        printf(RED "[sdl] main: Failed to initialize SDL, exiting...\n" NORMAL);
    }

    return retval;
}
