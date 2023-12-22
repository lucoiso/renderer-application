// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/VulkanRenderer

module;

// Vulkan
#include <volk.h>

// Imgui
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

    if (m_ViewportDescriptorSet != VK_NULL_HANDLE)
    {
        ImGui_ImplVulkan_RemoveTexture(m_ViewportDescriptorSet);
        m_ViewportDescriptorSet = VK_NULL_HANDLE;
    }

    VkSampler const Sampler     = m_Window->GetRenderer().GetSampler();
    VkImageView const ImageView = m_Window->GetRenderer().GetViewportRenderImageView();

    if (Sampler != VK_NULL_HANDLE && ImageView != VK_NULL_HANDLE)
    {
        m_ViewportDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
}

void AppViewport::PrePaint()
{
    VkClearValue const& g_ClearColor {RenderCore::g_ClearValues.at(0U)};
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4 {g_ClearColor.color.float32[0], g_ClearColor.color.float32[1], g_ClearColor.color.float32[2], g_ClearColor.color.float32[3]});
    ImGui::Begin("Viewport");
    ImGui::PopStyleColor();
}

void AppViewport::Paint()
{
    if (m_ViewportDescriptorSet != VK_NULL_HANDLE)
    {
        ImVec2 const ViewportSize {ImGui::GetContentRegionAvail()};
        ImGui::Image(m_ViewportDescriptorSet, ImVec2 {ViewportSize.x, ViewportSize.y});
    }
}

void AppViewport::PostPaint()
{
    ImGui::End();
}