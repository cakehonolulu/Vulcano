#include <vk_physical.h>

void vk_pick_physical_device(vulcano_struct *vulcano_state, bool *vulkan_error)
{
    uint32_t physical_device_cnt = 0;

    // Enumerate physical devices
    vkEnumeratePhysicalDevices(vulcano_state->instance, &physical_device_cnt, NULL);

    if (physical_device_cnt == 0)
    {
        printf(RED "[vulkan] vk_pick_physical_device: Vulkan-compatible compute devices not found, exiting..." NORMAL "\n");
        *vulkan_error = true;
        goto vk_pick_physical_device_end;
    }
    else
    {
        printf(YELLOW "[vulkan] vk_pick_physical_device: Found %d compute devices..." NORMAL "\n", physical_device_cnt);
    }

    vulcano_state->physical_devices = malloc (sizeof(VkPhysicalDevice) * physical_device_cnt);

    vkEnumeratePhysicalDevices(vulcano_state->instance, &physical_device_cnt, vulcano_state->physical_devices);

    VkPhysicalDeviceProperties *physical_devices_props = malloc(sizeof(VkPhysicalDeviceProperties) * physical_device_cnt);

    VkPhysicalDeviceMemoryProperties *physical_device_memory_props = malloc(sizeof(VkPhysicalDeviceMemoryProperties) * physical_device_cnt);

    for (size_t i = 0; i < physical_device_cnt; i++)
    {
        vkGetPhysicalDeviceProperties(vulcano_state->physical_devices[i], &physical_devices_props[i]);
		vkGetPhysicalDeviceMemoryProperties(vulcano_state->physical_devices[i], &physical_device_memory_props[i]);

        if (physical_devices_props[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
			printf(GREEN "[vulkan] vk_pick_physical_device: Found dGPU!" NORMAL "\n");
            printf(GREEN "         ┌─ Name" NORMAL "\n");
            printf(GREEN "         │  · %s" NORMAL "\n", physical_devices_props[i].deviceName);
            printf(GREEN "         └─ Physical ID" NORMAL "\n");
            printf(GREEN "            · %ld" NORMAL "\n", i);
    
		}
        else if (physical_devices_props[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
        {
            printf(GREEN "[vulkan] vk_pick_physical_device: Found iGPU!" NORMAL "\n");
            printf(GREEN "         ┌─ Name" NORMAL "\n");
            printf(GREEN "         │  · %s" NORMAL "\n", physical_devices_props[i].deviceName);
            printf(GREEN "         └─ Physical ID" NORMAL "\n");
            printf(GREEN "            · %ld" NORMAL "\n", i);
        }
        else
        {
            printf(YELLOW "[vulkan] vk_pick_physical_device: Unsupported compute device %d" NORMAL "\n", physical_devices_props[i].deviceType);
        }
    }

vk_pick_physical_device_end:
    return;
}
