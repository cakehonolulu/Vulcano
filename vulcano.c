#include <vulcano.h>
#include <vk_instance.h>
#include <vk_physical.h>

int vulkan_init(vulcano_struct *vulcano_state)
{
    int retval = 1;

    bool vulkan_error = false;

    vulcano_state->instance = vk_create_instance(vulcano_state, &vulkan_error);
    
    if (!vulkan_error)
    {
        int phys_dev_idx = vk_pick_physical_device(vulcano_state, &vulkan_error);

        VkPhysicalDevice *phys_dev = &vulcano_state->physical_devices[phys_dev_idx];

        if (!vulkan_error && phys_dev_idx != -1)
        {
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

    if (vulcano_state->physical_devices)
        free(vulcano_state->physical_devices);

    if (vulcano_state->vulkan_instance_extensions)
        free(vulcano_state->vulkan_instance_extensions);

    retval = 0;

    return retval;
}
