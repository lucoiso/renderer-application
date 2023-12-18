// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/VulkanRenderer

module;

#include <imgui.h>
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

void AppWindow::Paint()
{
}

void AppWindow::PostPaint()
{
    ImGui::End();

    ImGuiViewport const* const Viewport = ImGui::GetMainViewport();

    GetRenderer().SetViewportOffset(RenderCore::ViewSize {
            .X = Viewport->Pos.x,
            .Y = Viewport->Pos.y,
            .W = Viewport->Size.x,
            .H = Viewport->Size.y});
}

void AppWindow::SetDockingLayout()
{
    ImGuiID TempNodeID   = DockspaceID;
    ImGuiID const LeftID = ImGui::DockBuilderSplitNode(TempNodeID, ImGuiDir_Left, 0.7F, nullptr, &TempNodeID);
    ImGui::DockBuilderDockWindow("Options", LeftID);

    ImGui::DockBuilderFinish(DockspaceID);
}