#pragma once

#include <vulcano.h>

/* Function Definitions */
VkDevice vk_create_device(vulcano_struct *vulcano_state, uint32_t prop_count);
void vk_list_device_ext(vulcano_struct *vulcano_state);
