// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <imgui.h>
#include <imgui_internal.h>

module UserInterface.Window;

import RenderCore.Types.Transform;

import RenderCore.Renderer;
import UserInterface.Viewport;
import UserInterface.ScenePanel;
import UserInterface.StatusPanel;
import UserInterface.RadeonProfiler;
import RadeonManager.Manager;

using namespace UserInterface;

AppWindow::AppWindow()
{
    if (RadeonManager::IsLoaded())
    {
        AddIndependentChild<AppRadeonProfiler>();
    }

    AddIndependentChild<AppScenePanel>();
    AddIndependentChild<AppStatusPanel>();
}

ImGuiID g_DockspaceID { 0U };
bool    g_IsDockspaceInitialized { false };
bool    g_EnableViewport { true };

void AppWindow::PrePaint()
{
    ImGuiViewport const *const Viewport = ImGui::GetMainViewport();
    g_DockspaceID                       = ImGui::DockSpaceOverViewport(Viewport, ImGuiDockNodeFlags_PassthruCentralNode);

    if (!g_IsDockspaceInitialized)
    {
        if (ImGuiDockNode const *const Node = ImGui::DockBuilderGetNode(g_DockspaceID);
            Node != nullptr && !Node->IsSplitNode())
        {
            SetDockingLayout();
        }
        g_IsDockspaceInitialized = true;
    }
}

void AppWindow::OnInitialize()
{
    SetStyle();

    RenderCore::Renderer::SetRenderOffscreen(true);
    AddIndependentChild<AppViewport>();
}

void AppWindow::SetDockingLayout()
{
    ImGuiID TempNodeID = g_DockspaceID;

    if (g_EnableViewport)
    {
        ImGuiDockNode const *const CentralNode = ImGui::DockBuilderGetCentralNode(TempNodeID);
        ImGui::DockBuilderDockWindow("Viewport", CentralNode->ID);
    }

    ImGuiID const LeftID = ImGui::DockBuilderSplitNode(TempNodeID, ImGuiDir_Left, 0.3F, nullptr, &TempNodeID);

    ImGui::DockBuilderDockWindow("Scene", LeftID);
    ImGui::DockBuilderDockWindow("Status", LeftID);
    ImGui::DockBuilderDockWindow("Radeon Profiler", LeftID);

    ImGui::DockBuilderFinish(g_DockspaceID);
}

