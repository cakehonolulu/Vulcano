#include <vulkan/vulkan.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <termcolour.h>
#include <stdint.h>

typedef struct {
    VkInstance instance;
    SDL_Window *vulcano_window;
} vulcano_struct;

/* Function Definitions */
int vulkan_init(vulcano_struct *vulcano_state);
int vulkan_exit(vulcano_struct *vulcano_state);

int vk_create_instance(vulcano_struct *vulcano_state);
