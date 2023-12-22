// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/VulkanRenderer

module;

#include <cstdint>
#include <vector>

// Vulkan
#include <volk.h>

// Imgui
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_internal.h>

module Application.Window;

import RenderCore.Types.Transform;
import Application.Window.ScenePanel;
import Application.Window.StatusPanel;

using namespace Application;

AppWindow::AppWindow()
{
    AddChild<StatusPanel>(this);
    AddChild<ScenePanel>(this);
}

void AppWindow::Refresh()
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

    VkSampler const Sampler     = GetRenderer().GetSampler();
    VkImageView const ImageView = GetRenderer().GetViewportRenderImageView();

    if (Sampler != VK_NULL_HANDLE && ImageView != VK_NULL_HANDLE)
    {
        m_ViewportDescriptorSet = ImGui_ImplVulkan_AddTexture(Sampler, ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
}

static ImGuiID DockspaceID {0U};
static bool IsDockspaceInitialized {false};

void AppWindow::PrePaint()
{
    ImGuiViewport const* const Viewport = ImGui::GetMainViewport();
    DockspaceID                         = ImGui::DockSpaceOverViewport(Viewport, ImGuiDockNodeFlags_PassthruCentralNode);

    if (!IsDockspaceInitialized)
    {
        if (ImGuiDockNode const* const Node = ImGui::DockBuilderGetNode(DockspaceID); Node != nullptr && !Node->IsSplitNode())
        {
            SetDockingLayout();
        }
        IsDockspaceInitialized = true;
    }

    ImGui::Begin("Options");
}

void AppWindow::PostPaint()
{
    ImGui::End();

    ImGui::SetNextWindowBgAlpha(0.F);
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

    if (m_ViewportDescriptorSet != VK_NULL_HANDLE)
    {
        ImVec2 const WindowOffset = ImGui::GetWindowPos();
        ImVec2 const ViewportSize = ImGui::GetContentRegionAvail();

        ImGui::Image(m_ViewportDescriptorSet, ImVec2 {ViewportSize.x, ViewportSize.y});

        GetRenderer().SetViewportRect(VkRect2D {
                .offset = {static_cast<std::int32_t>(WindowOffset.x), static_cast<std::int32_t>(WindowOffset.y)},
                .extent = {static_cast<std::uint32_t>(ViewportSize.x), static_cast<std::uint32_t>(ViewportSize.y)}});
    }

    ImGui::End();
}

void AppWindow::SetDockingLayout()
{
    ImGuiID TempNodeID                     = DockspaceID;
    ImGuiDockNode const* const CentralNode = ImGui::DockBuilderGetCentralNode(TempNodeID);
    ImGui::DockBuilderDockWindow("Viewport", CentralNode->ID);

    ImGuiID const LeftID = ImGui::DockBuilderSplitNode(TempNodeID, ImGuiDir_Left, 0.7F, nullptr, &TempNodeID);
    ImGui::DockBuilderDockWindow("Options", LeftID);

    ImGui::DockBuilderFinish(DockspaceID);
}