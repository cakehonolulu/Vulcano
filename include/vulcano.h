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

    uint32_t vulkan_layer_ext_cnt;

    VkLayerProperties *vulkan_layer_extensions;

    // Vulkan Physical Devices
    VkPhysicalDevice *physical_devices;

    // Chosen Vulkan Physical Device
    VkPhysicalDevice *phys_dev;

    // Vulkan Queue Family Properties from a Vulkan's Physical Device
    VkQueueFamilyProperties *queue_family_props;

    // Vulkan's Primary Device
    VkDevice device;
    
    size_t vk_queue_family_idx;
    size_t vk_queue_mode;

    // Vulkan Surface
    VkSurfaceKHR surface;

    // Vulkan Surface Capabilities
    VkSurfaceCapabilitiesKHR vk_surface_capabilities;

    // Vulkan Surface Format
    VkSurfaceFormatKHR vk_surface_format;
    VkPresentModeKHR vk_surface_chosen_present;

    // Vulkan's Swapchain
    VkSwapchainKHR vk_swapchain;
	VkExtent2D vk_swapchain_chosen;

    uint32_t vk_swapchain_img_num;

    VkImage *vk_swapchain_imgs;
    VkImageView *vk_image_view;

    VkRenderPass vk_render_pass;

    VkFramebuffer *vk_framebuffer;

} vulcano_struct;

/* Function Definitions */
int vulkan_init(vulcano_struct *vulcano_state);
int vulkan_exit(vulcano_struct *vulcano_state);
