// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <filesystem>
#include <vector>

// Imgui
#include <imgui.h>
#include <Volk/volk.h>

module UserInterface.Viewport;

import RenderCore.Renderer;
import RenderCore.Types.Camera;
import RenderCore.Types.Object;
import RenderCore.Integrations.ImGuiVulkanBackend;

using namespace UserInterface;

AppViewport::AppViewport(Control *const Parent)
    : Control(Parent)
{
}

AppViewport::~AppViewport()
{
    if (!std::empty(m_ViewportDescriptorSets))
    {
        for (auto const &DescriptorSetIter : m_ViewportDescriptorSets)
        {
            if (DescriptorSetIter != VK_NULL_HANDLE)
            {
                RenderCore::ImGuiVulkanRemoveTexture(DescriptorSetIter);
            }
        }
        m_ViewportDescriptorSets.clear();
    }
}

void AppViewport::Refresh()
{
    if (!RenderCore::Renderer::IsImGuiInitialized())
    {
        return;
    }

    if (!std::empty(m_ViewportDescriptorSets))
    {
        for (auto const &DescriptorSetIter : m_ViewportDescriptorSets)
        {
            if (DescriptorSetIter != VK_NULL_HANDLE)
            {
                RenderCore::ImGuiVulkanRemoveTexture(DescriptorSetIter);
            }
        }
        m_ViewportDescriptorSets.clear();
    }

    VkSampler const Sampler { RenderCore::Renderer::GetSampler() };

    if (std::vector const ImageViews { RenderCore::Renderer::GetOffscreenImages() };
        Sampler != VK_NULL_HANDLE && !std::empty(ImageViews))
    {
        for (auto const &ImageViewIter : ImageViews)
        {
            m_ViewportDescriptorSets.push_back(RenderCore::ImGuiVulkanAddTexture(Sampler, ImageViewIter, VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL));
        }
    }
}

void AppViewport::PrePaint()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4 { 0.F, 0.F, 0.F, 1.F });

    m_Open = ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove) && ImGui::IsItemVisible();
    ImGui::PopStyleColor();
}

void AppViewport::Paint()
{
    if (m_Open && !std::empty(m_ViewportDescriptorSets))
    {
        if (std::optional<std::int32_t> const &ImageIndex = RenderCore::Renderer::GetImageIndex();
            ImageIndex.has_value())
        {
            ImVec2 const ViewportSize { ImGui::GetContentRegionAvail() };
            ImGui::Image(m_ViewportDescriptorSets.at(ImageIndex.value()), ImVec2 { ViewportSize.x, ViewportSize.y });
        }
    }
}

void AppViewport::PostPaint()
{
    ImGui::End();
}
