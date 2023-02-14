#pragma once

#include <vulkan/vulkan.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <termcolour.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    // SDL Window
    SDL_Window *vulcano_window;

    // Vulkan Instance
    VkInstance instance;

    // Vulkan Extension Count
    unsigned int vulkan_extensions_count;

    // Vulkan Extensions
    VkExtensionProperties *vulkan_extensions;

    // Vulkan Instance Extensions
    const char** vulkan_instance_extensions;

    // Vulkan Physical Devices
    VkPhysicalDevice *physical_devices;

    // Chosen Vulkan Physical Device
    VkPhysicalDevice *phys_dev;

    // Vulkan Queue Family Properties from a Vulkan's Physical Device
    VkQueueFamilyProperties *queue_family_props;

    // Vulkan's Primary Device
    VkDevice device;
    
    // Vulkan Surface
    VkSurfaceKHR surface;
} vulcano_struct;

/* Function Definitions */
int vulkan_init(vulcano_struct *vulcano_state);
int vulkan_exit(vulcano_struct *vulcano_state);
