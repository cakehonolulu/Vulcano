#pragma once

#include <vulcano.h>

/* Function Definitions */
void vk_buffer_create(vulcano_struct *vulcano_state);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, vulcano_struct *vulcano_state);

