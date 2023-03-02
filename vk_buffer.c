#include <vk_buffer.h>

Vertex vertices[] = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

void vk_buffer_create(vulcano_struct *vulcano_state)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * sizeof(vertices);
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vulcano_state->vertex_ct = bufferInfo.size;

    if (vkCreateBuffer(vulcano_state->device, &bufferInfo, NULL, &vulcano_state->vertexBuffer) != VK_SUCCESS) {
        printf("[vulcano] failed to create vertex buffer!");
        vulkan_exit(vulcano_state);
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vulcano_state->device, vulcano_state->vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vulcano_state);


    if (vkAllocateMemory(vulcano_state->device, &allocInfo, NULL, &vulcano_state->vertexBufferMemory) != VK_SUCCESS) {
        printf("[vulcano] failed to allocate memory for the vertex buffer!");
        vulkan_exit(vulcano_state);
    }

    if (vkBindBufferMemory(vulcano_state->device, vulcano_state->vertexBuffer, vulcano_state->vertexBufferMemory, 0) != VK_SUCCESS)
    {
        printf("[vulcano] failed to bind vertex buffer memory!");
        vulkan_exit(vulcano_state);
    }

    void* data;
    vkMapMemory(vulcano_state->device, vulcano_state->vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices, (size_t) sizeof(vertices));
    vkUnmapMemory(vulcano_state->device, vulcano_state->vertexBufferMemory);


}

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, vulcano_struct *vulcano_state)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(*vulcano_state->phys_dev, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    printf("[vulcano] failed to find memory!");
    vulkan_exit(vulcano_state);
}

