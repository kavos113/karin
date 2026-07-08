#ifndef GRAPHICS_VULKAN_VULKAN_FONT_RENDERER_H
#define GRAPHICS_VULKAN_VULKAN_FONT_RENDERER_H

#include <karin/graphics/graphics_context.h>
#include "font_renderer_impl.h"
#include "vulkan_glyph_cache.h"

namespace karin
{
class VulkanRendererImpl;

class VulkanFontRenderer : public IFontRendererImpl
{
public:
    explicit VulkanFontRenderer(VulkanRendererImpl *renderer, size_t maxFramesInFlight);
    ~VulkanFontRenderer() override;

    void drawText(const TextBlob& text, Point start, const Pattern& pattern, const GraphicsContext::State& state) const override;

    void cleanup() const;

    VkDescriptorSetLayout atlasDescriptorSetLayout() const
    {
        return m_glyphCache->atlasDescriptorSetLayout();
    }

    void flushGlyphUploads() const
    {
        m_glyphCache->flushUploadQueue();
    }

    std::vector<VkDescriptorSet> glyphAtlasDescriptorSets() const
    {
        return m_glyphCache->atlasDescriptorSets();
    }

private:
    struct GlyphUploadPosition
    {
        // origin: top-left of the glyph bitmap
        Rectangle position;
        Rectangle atlasUV; // in [0, 1]
    };

    std::unique_ptr<VulkanGlyphCache> m_glyphCache;

    VulkanRendererImpl* m_renderer;
};
} // karin

#endif //GRAPHICS_VULKAN_VULKAN_FONT_RENDERER_H
