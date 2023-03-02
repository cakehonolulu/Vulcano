#include <vulcano.h>
#include <vk_instance.h>
#include <vk_image.h>
#include <vk_command.h>
#include <vk_framebuffer.h>
#include <vk_physical.h>
#include <vk_graphics_queue.h>
#include <vk_buffer.h>
#include <vk_queue.h>
#include <vk_surface.h>
#include <vk_swapchain.h>
#include <vk_shader.h>
#include <vk_pipeline.h>
#include <vk_sync.h>
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
            uint32_t queue_family_number = vk_queue_get_prop_count_from_device(vulcano_state->phys_dev);

            vk_queue_get_props_from_device(vulcano_state, queue_family_number);

            vulcano_state->device = vk_create_device(vulcano_state, queue_family_number);

            vk_list_device_ext(vulcano_state);

            vulcano_state->vk_queue_family_idx = vk_graphics_queue_get_family_idx(vulcano_state, queue_family_number);

            vulcano_state->vk_queue_mode = vk_graphics_queue_get_mode(vulcano_state, vulcano_state->vk_queue_family_idx);

            vulcano_state->vk_image_array_layers = 1;

            vulcano_state->draw_queue = vk_graphics_queue_get_draw(vulcano_state, vulcano_state->vk_queue_family_idx);

	        vulcano_state->present_queue = vk_graphics_queue_get_presenting(vulcano_state, vulcano_state->vk_queue_family_idx, vulcano_state->vk_queue_mode);

            SDL_Vulkan_CreateSurface(vulcano_state->vulcano_window,
                (SDL_vulkanInstance) vulcano_state->instance, (SDL_vulkanSurface *) &vulcano_state->surface);

            VkBool32 vk_surface_support = 0;
	        
            vkGetPhysicalDeviceSurfaceSupportKHR(*vulcano_state->phys_dev, vulcano_state->vk_queue_family_idx, vulcano_state->surface, &vk_surface_support);

            if (vk_surface_support)
            {
                vk_surface_prepare(vulcano_state);

                vk_swapchain_create(vulcano_state);

                vk_image_prepare(vulcano_state);

                vk_framebuffer_prepare(vulcano_state);

                vk_framebuffer_create(vulcano_state);

                uint32_t vertexShaderSize = 0;
	            char vertexShaderFileName[] = "vert_shader.spv";
	            vulcano_state->vertexShaderCode = getShaderCode(vertexShaderFileName, &vertexShaderSize);

                if (vulcano_state->vertexShaderCode != NULL)
                {
                    vulcano_state->vertexShaderModule = createShaderModule(vulcano_state, vulcano_state->vertexShaderCode, vertexShaderSize);
                    
                    printf(MAGENTA BOLD "[vulkan] init: Vertex Shader Compiled Successfully!" NORMAL "\n");

                    uint32_t fragmentShaderSize = 0;
                    char fragmentShaderFileName[] = "frag_shader.spv";
                    vulcano_state->fragmentShaderCode = getShaderCode(fragmentShaderFileName, &fragmentShaderSize);

                    if (vulcano_state->fragmentShaderCode != NULL)
                    {
                        vulcano_state->fragmentShaderModule = createShaderModule(vulcano_state, vulcano_state->fragmentShaderCode, fragmentShaderSize);
                        
                        printf(MAGENTA BOLD "[vulkan] init: Fragment Shader Compiled Successfully!" NORMAL "\n");

                        vk_create_pipeline(vulcano_state);

                        vk_buffer_create(vulcano_state);

                        vk_command_pool_init(vulcano_state);

                        vk_sync_setup(vulcano_state);

                        retval = 0;
                    }
                    else
                    {
                        printf(RED "[vulkan] init: Couldn't compile the fragment shaders!" NORMAL "\n");
                    }
                }
                else
                {
                    printf(RED "[vulkan] init: Couldn't compile the vertex shaders!" NORMAL "\n");
                }
            }
            else
            {
                printf(RED "[vulkan] init: Platform doesn't have VkSurfaceKHR support, exiting..." NORMAL "\n");
            }
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

    if (vulcano_state->vk_back_fences)
        free(vulcano_state->vk_back_fences);

    if (vulcano_state->vk_front_fences)
    {
        for (size_t i = 0; i < vulcano_state->vk_max_frames; i++)
        {
            vkDestroyFence(vulcano_state->device, vulcano_state->vk_front_fences[i], NULL);
	    }

        free(vulcano_state->vk_front_fences);
    }


    if (vulcano_state->vk_wait_semaphore)
    {
        for (size_t i = 0; i < vulcano_state->vk_max_frames; i++)
        {
            vkDestroySemaphore(vulcano_state->device, vulcano_state->vk_wait_semaphore[i], NULL);
	    }

        free(vulcano_state->vk_wait_semaphore);
    }

    if (vulcano_state->vk_signal_semaphore)
    {
        for (size_t i = 0; i < vulcano_state->vk_max_frames; i++)
        {
            vkDestroySemaphore(vulcano_state->device, vulcano_state->vk_signal_semaphore[i], NULL);
        }

        free(vulcano_state->vk_signal_semaphore);
    }

    if (vulcano_state->vk_command_buf)
    {
        vkFreeCommandBuffers(vulcano_state->device, vulcano_state->vk_command_pool, vulcano_state->vk_swapchain_img_num, vulcano_state->vk_command_buf);
	    free(vulcano_state->vk_command_buf);
    }

    if (vulcano_state->indexBufferMemory)
        vkFreeMemory(vulcano_state->device, vulcano_state->indexBufferMemory, NULL);

    if (vulcano_state->vertexBufferMemory)
        vkFreeMemory(vulcano_state->device, vulcano_state->vertexBufferMemory, NULL);

    if (vulcano_state->vattribdesc)
        free(vulcano_state->vattribdesc);

    if (vulcano_state->vbindingdesc)
        free(vulcano_state->vbindingdesc);

    if (vulcano_state->vk_command_pool)
        vkDestroyCommandPool(vulcano_state->device, vulcano_state->vk_command_pool, NULL);

    if (vulcano_state->indexBuffer)
        vkDestroyBuffer(vulcano_state->device, vulcano_state->indexBuffer, NULL);

    if (vulcano_state->vertexBuffer)
        vkDestroyBuffer(vulcano_state->device, vulcano_state->vertexBuffer, NULL);

    if (vulcano_state->vk_pipeline)
        vkDestroyPipeline(vulcano_state->device, vulcano_state->vk_pipeline, NULL);

    if (vulcano_state->vk_pipeline_layout)
        vkDestroyPipelineLayout(vulcano_state->device, vulcano_state->vk_pipeline_layout, NULL);;

    if (vulcano_state->vertexShaderCode)
        free(vulcano_state->vertexShaderCode);

    if (vulcano_state->fragmentShaderCode)
        free(vulcano_state->fragmentShaderCode);

    if (vulcano_state->fragmentShaderModule)
        vkDestroyShaderModule(vulcano_state->device, vulcano_state->fragmentShaderModule, NULL);

    if (vulcano_state->vertexShaderModule)
        vkDestroyShaderModule(vulcano_state->device, vulcano_state->vertexShaderModule, NULL);

    if (vulcano_state->vk_framebuffer)
    {
        for (size_t i = 0; i < vulcano_state->vk_swapchain_img_num; i++)
        {
            vkDestroyFramebuffer(vulcano_state->device, vulcano_state->vk_framebuffer[i], NULL);
        }

        free(vulcano_state->vk_framebuffer);
    }

    if (vulcano_state->vk_render_pass)
        vkDestroyRenderPass(vulcano_state->device, vulcano_state->vk_render_pass, NULL);

    if (vulcano_state->vk_image_view)
    {
        for (size_t i = 0; i < vulcano_state->vk_swapchain_img_num; i++)
        {
            vkDestroyImageView(vulcano_state->device, vulcano_state->vk_image_view[i], NULL);
        }

        free(vulcano_state->vk_image_view);
    }

    if (vulcano_state->vk_swapchain_imgs)
        free(vulcano_state->vk_swapchain_imgs);

    if (vulcano_state->vk_swapchain)
        vkDestroySwapchainKHR(vulcano_state->device, vulcano_state->vk_swapchain, NULL);

    if (vulcano_state->surface)
        vkDestroySurfaceKHR(vulcano_state->instance, vulcano_state->surface, NULL);

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

    if (vulcano_state->vulkan_layer_extensions)
        free(vulcano_state->vulkan_layer_extensions);

    retval = 0;

    return retval;
}

