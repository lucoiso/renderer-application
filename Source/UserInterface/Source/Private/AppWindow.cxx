// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <imgui_internal.h>

module UserInterface.Window;

import UserInterface.ScenePanel;
import UserInterface.StatusPanel;
import luGUI.UserInterface.Controls.Viewport;

using namespace UserInterface;

AppWindow::AppWindow()
{
    AddChild<AppScenePanel>();
    AddChild<AppStatusPanel>();
}

ImGuiID g_DockspaceID { 0U };
bool    g_IsDockspaceInitialized { false };
bool    g_EnableViewport { true };

void AppWindow::PrePaint()
{
    ImGuiViewport const *const Viewport = ImGui::GetMainViewport();
    g_DockspaceID                       = ImGui::DockSpaceOverViewport(ImGuiID { 0U }, Viewport, ImGuiDockNodeFlags_PassthruCentralNode);

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
    AddChild<luGUI::Viewport>();
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

    ImGui::DockBuilderFinish(g_DockspaceID);
}

void AppWindow::SetStyle()
{
    /* Using: Moonlight style by Madam-Herta from ImThemes */

    ImGuiStyle &ImGuiStyle = ImGui::GetStyle();

    ImGuiStyle.Alpha = 1.0F;
    ImGuiStyle.DisabledAlpha = 1.0F;
    ImGuiStyle.WindowPadding = ImVec2(12.0F, 12.0F);
    ImGuiStyle.WindowRounding = 11.5F;
    ImGuiStyle.WindowBorderSize = 0.0F;
    ImGuiStyle.WindowMinSize = ImVec2(20.0F, 20.0F);
    ImGuiStyle.WindowTitleAlign = ImVec2(0.5f, 0.5F);
    ImGuiStyle.WindowMenuButtonPosition = ImGuiDir_Right;
    ImGuiStyle.ChildRounding = 0.0F;
    ImGuiStyle.ChildBorderSize = 1.0F;
    ImGuiStyle.PopupRounding = 0.0F;
    ImGuiStyle.PopupBorderSize = 1.0F;
    ImGuiStyle.FramePadding = ImVec2(20.0F, 3.400000095367432F);
    ImGuiStyle.FrameRounding = 11.89999961853027F;
    ImGuiStyle.FrameBorderSize = 0.0F;
    ImGuiStyle.ItemSpacing = ImVec2(4.300000190734863f, 5.5F);
    ImGuiStyle.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284F);
    ImGuiStyle.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137F);
    ImGuiStyle.IndentSpacing = 0.0F;
    ImGuiStyle.ColumnsMinSpacing = 4.900000095367432F;
    ImGuiStyle.ScrollbarSize = 11.60000038146973F;
    ImGuiStyle.ScrollbarRounding = 15.89999961853027F;
    ImGuiStyle.GrabMinSize = 3.700000047683716F;
    ImGuiStyle.GrabRounding = 20.0F;
    ImGuiStyle.TabRounding = 0.0F;
    ImGuiStyle.TabBorderSize = 0.0F;
    ImGuiStyle.TabMinWidthForCloseButton = 0.0F;
    ImGuiStyle.ColorButtonPosition = ImGuiDir_Right;
    ImGuiStyle.ButtonTextAlign = ImVec2(0.5f, 0.5F);
    ImGuiStyle.SelectableTextAlign = ImVec2(0.0F, 0.0F);

    ImVec4 *const ImGuiColors = ImGuiStyle.Colors;

    ImGuiColors[ImGuiCol_Text] = ImVec4(1.0F, 1.0F, 1.0F, 1.0F);
    ImGuiColors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0F);
    ImGuiColors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0F);
    ImGuiColors[ImGuiCol_ChildBg] = ImVec4(0.09411764889955521f, 0.1019607856869698f, 0.1176470592617989f, 1.0F);
    ImGuiColors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0F);
    ImGuiColors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0F);
    ImGuiColors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0F);
    ImGuiColors[ImGuiCol_FrameBg] = ImVec4(0.1137254908680916f, 0.125490203499794f, 0.1529411822557449f, 1.0F);
    ImGuiColors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0F);
    ImGuiColors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0F);
    ImGuiColors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0F);
    ImGuiColors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0F);
    ImGuiColors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0F);
    ImGuiColors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0F);
    ImGuiColors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0F);
    ImGuiColors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0F);
    ImGuiColors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0F);
    ImGuiColors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0F);
    ImGuiColors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0F, 0.4980392158031464f, 1.0F);
    ImGuiColors[ImGuiCol_SliderGrab] = ImVec4(0.9725490212440491f, 1.0F, 0.4980392158031464f, 1.0F);
    ImGuiColors[ImGuiCol_SliderGrabActive] = ImVec4(1.0F, 0.7960784435272217f, 0.4980392158031464f, 1.0F);
    ImGuiColors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0F);
    ImGuiColors[ImGuiCol_ButtonHovered] = ImVec4(0.1803921610116959f, 0.1882352977991104f, 0.196078434586525f, 1.0F);
    ImGuiColors[ImGuiCol_ButtonActive] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0F);
    ImGuiColors[ImGuiCol_Header] = ImVec4(0.1411764770746231f, 0.1647058874368668f, 0.2078431397676468f, 1.0F);
    ImGuiColors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0F);
    ImGuiColors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0F);
    ImGuiColors[ImGuiCol_Separator] = ImVec4(0.1294117718935013f, 0.1490196138620377f, 0.1921568661928177f, 1.0F);
    ImGuiColors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0F);
    ImGuiColors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0F);
    ImGuiColors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0F);
    ImGuiColors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0F, 0.4980392158031464f, 1.0F);
    ImGuiColors[ImGuiCol_ResizeGripActive] = ImVec4(1.0F, 1.0F, 1.0F, 1.0F);
    ImGuiColors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0F);
    ImGuiColors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0F);
    ImGuiColors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0F);
    ImGuiColors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0F);
    ImGuiColors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.125490203499794f, 0.2745098173618317f, 0.572549045085907f, 1.0F);
    ImGuiColors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0F);
    ImGuiColors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0F);
    ImGuiColors[ImGuiCol_PlotHistogram] = ImVec4(0.8823529481887817f, 0.7960784435272217f, 0.5607843399047852f, 1.0F);
    ImGuiColors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.95686274766922f, 0.95686274766922f, 0.95686274766922f, 1.0F);
    ImGuiColors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0F);
    ImGuiColors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0F);
    ImGuiColors[ImGuiCol_TableBorderLight] = ImVec4(0.0F, 0.0F, 0.0F, 1.0F);
    ImGuiColors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0F);
    ImGuiColors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0F);
    ImGuiColors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0F);
    ImGuiColors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0F, 1.0F);
    ImGuiColors[ImGuiCol_NavHighlight] = ImVec4(0.2666666805744171f, 0.2901960909366608f, 1.0F, 1.0F);
    ImGuiColors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0F, 1.0F);
    ImGuiColors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176F);
    ImGuiColors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176F);
}
