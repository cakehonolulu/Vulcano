#include <vk_queue.h>

uint32_t vk_queue_get_prop_count_from_device(VkPhysicalDevice *device)
{
    uint32_t count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(*device, &count, NULL);

    return count;
}

void vk_queue_get_props_from_device(vulcano_struct *vulcano_state, uint32_t prop_count)
{
    vulcano_state->queue_family_props = malloc(sizeof(VkQueueFamilyProperties) * prop_count);
	vkGetPhysicalDeviceQueueFamilyProperties(*vulcano_state->phys_dev, &prop_count, vulcano_state->queue_family_props);
}
