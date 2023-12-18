// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/VulkanRenderer

module;

#include <imgui.h>
#include <type_traits>

module Application.Window.StatusPanel;

using namespace Application;

import RenderCore.Renderer;
import RenderCore.Types.Camera;
import RenderCore.Types.Object;

StatusPanel::StatusPanel(Control* const Parent, AppWindow* const Window)
    : Control(Parent), m_Window(Window)
{
}

void StatusPanel::Paint()
{
    if (m_Window && ImGui::CollapsingHeader("Status "))
    {
        ImGui::Text("Frame Rate: %.3fms", m_Window->GetRenderer().GetFrameTime());
        ImGui::Text("Frame Time: %.0f FPS", 1.f / m_Window->GetRenderer().GetFrameTime());

        float MaxFPS = 1.0 / m_Window->GetRenderer().GetFrameRateCap();
        ImGui::InputFloat("Max FPS", &MaxFPS, 1.F, 1.F, "%.0f");
        m_Window->GetRenderer().SetFrameRateCap(MaxFPS);

        ImGui::Text("Camera Position: %.2f, %.2f, %.2f", m_Window->GetRenderer().GetCamera().GetPosition().X, m_Window->GetRenderer().GetCamera().GetPosition().X, m_Window->GetRenderer().GetCamera().GetPosition().Z);
        ImGui::Text("Camera Yaw: %.2f", m_Window->GetRenderer().GetCamera().GetRotation().Yaw);
        ImGui::Text("Camera Pitch: %.2f", m_Window->GetRenderer().GetCamera().GetRotation().Pitch);
        ImGui::Text("Camera Movement State: %d", static_cast<std::underlying_type_t<RenderCore::CameraMovementStateFlags>>(m_Window->GetRenderer().GetCamera().GetCameraMovementStateFlags()));

        float CameraSpeed = m_Window->GetRenderer().GetCamera().GetSpeed();
        ImGui::InputFloat("Camera Speed", &CameraSpeed, 0.1F, 1.F, "%.2f");
        m_Window->GetRenderer().GetMutableCamera().SetSpeed(CameraSpeed);

        float CameraSensitivity = m_Window->GetRenderer().GetCamera().GetSensitivity();
        ImGui::InputFloat("Camera Sensitivity", &CameraSensitivity, 0.1F, 1.F, "%.2f");
        m_Window->GetRenderer().GetMutableCamera().SetSensitivity(CameraSensitivity);
    }
}