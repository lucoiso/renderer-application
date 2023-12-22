// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/VulkanRenderer

module;

#include <imgui.h>
#include <imgui_internal.h>

module Application.Window;

import RenderCore.Types.Transform;

import Application.Viewport;
import Application.ScenePanel;
import Application.StatusPanel;

using namespace Application;

AppWindow::AppWindow()
{
    AddChild<AppStatusPanel>(this);
    AddChild<AppScenePanel>(this);
    AddIndependentChild<AppViewport>(this);
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