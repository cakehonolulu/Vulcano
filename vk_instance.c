#include <vk_instance.h>

VkInstance vk_create_instance(vulcano_struct *vulcano_state, bool *vulkan_error)
{
    VkInstance ret = {0};
    size_t vulkan_extensions_size = 0;
   
    VkApplicationInfo app_info = {0};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = "Vulkan Demo";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    // Find how many extensions the instance will have using Vulkan API functions
    if (vkEnumerateInstanceExtensionProperties(NULL, &vulcano_state->vulkan_extensions_count, NULL) != VK_SUCCESS)
    {
        printf(RED "[vulkan] vk_create_instance: Failed to get Vulkan Extensions count, exiting..." NORMAL "\n");
        *vulkan_error = true;
        goto vk_create_instance_end;
    }

    vulkan_extensions_size = vulcano_state->vulkan_extensions_count;

    // Allocate space for the extensions information
    vulcano_state->vulkan_extensions = malloc(sizeof(VkExtensionProperties) * vulcano_state->vulkan_extensions_count);

    // Get the actual available extensions
    if (vkEnumerateInstanceExtensionProperties(NULL, &vulcano_state->vulkan_extensions_count, vulcano_state->vulkan_extensions) != VK_SUCCESS)
    {
        printf(RED "[vulkan] vk_create_instance: Failed to obtain Vulkan Extension names, exiting..." NORMAL "\n");
        *vulkan_error = true;
        goto vk_create_instance_end;
    }

    printf(YELLOW "[vulkan] vk_create_instance: Listing %d available extensions..." NORMAL "\n", vulcano_state->vulkan_extensions_count);

    for (size_t i = 0; i < vulcano_state->vulkan_extensions_count; i++)
    {
        printf(YELLOW "[vulkan] #%lu > %s (Ver. %d)" NORMAL "\n", i, vulcano_state->vulkan_extensions[i].extensionName, vulcano_state->vulkan_extensions[i].specVersion);
    }

    if (SDL_Vulkan_GetInstanceExtensions(vulcano_state->vulcano_window, &vulcano_state->vulkan_extensions_count, NULL) != SDL_TRUE)
    {
        printf(RED "[vulkan] vk_create_instance: Failed to obtain SDL Instance Extensions count, exiting..." NORMAL "\n");
        *vulkan_error = true;
        goto vk_create_instance_end;
    }

    vulcano_state->vulkan_instance_extensions = malloc(sizeof(char *) * vulcano_state->vulkan_extensions_count);
    
    if (SDL_Vulkan_GetInstanceExtensions(vulcano_state->vulcano_window, &(vulcano_state->vulkan_extensions_count), vulcano_state->vulkan_instance_extensions) != SDL_TRUE)
    {
        printf(RED "[vulkan] vk_create_instance: Failed to obtain SDL Instance Extension names, exiting..." NORMAL "\n");
        *vulkan_error = true;
        goto vk_create_instance_end;
    }

    const char vulkan_wanted_layers[][VK_MAX_EXTENSION_NAME_SIZE] = {
		"VK_LAYER_KHRONOS_validation"
	};

	const char *vulkan_layers[] = {
		vulkan_wanted_layers[0]
	};

    VkInstanceCreateInfo creation_info = {0};
    creation_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    creation_info.pNext = NULL;
    creation_info.pApplicationInfo = &app_info;
    creation_info.enabledLayerCount = 0;
    creation_info.ppEnabledLayerNames = NULL;
    creation_info.enabledExtensionCount =  vulcano_state->vulkan_extensions_count;
    creation_info.ppEnabledExtensionNames = vulcano_state->vulkan_instance_extensions;

    // Check if we have Vulkan Validation Layer Support
    for (size_t i = 0; i < vulkan_extensions_size; i++)
    {
        if (strcmp(vulcano_state->vulkan_extensions[i].extensionName, vulkan_wanted_layers[0]) == 0)
        {
            creation_info.enabledLayerCount = 1;
            creation_info.ppEnabledLayerNames = vulkan_layers;
            printf(YELLOW "[vulkan] vk_create_instance: Enabling Vulkan Validation Layers..." NORMAL "\n");
        }
    }

    free(vulcano_state->vulkan_extensions);

    if (vkCreateInstance(&creation_info, NULL, &ret) != VK_SUCCESS)
    {
        printf(YELLOW "[vulkan] vk_create_instance: vkCreateInstance failed..." NORMAL "\n");
        *vulkan_error = true;
    }

vk_create_instance_end:
    return ret;
}
