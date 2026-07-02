#include "vulkan_view_context.h"

#include <stdexcept>

#define AlignmentSize(size, alignment) ((size + alignment - 1) & ~(alignment - 1))

namespace karin
{
VulkanViewContext::VulkanViewContext(float width, float height)
{
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(VulkanContext::instance().physicalDevice(), &props);
    minUniformBufferOffsetAlignment = props.limits.minUniformBufferOffsetAlignment;

    VkDescriptorSetLayoutBinding projMatrixLayoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
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

    for (size_t i = 0; i < MAX_PROJECTION_MATRIX_COUNT; i++)
    {
        m_projMatrixData[i].proj = glm::mat4(1.0f);
    }
    m_projMatrixData[0].proj[0][0] = 2.0f / width;
    m_projMatrixData[0].proj[1][1] = 2.0f / height;
    m_projMatrixData[0].proj[3][0] = -1.0f;
    m_projMatrixData[0].proj[3][1] = -1.0f;

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
        VkDeviceSize bufferSize = AlignmentSize(sizeof(MatrixBufferObject), minUniformBufferOffsetAlignment) * MAX_PROJECTION_MATRIX_COUNT;
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
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
            .pBufferInfo = &bufferInfoDesc,
        };
        vkUpdateDescriptorSets(VulkanContext::instance().device(), 1, &descriptorWrite, 0, nullptr);
    }
}

void VulkanViewContext::cleanup()
{
    for (auto & m_projMatrixBuffer : m_projMatrixBuffers)
    {
        m_projMatrixBuffer.cleanup();
    }

    vkDestroyDescriptorSetLayout(VulkanContext::instance().device(), m_projMatrixDescriptorSetLayout, nullptr);
}

void VulkanViewContext::resize(float width, float height)
{
    m_projMatrixData[0].proj[0][0] = 2.0f / width;
    m_projMatrixData[0].proj[1][1] = 2.0f / height;
    m_projMatrixData[0].proj[3][0] = -1.0f;
    m_projMatrixData[0].proj[3][1] = -1.0f;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        memcpy(
            m_projMatrixBuffers[i].mappedData,
            &m_projMatrixData[0],
            sizeof(MatrixBufferObject)
        );
    }
}

void VulkanViewContext::setProjMatrix(Rectangle rect, uint16_t layerID)
{
    m_projMatrixData[layerID].proj[0][0] = 2.0f / rect.size.width;
    m_projMatrixData[layerID].proj[1][1] = 2.0f / rect.size.height;
    m_projMatrixData[layerID].proj[3][0] = -1.0f - (rect.pos.x * 2.0f /  rect.size.width);
    m_projMatrixData[layerID].proj[3][1] = -1.0f - (rect.pos.y * 2.0f /  rect.size.height);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        auto *dst = reinterpret_cast<char*>(m_projMatrixBuffers[i].mappedData) + AlignmentSize(sizeof(MatrixBufferObject), minUniformBufferOffsetAlignment) * layerID;
        memcpy(
            dst,
            &m_projMatrixData[layerID],
            sizeof(MatrixBufferObject)
        );
    }
}

void VulkanViewContext::bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint16_t layerID, uint32_t currentFrame) const
{
    uint32_t offset = AlignmentSize(sizeof(MatrixBufferObject), minUniformBufferOffsetAlignment) * layerID;
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout,
        0, 1, &m_projMatrixDescriptorSets[currentFrame],
        1, &offset
    );
}

VkDescriptorSetLayout VulkanViewContext::descriptorSetLayout() const
{
    return m_projMatrixDescriptorSetLayout;
}
} // karin