#pragma once

#include <vulkan/vulkan.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <termcolour.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    VkInstance instance;
    unsigned int vulkan_extensions_count;
    VkExtensionProperties *vulkan_extensions;
    const char** vulkan_instance_extensions;
    SDL_Window *vulcano_window;
} vulcano_struct;

/* Function Definitions */
int vulkan_init(vulcano_struct *vulcano_state);
int vulkan_exit(vulcano_struct *vulcano_state);
