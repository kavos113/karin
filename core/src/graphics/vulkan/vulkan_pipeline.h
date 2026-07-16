#ifndef GRAPHICS_VULKAN_VULKAN_PIPELINE_H
#define GRAPHICS_VULKAN_VULKAN_PIPELINE_H

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>

namespace karin
{
class VulkanPipeline
{
public:
    VulkanPipeline(
        VkFormat targetFormat,
        const std::byte* vertShaderCode, unsigned int vertShaderSize,
        const std::byte* fragShaderCode, unsigned int fragShaderSize,
        const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
        const std::vector<VkPushConstantRange>& pushConstantRanges
    );
    ~VulkanPipeline() = default;

    enum class ShapeType : uint32_t
    {
        Nothing = 0,
        Ellipse = 1,
        RoundedRectangle = 2,
    };

    void cleanUp();

    VkPipeline pipeline() const
    {
        return m_graphicsPipeline;
    }

    VkPipelineLayout pipelineLayout() const
    {
        return m_pipelineLayout;
    }

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 uv;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription = {
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array attributeDescriptions = {
                VkVertexInputAttributeDescription{
                    .location = 0,
                    .binding = 0,
                    .format = VK_FORMAT_R32G32_SFLOAT,
                    .offset = offsetof(Vertex, pos)
                },
                VkVertexInputAttributeDescription{
                    .location = 1,
                    .binding = 0,
                    .format = VK_FORMAT_R32G32_SFLOAT,
                    .offset = offsetof(Vertex, uv)
                }
            };
            return attributeDescriptions;
        }
    };

private:
    void createPipeline(
        VkFormat targetFormat,
        const std::byte* vertShaderCode, unsigned int vertShaderSize,
        const std::byte* fragShaderCode, unsigned int fragShaderSize,
        const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
        const std::vector<VkPushConstantRange>& pushConstantRanges
    );

    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;
};
} // karin

#endif //GRAPHICS_VULKAN_VULKAN_PIPELINE_H
