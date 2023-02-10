#include <vk_device.h>

VkDevice vk_create_device(vulcano_struct *vulcano_state, uint32_t prop_count)
{
    VkDevice device = { 0 };

    VkDeviceQueueCreateInfo *dev_queue_info = malloc(sizeof(VkDeviceQueueCreateInfo) * prop_count);
	float **dev_queue_prio = malloc(sizeof(float *) * prop_count);

	for (uint32_t i = 0; i < prop_count; i++)
    {
		dev_queue_prio[i] = malloc(sizeof(float) * vulcano_state->queue_family_props[i].queueCount);

		for (uint32_t j = 0; j < vulcano_state->queue_family_props[i].queueCount; j++)
        {
			dev_queue_prio[i][j] = 1.0f;
		}

		dev_queue_info[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		dev_queue_info[i].pNext = NULL;
		dev_queue_info[i].flags = 0;
		dev_queue_info[i].queueFamilyIndex = i;
		dev_queue_info[i].queueCount = vulcano_state->queue_family_props[i].queueCount;
		dev_queue_info[i].pQueuePriorities = dev_queue_prio[i];
	}

    const char vulkan_wanted_vulkan_extensions[][VK_MAX_EXTENSION_NAME_SIZE] = {
		"VK_KHR_swapchain"
	};
	
    const char *vulkan_extensions[] = {
		vulkan_wanted_vulkan_extensions[0]
	};

	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(*vulcano_state->phys_dev, &physicalDeviceFeatures);

	VkDeviceCreateInfo deviceCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		NULL,
		0,
		prop_count,
		dev_queue_info,
		0,
		NULL,
		1,
		vulkan_extensions,
		&physicalDeviceFeatures
	};

	vkCreateDevice(*vulcano_state->phys_dev, &deviceCreateInfo, NULL, &device);

	for (uint32_t i = 0; i < prop_count; i++)
    {
		free(dev_queue_prio[i]);
	}

    if (dev_queue_prio)
        free(dev_queue_prio);

    if (dev_queue_info)
        free(dev_queue_info);

    return device;
}

void vk_list_device_ext(vulcano_struct *vulcano_state)
{
    vkEnumerateDeviceExtensionProperties(*vulcano_state->phys_dev, NULL, &vulcano_state->vulkan_extensions_count, NULL);

    vulcano_state->vulkan_extensions = malloc(sizeof(VkExtensionProperties) * vulcano_state->vulkan_extensions_count);

    vkEnumerateDeviceExtensionProperties(*vulcano_state->phys_dev, NULL, &vulcano_state->vulkan_extensions_count, vulcano_state->vulkan_extensions);

    printf(BOLD GREEN "[vulkan] vk_list_device_ext: Listing %d available device extensions..." NORMAL "\n", vulcano_state->vulkan_extensions_count);

    for (size_t i = 0; i < vulcano_state->vulkan_extensions_count; i++)
    {
        printf(BOLD GREEN "[vulkan] #%lu > %s" NORMAL "\n", i, vulcano_state->vulkan_extensions[i].extensionName);
    }

    if (vulcano_state->vulkan_extensions)
        free(vulcano_state->vulkan_extensions);
}
