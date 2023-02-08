#include <vulkan/vulkan.h>
#include <vulcano.h>
#include <termcolour.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char **argv)
{
    int retval = 1;
    bool run = true;

    vulcano_struct *vulcano_state;

    // SDL Event
    SDL_Event vulcano_event;

    printf("[vulcano] main: Vulcano 0.1\n");
    printf("[vulcano] main: Compiled on %s %s\n", __DATE__, __TIME__);
    printf("[sdl] main: Initializing SDL...\n");
    
    vulcano_state = malloc(sizeof(vulcano_struct));

    if (vulcano_state != NULL)
    {
        if (SDL_Init(SDL_INIT_VIDEO) == 0)
        {
            vulcano_state->vulcano_window = SDL_CreateWindow(
            "Vulcano (SDL2) - Vulkan",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_VULKAN
            );

            if (vulcano_state->vulcano_window != NULL)
            {
                if (vulkan_init(vulcano_state) == 0)
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
                                            vulkan_exit(vulcano_state);
                                            SDL_Quit();
                                            free(vulcano_state);
                                            run = false;
                                            printf("[vulcano] main: User-requested exit\n");
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
                    printf(RED "[vulcano] main: Failed to initialize Vulkan, exiting...\n" NORMAL);
                    SDL_Quit();
                    free(vulcano_state);
                }
            }
            else
            {
                printf(RED "[sdl] main: Failed to create the SDL window, exiting...\n" NORMAL);
                SDL_Quit();
                free(vulcano_state);
            }
        }
        else
        {
            printf(RED "[sdl] main: Failed to initialize SDL, exiting...\n" NORMAL);
        }
    }
    else
    {
        printf(RED "[vulcano] main: Failed to allocate Vulcano's state structure, exiting...\n" NORMAL);
    }

    return retval;
}
