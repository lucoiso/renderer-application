// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <algorithm>
#include <chrono>
#include <format>
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

void AppStatusPanel::CreateStatusPanel()
{
    if (ImGui::CollapsingHeader("Scene Status", ImGuiTreeNodeFlags_DefaultOpen) && ImGui::IsItemVisible())
    {
        CreateRendererPanel();
        CreateCameraPanel();
    }
}

void AppStatusPanel::CreateRendererPanel()
{
    if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen) && ImGui::IsItemVisible())
    {
        static float UpdateInterval = 1.F;
        static float FrameTime      = 0.F;

        {
            static auto LastTime    = std::chrono::high_resolution_clock::now();
            auto const  CurrentTime = std::chrono::high_resolution_clock::now();

            auto const     Milliseconds = std::chrono::duration<double, std::milli>(CurrentTime - LastTime).count();
            constexpr auto Denominator  = static_cast<double>(std::milli::den);

            if (auto const Seconds = Milliseconds / Denominator;
                Seconds >= UpdateInterval)
            {
                LastTime  = CurrentTime;
                FrameTime = RenderCore::Renderer::GetFrameTime();
            }
        }

        ImGui::Text(std::data(std::format("Frame Time: {:.3f}ms\nFrame Rate: {:.0f} FPS",
                                          FrameTime <= 0.F ? 0.F : FrameTime * 1000.F,
                                          FrameTime <= 0.F ? 0.F : 1.f / FrameTime)));

        static bool EnableVSync = RenderCore::Renderer::GetVSync();
        bool        NewVSync    = EnableVSync;
        if (ImGui::Checkbox("VSync Enabled", &NewVSync) && ImGui::IsItemVisible() && EnableVSync != NewVSync)
        {
            EnableVSync = NewVSync;
            RenderCore::Renderer::SetVSync(EnableVSync);
            RenderCore::Renderer::RequestUpdateResources();
        }

        static float MaxFPS = 1.0 / RenderCore::Renderer::GetFPSLimit();
        if (ImGui::InputFloat("Max FPS", &MaxFPS, 1.F, 1.F, "%.0f") && ImGui::IsItemVisible())
        {
            RenderCore::Renderer::SetFPSLimit(MaxFPS);
        }

        if (ImGui::InputFloat("Interval", &UpdateInterval, 0.1F, 1.F, "%.2f") && ImGui::IsItemVisible())
        {
            UpdateInterval = std::clamp(UpdateInterval, 0.1F, 5.F);
        }
    }
}

void AppStatusPanel::CreateCameraPanel()
{
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen) && ImGui::IsItemVisible())
    {
        ImGui::Text(std::data(std::format("Position: {:.2f}, {:.2f}, {:.2f}\nRotation: {:.2f}, {:.2f}, {:.2f}\nMovement State: {}",
                                          RenderCore::Renderer::GetCamera().GetPosition().x,
                                          RenderCore::Renderer::GetCamera().GetPosition().y,
                                          RenderCore::Renderer::GetCamera().GetPosition().z,
                                          RenderCore::Renderer::GetCamera().GetRotation().x,
                                          RenderCore::Renderer::GetCamera().GetRotation().y,
                                          RenderCore::Renderer::GetCamera().GetRotation().z,
                                          static_cast<std::underlying_type_t<RenderCore::CameraMovementStateFlags>>(RenderCore::Renderer::GetCamera().
                                              GetCameraMovementStateFlags()))));

        static float CameraSpeed = RenderCore::Renderer::GetCamera().GetSpeed();
        if (ImGui::InputFloat("Speed", &CameraSpeed, 0.1F, 1.F, "%.2f") && ImGui::IsItemVisible())
        {
            RenderCore::Renderer::GetMutableCamera().SetSpeed(CameraSpeed);
        }

        static float CameraSensitivity = RenderCore::Renderer::GetCamera().GetSensitivity();
        if (ImGui::InputFloat("Sensitivity", &CameraSensitivity, 0.01F, 1.F, "%.2f") && ImGui::IsItemVisible())
        {
            RenderCore::Renderer::GetMutableCamera().SetSensitivity(CameraSensitivity);
        }
    }
}