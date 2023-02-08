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
        printf(RED "[vulkan] init: Failed to create Vulkan Instance, exiting...5" NORMAL "\n");
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
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };

    // Find how many extensions the instance will have using Vulkan API functions
    if (vkEnumerateInstanceExtensionProperties(NULL, &vulcano_state->vulkan_extensions_count, NULL) != VK_SUCCESS)
    {
        printf(RED "[vulkan] vk_create_instance: Failed to get Vulkan Extensions count, exiting..." NORMAL "\n");
        *vulkan_error = true;
        goto vk_create_instance_end;
    }

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

    const char vulkan_wanted_layers[][VK_MAX_EXTENSION_NAME_SIZE] = {
		"VK_LAYER_KHRONOS_validation"
	};

	const char *vulkan_layers[] = {
		vulkan_wanted_layers[0]
	};

    VkInstanceCreateInfo creation_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount =  vulcano_state->vulkan_extensions_count,
        .ppEnabledExtensionNames = (const char *const *) vulcano_state->vulkan_extensions,
    };

    // Check if we have Vulkan Validation Layer Support
    for (size_t i = 0; i < vulcano_state->vulkan_extensions_count; i++)
    {
        if (strcmp(vulcano_state->vulkan_extensions[i].extensionName, vulkan_wanted_layers[0]) == 0)
        {
            creation_info.enabledLayerCount = 1;
            creation_info.ppEnabledLayerNames = vulkan_layers;
            printf(YELLOW "[vulkan] vk_create_instance: Enabling Vulkan Validation Layers..." NORMAL "\n");
        }
    }

    if (vkCreateInstance(&creation_info, NULL, &ret) != VK_SUCCESS)
    {
        printf(YELLOW "[vulkan] vk_create_instance: vkCreateInstance failed..." NORMAL "\n");
        *vulkan_error = true;
    }

vk_create_instance_end:
    return ret;
}

VkResult vulkan_check_for_validation_layer(vulcano_struct *vulcano_state)
{

}
