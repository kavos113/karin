#ifndef GRAPHICS_VULKAN_VULKAN_GEOMETRY_BUFFER_H
#define GRAPHICS_VULKAN_VULKAN_GEOMETRY_BUFFER_H

#include <cstdint>

#include <vector>

#include <vulkan/vulkan.h>

#include "vulkan_buffer.h"
#include "vulkan_pipeline.h"

namespace karin
{
class VulkanGeometryBuffer
{
public:
    VulkanGeometryBuffer();
    ~VulkanGeometryBuffer() = default;

    // cleanup and free resources
    void cleanup();

    // reset buffer for every beginning of frame
    void reset();

    /**
     * @return index offset for given indices.
     * (ex)
     *   current: m_indices has 100.
     *   append request: 10 indices
     *   -> returns 100. next append request: returns 110.
     */
    uint32_t append(const std::vector<VulkanPipeline::Vertex>& vertices, std::vector<uint16_t>& indices);
    void bind(VkCommandBuffer commandBuffer) const;

private:
    void createVertexBuffer();
    void createIndexBuffer();

    VulkanBuffer<VulkanPipeline::Vertex> m_vertexBuffer;
    VulkanPipeline::Vertex* m_vertexStartPoint = nullptr;
    VulkanBuffer<uint16_t> m_indexBuffer;
    uint16_t* m_indexStartPoint = nullptr;
    uint16_t m_vertexOffset = 0;
    size_t m_indexCount = 0;

    static constexpr VkDeviceSize vertexBufferSize = 1024 * 128; // 2MB
    static constexpr VkDeviceSize indexBufferSize = 1024 * 512; // 2MB
};
} // karin

#endif //GRAPHICS_VULKAN_VULKAN_GEOMETRY_BUFFER_H
