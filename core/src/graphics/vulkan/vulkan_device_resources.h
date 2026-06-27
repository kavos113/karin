#ifndef SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
#define SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H

#include <cstddef>
#include <cstdint>

#include <vector>
#include <unordered_map>
#include <array>

#include <vulkan/vulkan.h>
#include "vma.h"

#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/image.h>
#include <karin/graphics/pattern.h>
#include "vulkan_buffer.h"

namespace karin
{
class VulkanTextureResourceDescriptor
{
public:
    VkDescriptorSet descriptorSet(uint32_t currentFrame) const
    {
        return descriptorSets[currentFrame];
    }

private:
    friend class VulkanDeviceResources;

    VulkanTextureResourceDescriptor() = default;
    explicit VulkanTextureResourceDescriptor(
        VkImage i,
        VmaAllocation a,
        VkImageView iv,
        const std::vector<VkDescriptorSet>& ds
    ) : image(i), allocation(a), imageView(iv), descriptorSets(ds)
    {
    }

    VkImage image = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets; // One per frame in flight
};

class VulkanDeviceResources
{
public:
    explicit VulkanDeviceResources(
        size_t maxFramesInFlight
    )
        : m_maxFramesInFlight(maxFramesInFlight)
    {
        createSamplers();
        createDescriptorSetLayouts();
        createDummyTexture();
    }

    ~VulkanDeviceResources() = default;

    Image createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height);

    void cleanup();

    const VulkanTextureResourceDescriptor *gradientPointLut(const GradientPoints& points);
    const VulkanTextureResourceDescriptor *texture(Image image);
    const VulkanTextureResourceDescriptor *dummyTexture() const;
    VkDescriptorSet offscreenImageDescriptorSet(VkImageView imageView);

    VkDescriptorSetLayout geometryDescriptorSetLayout() const
    {
        return m_geometryDescriptorSetLayout;
    }

    VulkanImage newOffscreenImage(const Rectangle& rect, VkFormat imageFormat);
    void clearOffscreenImages();

private:
    static constexpr size_t LUT_WIDTH = 256;

    void createSamplers();
    void createDescriptorSetLayouts();
    void createDummyTexture();
    std::array<uint8_t, LUT_WIDTH * 4> generateGradientPointLut(
        const std::vector<GradientPoints::GradientPoint>& gradientPoints
    ) const;

    std::unordered_map<size_t, VulkanTextureResourceDescriptor> m_gradientPointLutMap;
    std::unordered_map<size_t, VulkanTextureResourceDescriptor> m_textureMap;
    VulkanTextureResourceDescriptor m_dummyTexture; // 1 x 1 white pixel
    std::vector<VulkanImage> m_offscreenImages;

    VkSampler m_clampSampler = VK_NULL_HANDLE;
    VkSampler m_repeatSampler = VK_NULL_HANDLE;
    VkSampler m_mirrorSampler = VK_NULL_HANDLE;
    uint32_t m_maxFramesInFlight = 2;
    VkDescriptorSetLayout m_geometryDescriptorSetLayout = VK_NULL_HANDLE;
};
} // karin

#endif //SRC_GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H