#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_VIEW_CONTEXT_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_VIEW_CONTEXT_H

#include <array>
#include <vector>

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <karin/common/geometry/rectangle.h>
#include "vulkan_buffer.h"

namespace karin
{
class VulkanViewContext
{
public:
    VulkanViewContext(float width, float height);
    ~VulkanViewContext() = default;

    void cleanup();
    void resize(float width, float height);

    void setProjMatrix(Rectangle rect, uint16_t layerID);
    void bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint16_t layerID, uint32_t currentFrame) const;

    VkDescriptorSetLayout descriptorSetLayout() const;

private:
    struct MatrixBufferObject
    {
        glm::mat4 proj;
    };

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
    static constexpr uint32_t MAX_PROJECTION_MATRIX_COUNT = 16;
    VkDeviceSize minUniformBufferOffsetAlignment;

    std::array<MatrixBufferObject, MAX_PROJECTION_MATRIX_COUNT> m_projMatrixData;
    VkDescriptorSetLayout m_projMatrixDescriptorSetLayout = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> m_projMatrixDescriptorSets;
    std::vector<VulkanBuffer<MatrixBufferObject>> m_projMatrixBuffers;
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_VIEW_CONTEXT_H