void render(vulcano_struct *vulcano_state)
{
    bool run = true;
	uint32_t current_frame = 0;

    // SDL Event
    SDL_Event vulcano_event;

    while (run)
    {
        vkWaitForFences(vulcano_state->device, 1, &vulcano_state->vk_front_fences[current_frame], VK_TRUE, UINT64_MAX);
		uint32_t image_index = 0;

		vkAcquireNextImageKHR(vulcano_state->device, vulcano_state->vk_swapchain, UINT64_MAX, vulcano_state->vk_wait_semaphore[current_frame], NULL, &image_index);

		//if(vulcano_state->vk_back_fences[image_index] != NULL)
        //{
            // FIXME
			//vkWaitForFences(vulcano_state->device, 1, &vulcano_state->vk_back_fences[image_index], VK_TRUE, UINT64_MAX);
		//}
		//vulcano_state->vk_back_fences[image_index] = vulcano_state->vk_front_fences[current_frame];

		VkPipelineStageFlags pipeline_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo submitInfo = {
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			NULL,
			1,
			&vulcano_state->vk_wait_semaphore[current_frame],
			&pipeline_flags,
			1,
			&vulcano_state->vk_command_buf[image_index],
			1,
			&vulcano_state->vk_signal_semaphore[current_frame]
		};

		vkResetFences(vulcano_state->device, 1, &vulcano_state->vk_front_fences[current_frame]);
		vkQueueSubmit(vulcano_state->draw_queue, 1, &submitInfo, vulcano_state->vk_front_fences[current_frame]);

		VkPresentInfoKHR present_info = {
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			NULL,
			1,
			&vulcano_state->vk_signal_semaphore[current_frame],
			1,
			&vulcano_state->vk_swapchain,
			&image_index,
			NULL
		};
		vkQueuePresentKHR(vulcano_state->present_queue, &present_info);

		current_frame = (current_frame + 1) % vulcano_state->vk_max_frames;

	    vkDeviceWaitIdle(vulcano_state->device);

        while (SDL_PollEvent(&vulcano_event))
        {
            switch (vulcano_event.type)
            {
                case SDL_WINDOWEVENT:
                {
                    switch (vulcano_event.window.event)
                    {
                        case SDL_WINDOWEVENT_CLOSE:
                            vulkan_exit(vulcano_state);
                            SDL_Quit();
                            free(vulcano_state);
                            run = false;
                            printf("[vulcano] main: User-requested exit\n");
                            break;
                    }
                    break;
                }
            }
        }
    }
}
