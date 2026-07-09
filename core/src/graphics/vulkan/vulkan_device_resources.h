#ifndef GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
#define GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H

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
    VulkanTextureResourceDescriptor(const VulkanTextureResourceDescriptor&) = delete;
    VulkanTextureResourceDescriptor& operator=(const VulkanTextureResourceDescriptor&) = delete;

    VulkanTextureResourceDescriptor(VulkanTextureResourceDescriptor&& other) noexcept
        : image(std::move(other.image)), descriptorSets(other.descriptorSets)
    {
        other.descriptorSets.clear();
    }

    VulkanTextureResourceDescriptor& operator=(VulkanTextureResourceDescriptor&& other) noexcept
    {
        if (this != &other)
        {
            cleanup();

            image = std::move(other.image);
            descriptorSets = std::move(other.descriptorSets);

            other.descriptorSets.clear();
        }
        return *this;
    }

    VkDescriptorSet descriptorSet(uint32_t currentFrame) const
    {
        return descriptorSets[currentFrame];
    }

private:
    friend class VulkanDeviceResources;

    VulkanTextureResourceDescriptor() = default;
    explicit VulkanTextureResourceDescriptor(
        VulkanImage i,
        const std::vector<VkDescriptorSet>& ds
    ) : image(std::move(i)), descriptorSets(ds)
    {
    }

    void cleanup()
    {
        image.cleanup();
    }

    VulkanImage image;
    std::vector<VkDescriptorSet> descriptorSets; // One per frame in flight
};

class VulkanLayerPool;

class VulkanDeviceResources
{
public:
    explicit VulkanDeviceResources(size_t maxFramesInFlight);
    ~VulkanDeviceResources();

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

    void clearOffscreenImages() const;
    VulkanImage* offscreenImage(uint16_t layerID, Size imageSize, VkFormat imageFormat) const;
    Rectangle offscreenImageUv(uint16_t layerID, Size imageSize) const;

private:
    static constexpr size_t LUT_WIDTH = 256;

    void createSamplers();
    void createDescriptorSetLayouts();
    void createDummyTexture();

    static std::array<uint8_t, LUT_WIDTH * 4> generateGradientPointLut(
        const std::vector<GradientPoints::GradientPoint>& gradientPoints
    );

    std::unique_ptr<VulkanLayerPool> m_offscreenLayerPool;

    std::unordered_map<size_t, VulkanTextureResourceDescriptor> m_gradientPointLutMap;
    std::unordered_map<size_t, VulkanTextureResourceDescriptor> m_textureMap;
    VulkanTextureResourceDescriptor m_dummyTexture; // 1 x 1 white pixel
    std::unordered_map<uint16_t, VulkanImage> m_offscreenImages;

    VkSampler m_clampSampler = VK_NULL_HANDLE;
    VkSampler m_repeatSampler = VK_NULL_HANDLE;
    VkSampler m_mirrorSampler = VK_NULL_HANDLE;
    uint32_t m_maxFramesInFlight = 2;
    VkDescriptorSetLayout m_geometryDescriptorSetLayout = VK_NULL_HANDLE;
};
} // karin

#endif //GRAPHICS_VULKAN_VULKAN_DEVICE_RESOURCES_H
