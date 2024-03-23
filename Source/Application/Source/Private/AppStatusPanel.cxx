// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <imgui.h>
#include <type_traits>

module Application.StatusPanel;

using namespace Application;

import RenderCore.Renderer;
import RenderCore.Types.Camera;
import RenderCore.Types.Object;

AppStatusPanel::AppStatusPanel(Control *const Parent, AppWindow *const Window) : Control(Parent), m_Window(Window)
{
}

void AppStatusPanel::Paint()
{
    if (m_Window && ImGui::CollapsingHeader("Status", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Renderer");

        ImGui::Text("Frame Time: %.3fms", m_Window->GetRenderer().GetFrameTime());
        ImGui::Text("Frame Rate: %.0f FPS", 1.f / m_Window->GetRenderer().GetFrameTime());

        float MaxFPS = 1.0 / m_Window->GetRenderer().GetFrameRateCap();
        ImGui::InputFloat("Max FPS", &MaxFPS, 1.F, 1.F, "%.0f");
        m_Window->GetRenderer().SetFrameRateCap(MaxFPS);

        ImGui::Text("Renderer Flags: %d", static_cast<std::underlying_type_t<RenderCore::RendererStateFlags>>(m_Window->GetRenderer().GetStateFlags()));

        ImGui::Separator();

        ImGui::Text("Camera");

        ImGui::Text("Position: %.2f, %.2f, %.2f",
                    m_Window->GetRenderer().GetCamera().GetPosition().X,
                    m_Window->GetRenderer().GetCamera().GetPosition().X,
                    m_Window->GetRenderer().GetCamera().GetPosition().Z);

        ImGui::Text("Yaw: %.2f", m_Window->GetRenderer().GetCamera().GetRotation().Yaw);
        ImGui::Text("Pitch: %.2f", m_Window->GetRenderer().GetCamera().GetRotation().Pitch);
        ImGui::Text(
                "Movement State: %d",
                static_cast<std::underlying_type_t<RenderCore::CameraMovementStateFlags>>(m_Window->GetRenderer().GetCamera().GetCameraMovementStateFlags()));

        float CameraSpeed = m_Window->GetRenderer().GetCamera().GetSpeed();
        ImGui::InputFloat("Speed", &CameraSpeed, 0.1F, 1.F, "%.2f");
        m_Window->GetRenderer().GetMutableCamera().SetSpeed(CameraSpeed);

        float CameraSensitivity = m_Window->GetRenderer().GetCamera().GetSensitivity();
        ImGui::InputFloat("Sensitivity", &CameraSensitivity, 0.1F, 1.F, "%.2f");
        m_Window->GetRenderer().GetMutableCamera().SetSensitivity(CameraSensitivity);
    }
}
