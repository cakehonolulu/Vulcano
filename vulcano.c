#include <vulcano.h>

int vulkan_init(vulcano_struct *vulcano_state)
{
    int retval = 1;

    if (vk_create_instance(vulcano_state) == 0)
    {
        retval = 0;
    }
    else
    {
        printf(RED "[vulkan] init: Failed to create Vulkan Instance, exiting...\n" NORMAL);
    }

    return retval;
}

int vulkan_exit(vulcano_struct *vulcano_state)
{
    int retval = 1;

    if (vulcano_state->vulkan_extensions)
        free(vulcano_state->vulkan_extensions);

    retval = 0;

    return retval;
}

int vk_create_instance(vulcano_struct *vulcano_state)
{
    int retval = 1;
    
    const VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "Vulkan Demo",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_0,
    };

    // Find how many extensions the instance will have using SDL
    SDL_Vulkan_GetInstanceExtensions(vulcano_state->vulcano_window, &vulcano_state->vulkan_extensions_count, NULL);

    // Allocate space for the extensions information
    vulcano_state->vulkan_extensions = malloc(sizeof(char*) * vulcano_state->vulkan_extensions_count);

    // Get the actual available extensions
    SDL_Vulkan_GetInstanceExtensions(vulcano_state->vulcano_window, &(vulcano_state->vulkan_extensions_count), vulcano_state->vulkan_extensions);

    VkInstanceCreateInfo creation_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount =  vulcano_state->vulkan_extensions_count,
        .ppEnabledExtensionNames = (const char *const *) vulcano_state->vulkan_extensions,
    };

    printf(YELLOW "[vulkan] vk_create_instance: Available extensions...\n" NORMAL);

    for (size_t i = 0; i < vulcano_state->vulkan_extensions_count; i++)
    {
        printf(YELLOW "[vulkan] #%lu > %s\n" NORMAL, i, vulcano_state->vulkan_extensions[i]);
    }

    if (vkCreateInstance(&creation_info, NULL, &vulcano_state->instance) == VK_SUCCESS)
    {
        retval = 0;
    }

    return retval;
}
