#include <vulcano.h>
#include <vk_instance.h>

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

    if (vulcano_state->vulkan_instance_extensions)
        free(vulcano_state->vulkan_instance_extensions);

    retval = 0;

    return retval;
}
