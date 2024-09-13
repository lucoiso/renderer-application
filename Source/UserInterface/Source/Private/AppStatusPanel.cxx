// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

module UserInterface.StatusPanel;

import RenderCore.Renderer;
import RenderCore.Runtime.Scene;
import RenderCore.Runtime.Memory;

using namespace UserInterface;

AppStatusPanel::AppStatusPanel(Control *const Parent)
    : Control(Parent)
{
}

void AppStatusPanel::Paint()
{
    if (ImGui::Begin("Status"))
    {
        CreateRendererPanel();
        CreateCameraPanel();
    }
    ImGui::End();
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
        if (ImGui::Checkbox("VSync", &NewVSync) && ImGui::IsItemVisible() && EnableVSync != NewVSync)
        {
            EnableVSync = NewVSync;
            RenderCore::Renderer::SetVSync(EnableVSync);
        }

        static bool EnableDefaultSync = RenderCore::Renderer::GetUseDefaultSync();
        bool        NewDefaultSync    = EnableDefaultSync;
        if (ImGui::Checkbox("Default Sync", &NewDefaultSync) && ImGui::IsItemVisible() && EnableDefaultSync != NewDefaultSync)
        {
            EnableDefaultSync = NewDefaultSync;
            RenderCore::Renderer::SetUseDefaultSync(EnableDefaultSync);
        }

        static float MaxFPS = 1.F / RenderCore::Renderer::GetFPSLimit();
        if (ImGui::InputFloat("Max FPS", &MaxFPS, 1.F, 1.F, "%.0f") && ImGui::IsItemVisible())
        {
            RenderCore::Renderer::SetFPSLimit(MaxFPS);
        }

        ImGui::SliderFloat("Interval", &UpdateInterval, 0.1F, 10.F, "%.2f");

        static strzilla::string s_StatusString {};

        if (ImGui::Button("Print Allocator Status"))
        {
            s_StatusString = RenderCore::GetMemoryAllocatorStats(true);
            ImGui::OpenPopup("Allocator Status");
            ImGui::SetNextWindowSize(ImVec2(800, 600));
        }

        if (ImGui::BeginPopupModal("Allocator Status"))
        {
            ImGui::TextUnformatted(std::data(s_StatusString));

            if (ImGui::Button("Close Allocator Status Panel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}

void AppStatusPanel::CreateCameraPanel()
{
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_CollapsingHeader) && ImGui::IsItemVisible())
    {
        ImGui::Text(std::data(std::format("Position: {:.2f}, {:.2f}, {:.2f}\nRotation: {:.2f}, {:.2f}, {:.2f}\nMovement State: {}",
                                          RenderCore::GetCamera().GetPosition().x,
                                          RenderCore::GetCamera().GetPosition().y,
                                          RenderCore::GetCamera().GetPosition().z,
                                          RenderCore::GetCamera().GetRotation().x,
                                          RenderCore::GetCamera().GetRotation().y,
                                          RenderCore::GetCamera().GetRotation().z,
                                          static_cast<std::underlying_type_t<RenderCore::CameraMovementStateFlags>>(RenderCore::GetCamera().
                                              GetCameraMovementStateFlags()))));

        static float CameraSpeed = RenderCore::GetCamera().GetSpeed();
        if (ImGui::SliderFloat("Speed", &CameraSpeed, 0.1F, 50.F, "%.2f") && ImGui::IsItemVisible())
        {
            RenderCore::GetCamera().SetSpeed(CameraSpeed);
        }

        static float CameraSensitivity = RenderCore::GetCamera().GetSensitivity();
        if (ImGui::SliderFloat("Sensitivity", &CameraSensitivity, 0.01F, 10.F, "%.2f") && ImGui::IsItemVisible())
        {
            RenderCore::GetCamera().SetSensitivity(CameraSensitivity);
        }
    }
}
