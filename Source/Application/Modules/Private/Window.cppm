// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/VulkanRenderer

module;

#include <imgui.h>

module Application.Window;

import Application.Window.ScenePanel;
import Application.Window.StatusPanel;

using namespace Application;

AppWindow::AppWindow()
{
    AddChild<StatusPanel>(this);
    AddChild<ScenePanel>(this);
}

void AppWindow::PrePaint()
{
    ImGui::Begin("Options", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
}

void AppWindow::PostPaint()
{
    ImGui::End();
}