#include <vulkan/vulkan.h>
#include <SDL.h>
#include <stdint.h>

typedef struct {
    VkInstance instance;
    SDL_Window *vulcano_window;
} vulcano_struct;

/* Function Definitions */
int vulkan_init(vulcano_struct *vulcano_state);
int vulkan_exit(vulcano_struct *vulcano_state);
