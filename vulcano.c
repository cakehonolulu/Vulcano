#include <vulcano.h>

int vulkan_init(vulcano_struct *vulcano_state)
{
    int retval = 1;

    bool vulkan_error = false;

    vulcano_state->instance = vk_create_instance(vulcano_state, &vulkan_error);
    
    if (!vulkan_error)
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

VkInstance vk_create_instance(vulcano_struct *vulcano_state, bool *vulkan_error)
{
    VkInstance ret;
    
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

    const char layerList[][VK_MAX_EXTENSION_NAME_SIZE] = {
		"VK_LAYER_KHRONOS_validation"
	};
	const char *layers[] = {
		layerList[0]
	};

    VkInstanceCreateInfo creation_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = layers,
        .enabledExtensionCount =  vulcano_state->vulkan_extensions_count,
        .ppEnabledExtensionNames = (const char *const *) vulcano_state->vulkan_extensions,
    };

    printf(YELLOW "[vulkan] vk_create_instance: Available extensions...\n" NORMAL);

    for (size_t i = 0; i < vulcano_state->vulkan_extensions_count; i++)
    {
        printf(YELLOW "[vulkan] #%lu > %s\n" NORMAL, i, vulcano_state->vulkan_extensions[i]);
    }

    if (vkCreateInstance(&creation_info, NULL, &ret) != VK_SUCCESS)
    {
        printf(YELLOW "[vulkan] vk_create_instance: vkCreateInstance failed...\n" NORMAL);
        *vulkan_error = true;
    }
    
    return ret;
}
