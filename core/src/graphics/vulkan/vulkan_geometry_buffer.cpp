#include "vulkan_geometry_buffer.h"

#include <stdexcept>

namespace karin
{
VulkanGeometryBuffer::VulkanGeometryBuffer()
{
    createVertexBuffer();
    createIndexBuffer();
}

void VulkanGeometryBuffer::cleanup()
{
    m_vertexBuffer.cleanup();
    m_indexBuffer.cleanup();
}

uint32_t VulkanGeometryBuffer::append(
    const std::vector<VulkanPipeline::Vertex>& vertices,
    std::vector<uint16_t>& indices
)
{
    uint32_t indexOffset = m_indexCount;

    memcpy(m_vertexBuffer.mappedData, vertices.data(), vertices.size() * sizeof(VulkanPipeline::Vertex));
    m_vertexBuffer.mappedData += vertices.size();

    for (uint16_t& index : indices)
    {
        index += m_vertexOffset;
    }

    memcpy(m_indexBuffer.mappedData, indices.data(), indices.size() * sizeof(uint16_t));
    m_indexBuffer.mappedData += indices.size();
    m_indexCount += indices.size();

    // TODO: オフセットのオーバーフロー
    m_vertexOffset += static_cast<uint16_t>(vertices.size());

    return indexOffset;
}

void VulkanGeometryBuffer::reset()
{
    m_vertexBuffer.mappedData = m_vertexStartPoint;
    m_indexBuffer.mappedData = m_indexStartPoint;
    m_vertexOffset = 0;
    m_indexCount = 0;
}

void VulkanGeometryBuffer::bind(VkCommandBuffer commandBuffer) const
{
    std::array vertexBuffers = {m_vertexBuffer.buffer};
    std::array<VkDeviceSize, 1> offsets = {};
    vkCmdBindVertexBuffers(
        commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data()
    );
    vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
}

void VulkanGeometryBuffer::createVertexBuffer()
{
    VmaAllocationCreateInfo allocInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = vertexBufferSize,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (m_vertexBuffer.create(bufferInfo, allocInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer");
    }
    m_vertexStartPoint = m_vertexBuffer.mappedData;
}

void VulkanGeometryBuffer::createIndexBuffer()
{
    VmaAllocationCreateInfo allocInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = indexBufferSize,
        .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (m_indexBuffer.create(bufferInfo, allocInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create index buffer");
    }
    m_indexStartPoint = m_indexBuffer.mappedData;
}
} // karin