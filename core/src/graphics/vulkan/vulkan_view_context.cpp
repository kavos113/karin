#include "vulkan_view_context.h"

#include <stdexcept>

namespace karin
{
VulkanViewContext::VulkanViewContext(float width, float height)
{
    VkDescriptorSetLayoutBinding projMatrixLayoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr,
    };
    VkDescriptorSetLayoutCreateInfo layoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &projMatrixLayoutBinding,
    };
    if (vkCreateDescriptorSetLayout(
        VulkanContext::instance().device(), &layoutInfo, nullptr, &m_projMatrixDescriptorSetLayout
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create projection matrix descriptor set layout");
    }

    m_projMatrixData.proj = glm::mat4(1.0f);
    m_projMatrixData.proj[0][0] = 2.0f / width;
    m_projMatrixData.proj[1][1] = 2.0f / height;
    m_projMatrixData.proj[3][0] = -1.0f;
    m_projMatrixData.proj[3][1] = -1.0f;

    m_projMatrixDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    m_projMatrixBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    std::vector layouts(MAX_FRAMES_IN_FLIGHT, m_projMatrixDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfoDesc = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = VulkanContext::instance().descriptorPool(),
        .descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
        .pSetLayouts = layouts.data(),
    };
    if (vkAllocateDescriptorSets(
        VulkanContext::instance().device(), &allocInfoDesc, m_projMatrixDescriptorSets.data()
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate projection matrix descriptor sets");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDeviceSize bufferSize = sizeof(MatrixBufferObject);
        VmaAllocationCreateInfo allocInfo = {
            .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO,
        };

        VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = bufferSize,
            .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        if (m_projMatrixBuffers[i].create(bufferInfo, allocInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create projection matrix buffer");
        }

        memcpy(
            m_projMatrixBuffers[i].mappedData,
            &m_projMatrixData,
            sizeof(MatrixBufferObject)
        );

        VkDescriptorBufferInfo bufferInfoDesc = {
            .buffer = m_projMatrixBuffers[i].buffer,
            .offset = 0,
            .range = sizeof(MatrixBufferObject),
        };
        VkWriteDescriptorSet descriptorWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = m_projMatrixDescriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &bufferInfoDesc,
        };
        vkUpdateDescriptorSets(VulkanContext::instance().device(), 1, &descriptorWrite, 0, nullptr);
    }
}

void VulkanViewContext::cleanup()
{
    for (size_t i = 0; i < m_projMatrixBuffers.size(); ++i)
    {
        m_projMatrixBuffers[i].cleanup();
    }

    vkDestroyDescriptorSetLayout(VulkanContext::instance().device(), m_projMatrixDescriptorSetLayout, nullptr);
}

void VulkanViewContext::resize(float width, float height)
{
    m_projMatrixData.proj[0][0] = 2.0f / width;
    m_projMatrixData.proj[1][1] = 2.0f / height;
    m_projMatrixData.proj[3][0] = -1.0f;
    m_projMatrixData.proj[3][1] = -1.0f;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        memcpy(
            m_projMatrixBuffers[i].mappedData,
            &m_projMatrixData,
            sizeof(MatrixBufferObject)
        );
    }
}

VkDescriptorSetLayout VulkanViewContext::descriptorSetLayout() const
{
    return m_projMatrixDescriptorSetLayout;
}

VkDescriptorSet VulkanViewContext::descriptorSet(uint32_t currentFrame) const
{
    return m_projMatrixDescriptorSets[currentFrame];
}
} // karin