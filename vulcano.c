#include <vulcano.h>

int vulkan_init(vulcano_struct *vulcano_state)
{
    int retval = 1;

    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Vulkan Demo";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo creation_info;
    creation_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    creation_info.pApplicationInfo = &app_info;

    unsigned int vulkan_extensions_count = 0;
    SDL_Vulkan_GetInstanceExtensions(vulcano_state->vulcano_window, &vulkan_extensions_count, NULL);

    const char** vulkan_extensions = malloc(sizeof(char*) * vulkan_extensions_count);
    SDL_Vulkan_GetInstanceExtensions(vulcano_state->vulcano_window, &vulkan_extensions_count, vulkan_extensions);

    printf("[vulkan] init: Available extensions:\n");

    for (size_t i = 0; i < vulkan_extensions_count; i++)
    {
        printf("[vulkan]  #%lu > %s\n", i, vulkan_extensions[i]);
    }

    retval = 0;

    return retval;
}

int vulkan_exit(vulcano_struct *vulcano_state)
{
    int retval = 1;

    return retval;
}
