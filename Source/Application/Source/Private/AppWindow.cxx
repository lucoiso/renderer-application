// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <imgui.h>
#include <imgui_internal.h>

module Application.Window;

import RenderCore.Types.Transform;

import Application.Viewport;
import Application.ScenePanel;
import Application.StatusPanel;
import Application.RadeonProfiler;
import RadeonManager.Manager;

using namespace Application;

AppWindow::AppWindow()
{
    AddIndependentChild<AppStatusPanel>(this);
    AddIndependentChild<AppScenePanel>(this);

    if (RadeonManager::IsLoaded())
    {
        AddIndependentChild<AppRadeonProfiler>(this);
    }

    AddIndependentChild<AppViewport>(this);
}

static ImGuiID DockspaceID {0U};
static bool    IsDockspaceInitialized {false};

void AppWindow::PrePaint()
{
    ImGuiViewport const *const Viewport = ImGui::GetMainViewport();
    DockspaceID                         = ImGui::DockSpaceOverViewport(Viewport, ImGuiDockNodeFlags_PassthruCentralNode);

    if (!IsDockspaceInitialized)
    {
        if (ImGuiDockNode const *const Node = ImGui::DockBuilderGetNode(DockspaceID); Node != nullptr && !Node->IsSplitNode())
        {
            SetDockingLayout();
        }
        IsDockspaceInitialized = true;
    }
}

void AppWindow::PostPaint()
{
}

void AppWindow::SetDockingLayout()
{
    ImGuiID                    TempNodeID  = DockspaceID;
    ImGuiDockNode const *const CentralNode = ImGui::DockBuilderGetCentralNode(TempNodeID);
    ImGui::DockBuilderDockWindow("Viewport", CentralNode->ID);

    ImGuiID const LeftID = ImGui::DockBuilderSplitNode(TempNodeID, ImGuiDir_Left, 0.3F, nullptr, &TempNodeID);

    ImGui::DockBuilderDockWindow("Scene", LeftID);
    ImGui::DockBuilderDockWindow("Status", LeftID);
    ImGui::DockBuilderDockWindow("Radeon Profiler", LeftID);

    ImGui::DockBuilderFinish(DockspaceID);
}
