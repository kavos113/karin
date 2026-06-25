#ifndef CORE_SRC_GRAPHICS_VULKAN_VULKAN_VIEW_CONTEXT_H
#define CORE_SRC_GRAPHICS_VULKAN_VULKAN_VIEW_CONTEXT_H

#include <vector>

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

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

    VkDescriptorSetLayout descriptorSetLayout() const;
    VkDescriptorSet descriptorSet(uint32_t currentFrame) const;

private:
    struct MatrixBufferObject
    {
        glm::mat4 proj;
    };

    MatrixBufferObject m_projMatrixData = {};
    VkDescriptorSetLayout m_projMatrixDescriptorSetLayout = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> m_projMatrixDescriptorSets;
    std::vector<VulkanBuffer<MatrixBufferObject>> m_projMatrixBuffers;

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
};
} // karin

#endif //CORE_SRC_GRAPHICS_VULKAN_VULKAN_VIEW_CONTEXT_H
