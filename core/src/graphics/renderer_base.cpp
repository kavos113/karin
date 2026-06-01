#include <karin/graphics/renderer_base.h>

#include <cstring>

#include <vector>
#include <stdexcept>

#include <stb_image/stb_image.h>

#include "renderer_impl.h"

namespace karin
{
RendererBase::~RendererBase() = default;

void RendererBase::addDrawCommand(std::function<void(GraphicsContext&)> command)
{
    m_drawCommands.push_back(std::move(command));
}

void RendererBase::setClearColor(const Color& color)
{
    m_impl->setClearColor(color);
}

Image RendererBase::createImage(const std::string& filePath)
{
    int width, height, channels;
    stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
    if (!data)
    {
        throw std::runtime_error("Failed to load image: " + filePath);
    }

    std::vector<std::byte> imageData(width * height * 4);
    memcpy(imageData.data(), data, imageData.size());

    return m_impl->createImage(imageData, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}

Image RendererBase::createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height)
{
    return m_impl->createImage(data, width, height);
}

void RendererBase::cleanUp()
{
    m_impl->cleanUp();
}

}