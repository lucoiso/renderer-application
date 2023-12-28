// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/VulkanRenderer

module;

#include <vector>

// Vulkan
#include <volk.h>

// Imgui
#include <filesystem>
#include <imgui.h>
#include <imgui_impl_vulkan.h>

module Application.Viewport;

using namespace Application;

import RenderCore.Renderer;
import RenderCore.Types.Camera;
import RenderCore.Types.Object;
import RenderCore.Utils.Constants;

AppViewport::AppViewport(Control* const Parent, AppWindow* const Window)
    : Control(Parent), m_Window(Window)
{
}

void AppViewport::Refresh()
{
    if (!RenderCore::Renderer::IsImGuiInitialized())
    {
        return;
    }

    if (!std::empty(m_ViewportDescriptorSets))
    {
        for (auto const& DescriptorSetIter: m_ViewportDescriptorSets)
        {
            if (DescriptorSetIter != VK_NULL_HANDLE)
            {
                ImGui_ImplVulkan_RemoveTexture(DescriptorSetIter);
            }
        }
        m_ViewportDescriptorSets.clear();
    }

    VkSampler const Sampler{m_Window->GetRenderer().GetSampler()};
    std::vector const ImageViews{m_Window->GetRenderer().GetViewportRenderImageViews()};

    if (Sampler != VK_NULL_HANDLE && !std::empty(ImageViews))
    {
        for (auto const& ImageViewIter: ImageViews)
        {
            m_ViewportDescriptorSets.push_back(ImGui_ImplVulkan_AddTexture(Sampler, ImageViewIter, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        }
    }
}

void AppViewport::PrePaint()
{
    VkClearValue const& ClearColor{RenderCore::g_ClearValues.at(0U)};
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ClearColor.color.float32[0], ClearColor.color.float32[1], ClearColor.color.float32[2], ClearColor.color.float32[3]});
    ImGui::Begin("Viewport");
    ImGui::PopStyleColor();
}

void AppViewport::Paint()
{
    if (!std::empty(m_ViewportDescriptorSets))
    {
        if (std::optional<std::int32_t> const& ImageIndex = m_Window->GetRenderer().GetImageIndex(); ImageIndex.has_value())
        {
            constexpr auto ViewportFramePath{"Frame.png"};
            if (ImGui::Button("Save Frame Image"))
            {
                m_Window->GetRenderer().SaveFrameToImageFile(ViewportFramePath);
            }

            if (std::filesystem::exists(ViewportFramePath))
            {
                ImGui::SameLine();
                if (ImGui::Button("Open Frame Image"))
                {
                    std::filesystem::path const FramePath{ViewportFramePath};
                    std::string const Command{"start " + FramePath.string()};
                    std::system(std::data(Command));
                }
            }

            ImVec2 const ViewportSize{ImGui::GetContentRegionAvail()};
            ImGui::Image(m_ViewportDescriptorSets.at(ImageIndex.value()), ImVec2{ViewportSize.x, ViewportSize.y});
        }
    }
}

void AppViewport::PostPaint()
{
    ImGui::End();
}
