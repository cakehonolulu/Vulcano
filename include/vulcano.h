#include <vulkan/vulkan.h>
#include <stdint.h>

typedef struct {
    VkInstance instance;
} vulcano_struct;

/* Function Definitions */
int vulkan_init();
int vulkan_exit();