void AppWindow::SetStyle()
{
    ImGuiStyle &ImGuiStyle = ImGui::GetStyle();

    ImGuiStyle.WindowPadding     = ImVec2(8.00F, 8.00F);
    ImGuiStyle.FramePadding      = ImVec2(5.00F, 2.00F);
    ImGuiStyle.CellPadding       = ImVec2(6.00F, 6.00F);
    ImGuiStyle.ItemSpacing       = ImVec2(6.00F, 6.00F);
    ImGuiStyle.ItemInnerSpacing  = ImVec2(6.00F, 6.00F);
    ImGuiStyle.TouchExtraPadding = ImVec2(0.00F, 0.00F);
    ImGuiStyle.IndentSpacing     = 25.F;
    ImGuiStyle.ScrollbarSize     = 15.F;
    ImGuiStyle.GrabMinSize       = 10.F;
    ImGuiStyle.WindowBorderSize  = 1.F;
    ImGuiStyle.ChildBorderSize   = 1.F;
    ImGuiStyle.PopupBorderSize   = 1.F;
    ImGuiStyle.FrameBorderSize   = 1.F;
    ImGuiStyle.TabBorderSize     = 1.F;
    ImGuiStyle.WindowRounding    = 7.F;
    ImGuiStyle.ChildRounding     = 4.F;
    ImGuiStyle.FrameRounding     = 3.F;
    ImGuiStyle.PopupRounding     = 4.F;
    ImGuiStyle.ScrollbarRounding = 9.F;
    ImGuiStyle.GrabRounding      = 3.F;
    ImGuiStyle.LogSliderDeadzone = 4.F;
    ImGuiStyle.TabRounding       = 4.F;

    ImVec4 *ImGuiColors = ImGuiStyle.Colors;

    ImGuiColors[ImGuiCol_Text]                  = ImVec4(1.00F, 1.00F, 1.00F, 1.00F);
    ImGuiColors[ImGuiCol_TextDisabled]          = ImVec4(0.50F, 0.50F, 0.50F, 1.00F);
    ImGuiColors[ImGuiCol_WindowBg]              = ImVec4(0.10F, 0.10F, 0.10F, 1.00F);
    ImGuiColors[ImGuiCol_ChildBg]               = ImVec4(0.00F, 0.00F, 0.00F, 0.00F);
    ImGuiColors[ImGuiCol_PopupBg]               = ImVec4(0.19F, 0.19F, 0.19F, 0.92F);
    ImGuiColors[ImGuiCol_Border]                = ImVec4(0.19F, 0.19F, 0.19F, 0.29F);
    ImGuiColors[ImGuiCol_BorderShadow]          = ImVec4(0.00F, 0.00F, 0.00F, 0.24F);
    ImGuiColors[ImGuiCol_FrameBg]               = ImVec4(0.05F, 0.05F, 0.05F, 0.54F);
    ImGuiColors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19F, 0.19F, 0.19F, 0.54F);
    ImGuiColors[ImGuiCol_FrameBgActive]         = ImVec4(0.20F, 0.22F, 0.23F, 1.00F);
    ImGuiColors[ImGuiCol_TitleBg]               = ImVec4(0.00F, 0.00F, 0.00F, 1.00F);
    ImGuiColors[ImGuiCol_TitleBgActive]         = ImVec4(0.06F, 0.06F, 0.06F, 1.00F);
    ImGuiColors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00F, 0.00F, 0.00F, 1.00F);
    ImGuiColors[ImGuiCol_MenuBarBg]             = ImVec4(0.14F, 0.14F, 0.14F, 1.00F);
    ImGuiColors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05F, 0.05F, 0.05F, 0.54F);
    ImGuiColors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34F, 0.34F, 0.34F, 0.54F);
    ImGuiColors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40F, 0.40F, 0.40F, 0.54F);
    ImGuiColors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56F, 0.56F, 0.56F, 0.54F);
    ImGuiColors[ImGuiCol_CheckMark]             = ImVec4(0.33F, 0.67F, 0.86F, 1.00F);
    ImGuiColors[ImGuiCol_SliderGrab]            = ImVec4(0.34F, 0.34F, 0.34F, 0.54F);
    ImGuiColors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56F, 0.56F, 0.56F, 0.54F);
    ImGuiColors[ImGuiCol_Button]                = ImVec4(0.05F, 0.05F, 0.05F, 0.54F);
    ImGuiColors[ImGuiCol_ButtonHovered]         = ImVec4(0.19F, 0.19F, 0.19F, 0.54F);
    ImGuiColors[ImGuiCol_ButtonActive]          = ImVec4(0.20F, 0.22F, 0.23F, 1.00F);
    ImGuiColors[ImGuiCol_Header]                = ImVec4(0.00F, 0.00F, 0.00F, 0.52F);
    ImGuiColors[ImGuiCol_HeaderHovered]         = ImVec4(0.00F, 0.00F, 0.00F, 0.36F);
    ImGuiColors[ImGuiCol_HeaderActive]          = ImVec4(0.20F, 0.22F, 0.23F, 0.33F);
    ImGuiColors[ImGuiCol_Separator]             = ImVec4(0.28F, 0.28F, 0.28F, 0.29F);
    ImGuiColors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44F, 0.44F, 0.44F, 0.29F);
    ImGuiColors[ImGuiCol_SeparatorActive]       = ImVec4(0.40F, 0.44F, 0.47F, 1.00F);
    ImGuiColors[ImGuiCol_ResizeGrip]            = ImVec4(0.28F, 0.28F, 0.28F, 0.29F);
    ImGuiColors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44F, 0.44F, 0.44F, 0.29F);
    ImGuiColors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40F, 0.44F, 0.47F, 1.00F);
    ImGuiColors[ImGuiCol_Tab]                   = ImVec4(0.00F, 0.00F, 0.00F, 0.52F);
    ImGuiColors[ImGuiCol_TabHovered]            = ImVec4(0.14F, 0.14F, 0.14F, 1.00F);
    ImGuiColors[ImGuiCol_TabActive]             = ImVec4(0.20F, 0.20F, 0.20F, 0.36F);
    ImGuiColors[ImGuiCol_TabUnfocused]          = ImVec4(0.00F, 0.00F, 0.00F, 0.52F);
    ImGuiColors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14F, 0.14F, 0.14F, 1.00F);
    ImGuiColors[ImGuiCol_DockingPreview]        = ImVec4(0.33F, 0.67F, 0.86F, 1.00F);
    ImGuiColors[ImGuiCol_DockingEmptyBg]        = ImVec4(1.00F, 0.00F, 0.00F, 1.00F);
    ImGuiColors[ImGuiCol_PlotLines]             = ImVec4(1.00F, 0.00F, 0.00F, 1.00F);
    ImGuiColors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00F, 0.00F, 0.00F, 1.00F);
    ImGuiColors[ImGuiCol_PlotHistogram]         = ImVec4(1.00F, 0.00F, 0.00F, 1.00F);
    ImGuiColors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00F, 0.00F, 0.00F, 1.00F);
    ImGuiColors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00F, 0.00F, 0.00F, 0.52F);
    ImGuiColors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00F, 0.00F, 0.00F, 0.52F);
    ImGuiColors[ImGuiCol_TableBorderLight]      = ImVec4(0.28F, 0.28F, 0.28F, 0.29F);
    ImGuiColors[ImGuiCol_TableRowBg]            = ImVec4(0.00F, 0.00F, 0.00F, 0.00F);
    ImGuiColors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00F, 1.00F, 1.00F, 0.06F);
    ImGuiColors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20F, 0.22F, 0.23F, 1.00F);
    ImGuiColors[ImGuiCol_DragDropTarget]        = ImVec4(0.33F, 0.67F, 0.86F, 1.00F);
    ImGuiColors[ImGuiCol_NavHighlight]          = ImVec4(1.00F, 0.00F, 0.00F, 1.00F);
    ImGuiColors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00F, 0.00F, 0.00F, 0.70F);
    ImGuiColors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00F, 0.00F, 0.00F, 0.20F);
    ImGuiColors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00F, 0.00F, 0.00F, 0.35F);
}
