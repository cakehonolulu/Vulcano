#include <vulcano.h>
#include <vk_instance.h>
#include <vk_physical.h>
#include <vk_queue.h>
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
            uint32_t phys_dev_prop_count = vk_queue_get_prop_count_from_device(vulcano_state->phys_dev);

            vk_queue_get_props_from_device(vulcano_state, phys_dev_prop_count);

            vulcano_state->device = vk_create_device(vulcano_state, phys_dev_prop_count);

            vk_list_device_ext(vulcano_state);

            retval = 0;
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
