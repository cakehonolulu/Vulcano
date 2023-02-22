#include <vulcano.h>
#include <vk_instance.h>
#include <vk_image.h>
#include <vk_framebuffer.h>
#include <vk_physical.h>
#include <vk_graphics_queue.h>
#include <vk_queue.h>
#include <vk_surface.h>
#include <vk_swapchain.h>
#include <vk_shader.h>
#include <vk_device.h>

int vulkan_init(vulcano_struct *vulcano_state)
{
    int retval = 1;

    bool vulkan_error = false;

    vulcano_state->instance = vk_create_instance(vulcano_state, &vulkan_error);
    
    if (!vulkan_error)
    {
        int phys_dev_idx = vk_pick_physical_device(vulcano_state, &vulkan_error);

        vulcano_state->phys_dev = &vulcano_state->physical_devices[phys_dev_idx];

        if (!vulkan_error && phys_dev_idx != -1)
        {
            uint32_t queue_family_number = vk_queue_get_prop_count_from_device(vulcano_state->phys_dev);

            vk_queue_get_props_from_device(vulcano_state, queue_family_number);

            vulcano_state->device = vk_create_device(vulcano_state, queue_family_number);

            vk_list_device_ext(vulcano_state);

            vulcano_state->vk_queue_family_idx = vk_graphics_queue_get_family_idx(vulcano_state, queue_family_number);

            vulcano_state->vk_queue_mode = vk_graphics_queue_get_mode(vulcano_state, vulcano_state->vk_queue_family_idx);

            VkQueue draw_queue = vk_graphics_queue_get_draw(vulcano_state, vulcano_state->vk_queue_family_idx);

	        VkQueue present_queue = vk_graphics_queue_get_presenting(vulcano_state, vulcano_state->vk_queue_family_idx, vulcano_state->vk_queue_mode);

            SDL_Vulkan_CreateSurface(vulcano_state->vulcano_window,
                (SDL_vulkanInstance) vulcano_state->instance, (SDL_vulkanSurface *) &vulcano_state->surface);

            VkBool32 vk_surface_support = 0;
	        
            vkGetPhysicalDeviceSurfaceSupportKHR(*vulcano_state->phys_dev, vulcano_state->vk_queue_family_idx, vulcano_state->surface, &vk_surface_support);

            if (vk_surface_support)
            {
                vk_surface_prepare(vulcano_state);

                vk_swapchain_create(vulcano_state);

                vk_image_prepare(vulcano_state);

                vk_framebuffer_prepare(vulcano_state);

                vk_framebuffer_create(vulcano_state);

                uint32_t vertexShaderSize = 0;
	            char vertexShaderFileName[] = "triangle.vert";
	            char *vertexShaderCode = getShaderCode(vertexShaderFileName, &vertexShaderSize);

                if (vertexShaderCode != NULL)
                {
                    VkShaderModule vertexShaderModule = createShaderModule(vulcano_state, vertexShaderCode, vertexShaderSize);
                    
                    printf(MAGENTA BOLD "[vulkan] init: Vertex Shader Compiled Successfully!" NORMAL "\n");

                    uint32_t fragmentShaderSize = 0;
                    char fragmentShaderFileName[] = "triangle.frag";
                    char *fragmentShaderCode = getShaderCode(fragmentShaderFileName, &fragmentShaderSize);

                    if (fragmentShaderCode != NULL)
                    {
                        VkShaderModule fragmentShaderModule = createShaderModule(vulcano_state, fragmentShaderCode, fragmentShaderSize);
                        
                        printf(MAGENTA BOLD "[vulkan] init: Fragment Shader Compiled Successfully!" NORMAL "\n");

                        retval = 0;
                    }
                    else
                    {
                        printf(RED "[vulkan] init: Couldn't compile the fragment shaders!" NORMAL "\n");
                    }
                }
                else
                {
                    printf(RED "[vulkan] init: Couldn't compile the vertex shaders!" NORMAL "\n");
                }
            }
            else
            {
                printf(RED "[vulkan] init: Platform doesn't have VkSurfaceKHR support, exiting..." NORMAL "\n");
            }
        }
        else
        {
            printf(RED "[vulkan] init: Failed to pick a compute device, exiting..." NORMAL "\n");
        }
    }
    else
    {
        printf(RED "[vulkan] init: Failed to create Vulkan Instance, exiting..." NORMAL "\n");
    }

    return retval;
}

int vulkan_exit(vulcano_struct *vulcano_state)
{
    int retval = 1;

    if (vulcano_state->vk_swapchain_imgs)
        free(vulcano_state->vk_swapchain_imgs);

    if (vulcano_state->vk_swapchain)
        vkDestroySwapchainKHR(vulcano_state->device, vulcano_state->vk_swapchain, NULL);

    if (vulcano_state->surface)
        vkDestroySurfaceKHR(vulcano_state->instance, vulcano_state->surface, NULL);

    if (vulcano_state->device)
        vkDestroyDevice(vulcano_state->device, NULL);

    if (vulcano_state->instance)
        vkDestroyInstance(vulcano_state->instance, NULL);

    if (vulcano_state->queue_family_props)
        free(vulcano_state->queue_family_props);

    if (vulcano_state->physical_devices)
        free(vulcano_state->physical_devices);

    if (vulcano_state->vulkan_instance_extensions)
        free(vulcano_state->vulkan_instance_extensions);

    retval = 0;

    return retval;
}
