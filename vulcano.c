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

    return retval;
}

int vk_create_instance(vulcano_struct *vulcano_state)
{
    int retval = 1;
    VkApplicationInfo app_info;
    VkInstanceCreateInfo creation_info;

    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Vulkan Demo";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.apiVersion = VK_API_VERSION_1_0;

    creation_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    creation_info.pApplicationInfo = &app_info;

    unsigned int vulkan_extensions_count = 0;
    SDL_Vulkan_GetInstanceExtensions(vulcano_state->vulcano_window, &vulkan_extensions_count, NULL);

    const char** vulkan_extensions = malloc(sizeof(char*) * vulkan_extensions_count);
    SDL_Vulkan_GetInstanceExtensions(vulcano_state->vulcano_window, &vulkan_extensions_count, vulkan_extensions);

    printf(YELLOW "[vulkan] vk_create_instance: Available extensions...\n" NORMAL);

    for (size_t i = 0; i < vulkan_extensions_count; i++)
    {
        printf(YELLOW "[vulkan] #%lu > %s\n" NORMAL, i, vulkan_extensions[i]);
    }

    creation_info.enabledExtensionCount = vulkan_extensions_count;
    creation_info.ppEnabledExtensionNames = vulkan_extensions;
    creation_info.enabledLayerCount = 0;

    if (vkCreateInstance(&creation_info, NULL, &vulcano_state->instance) == VK_SUCCESS)
    {
        retval = 0;
    }

    return retval;
}
