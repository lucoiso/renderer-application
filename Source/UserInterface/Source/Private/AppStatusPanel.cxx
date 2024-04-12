// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <imgui.h>
#include <type_traits>

module UserInterface.StatusPanel;

import RenderCore.Renderer;
import RenderCore.Types.Camera;
import RenderCore.Types.Object;
import RenderCore.Types.RendererStateFlags;

using namespace UserInterface;

AppStatusPanel::AppStatusPanel(Control *const Parent)
    : Control(Parent)
{
}

void AppStatusPanel::Paint()
{
    if (ImGui::Begin("Status"))
    {
        CreateStatusPanel();
    }
    ImGui::End();
}

void AppStatusPanel::CreateStatusPanel() const
{
    if (ImGui::CollapsingHeader("Scene Status", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Renderer");

        ImGui::Text("Frame Time: %.5fms", RenderCore::Renderer::GetFrameTime() * 1000.F);
        ImGui::Text("Frame Rate: %.0f FPS", 1.f / RenderCore::Renderer::GetFrameTime());

        float MaxFPS = 1.0 / RenderCore::Renderer::GetFrameRateCap();
        ImGui::InputFloat("Max FPS", &MaxFPS, 1.F, 1.F, "%.0f");
        RenderCore::Renderer::SetFrameRateCap(MaxFPS);

        ImGui::Text("Renderer Flags: %d", static_cast<std::underlying_type_t<RenderCore::RendererStateFlags>>(RenderCore::Renderer::GetStateFlags()));

        ImGui::Separator();

        ImGui::Text("Camera");

        ImGui::Text("Position: %.2f, %.2f, %.2f",
                    RenderCore::Renderer::GetCamera().GetPosition().x,
                    RenderCore::Renderer::GetCamera().GetPosition().y,
                    RenderCore::Renderer::GetCamera().GetPosition().z);

        ImGui::Text("Rotation: %.2f, %.2f, %.2f",
                    RenderCore::Renderer::GetCamera().GetRotation().x,
                    RenderCore::Renderer::GetCamera().GetRotation().y,
                    RenderCore::Renderer::GetCamera().GetRotation().z);

        ImGui::Text("Movement State: %d",
                    static_cast<std::underlying_type_t<RenderCore::CameraMovementStateFlags>>(RenderCore::Renderer::GetCamera().
                        GetCameraMovementStateFlags()));

        float CameraSpeed = RenderCore::Renderer::GetCamera().GetSpeed();
        ImGui::InputFloat("Speed", &CameraSpeed, 0.1F, 1.F, "%.2f");
        RenderCore::Renderer::GetMutableCamera().SetSpeed(CameraSpeed);

        float CameraSensitivity = RenderCore::Renderer::GetCamera().GetSensitivity();
        ImGui::InputFloat("Sensitivity", &CameraSensitivity, 0.01F, 1.F, "%.2f");
        RenderCore::Renderer::GetMutableCamera().SetSensitivity(CameraSensitivity);
    }
}
