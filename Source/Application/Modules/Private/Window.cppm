// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/VulkanRenderer

module;

#include <filesystem>
#include <imgui.h>
#include <numeric>

module Application.Window;

using namespace Application;

import RenderCore.Renderer;
import RenderCore.Types.Camera;
import RenderCore.Types.Object;

std::unordered_map<std::string, std::string> GetAvailableglTFAssetsInDirectory(std::string const& Root, std::vector<std::string> const& Extensions)
{
    std::unordered_map<std::string, std::string> OptionsMap {{"None", ""}};
    try
    {
        for (auto const& Entry: std::filesystem::recursive_directory_iterator(Root))
        {
            if (Entry.is_regular_file() && std::ranges::find(Extensions, Entry.path().extension()) != std::cend(Extensions))
            {
                OptionsMap.emplace(Entry.path().stem().string(), Entry.path().string());
            }
        }
    }
    catch (...)
    {
    }

    return OptionsMap;
}

void AppWindow::CreateOverlay()
{
    ImGui::Begin("Options", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    {
        constexpr auto OptionNone                                              = "None";
        static std::unordered_map<std::string, std::string> const s_OptionsMap = GetAvailableglTFAssetsInDirectory("Resources/Assets", {".gltf", ".glb"});
        static std::string s_SelectedItem                                      = OptionNone;
        static std::string s_ModelPath                                         = s_OptionsMap.at(s_SelectedItem);

        if (ImGui::CollapsingHeader("Status "))
        {
            ImGui::Text("Frame Rate: %.3fms", GetRenderer().GetFrameTime());
            ImGui::Text("Frame Time: %.0f FPS", 1.f / GetRenderer().GetFrameTime());

            float MaxFPS = 1.0 / GetRenderer().GetFrameRateCap();
            ImGui::InputFloat("Max FPS", &MaxFPS, 1.F, 1.F, "%.0f");
            GetRenderer().SetFrameRateCap(MaxFPS);

            ImGui::Text("Camera Position: %.2f, %.2f, %.2f", GetRenderer().GetCamera().GetPosition().X, GetRenderer().GetCamera().GetPosition().X, GetRenderer().GetCamera().GetPosition().Z);
            ImGui::Text("Camera Yaw: %.2f", GetRenderer().GetCamera().GetRotation().Yaw);
            ImGui::Text("Camera Pitch: %.2f", GetRenderer().GetCamera().GetRotation().Pitch);
            ImGui::Text("Camera Movement State: %d", static_cast<std::underlying_type_t<RenderCore::CameraMovementStateFlags>>(GetRenderer().GetCamera().GetCameraMovementStateFlags()));

            float CameraSpeed = GetRenderer().GetCamera().GetSpeed();
            ImGui::InputFloat("Camera Speed", &CameraSpeed, 0.1F, 1.F, "%.2f");
            GetRenderer().GetMutableCamera().SetSpeed(CameraSpeed);

            float CameraSensitivity = GetRenderer().GetCamera().GetSensitivity();
            ImGui::InputFloat("Camera Sensitivity", &CameraSensitivity, 0.1F, 1.F, "%.2f");
            GetRenderer().GetMutableCamera().SetSensitivity(CameraSensitivity);

            if (GetRenderer().GetNumObjects() == 0U)
            {
                s_SelectedItem = OptionNone;
            }

            if (ImGui::BeginCombo("glTF Scene", std::data(s_SelectedItem)))
            {
                bool SelectionChanged = false;

                for (auto const& [Name, Path]: s_OptionsMap)
                {
                    bool const bIsSelected = s_SelectedItem == Name;
                    if (ImGui::Selectable(std::data(Name), bIsSelected))
                    {
                        s_SelectedItem   = Name;
                        s_ModelPath      = Path;
                        SelectionChanged = true;
                    }

                    if (bIsSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();

                if (SelectionChanged)
                {
                    GetRenderer().UnloadAllScenes();
                    if (!std::empty(s_ModelPath))
                    {
                        [[maybe_unused]] auto const _ = GetRenderer().LoadScene(s_ModelPath);
                    }
                }
            }
        }

        if (auto const& Objects = GetRenderer().GetObjects();
            ImGui::CollapsingHeader(std::format("Loaded Objects: {} ", GetRenderer().GetNumObjects()).c_str()))
        {
            if (!std::empty(Objects))
            {
                ImGui::Text("Loaded Objects: %d", GetRenderer().GetNumObjects());
                ImGui::Text("Total Triangles: %d", std::accumulate(std::begin(Objects),
                                                                   std::end(Objects),
                                                                   0U,
                                                                   [](std::uint32_t const Sum, auto const& Object) {
                                                                       return Sum + Object->GetTrianglesCount();
                                                                   }));

                if (ImGui::Button("Reload Scene"))
                {
                    GetRenderer().UnloadAllScenes();
                    [[maybe_unused]] auto const _ = GetRenderer().LoadScene(s_ModelPath);
                }

                ImGui::SameLine();

                if (ImGui::Button("Destroy All"))
                {
                    GetRenderer().UnloadAllScenes();
                }
            }

            for (auto const& Object: Objects)
            {
                if (!Object)
                {
                    continue;
                }

                if (ImGui::CollapsingHeader(std::format("[{}] {}", Object->GetID(), std::data(Object->GetName())).c_str()))
                {
                    ImGui::Text("ID: %d", Object->GetID());
                    ImGui::Text("Name: %s", std::data(Object->GetName()));
                    ImGui::Text("Path: %s", std::data(Object->GetPath()));
                    ImGui::Text("Triangles Count: %d", Object->GetTrianglesCount());

                    ImGui::Separator();

                    ImGui::Text("Transform");
                    float Position[3] = {Object->GetPosition().X, Object->GetPosition().Y, Object->GetPosition().Z};
                    ImGui::InputFloat3(std::format("{} Position", Object->GetName()).c_str(), &Position[0], "%.2f");
                    Object->SetPosition({Position[0], Position[1], Position[2]});

                    float Scale[3] = {Object->GetScale().X, Object->GetScale().Y, Object->GetScale().Z};
                    ImGui::InputFloat3(std::format("{} Scale", Object->GetName()).c_str(), &Scale[0], "%.2f");
                    Object->SetScale({Scale[0], Scale[1], Scale[2]});

                    float Rotation[3] = {Object->GetRotation().Pitch, Object->GetRotation().Yaw, Object->GetRotation().Roll};
                    ImGui::InputFloat3(std::format("{} Rotation", Object->GetName()).c_str(), &Rotation[0], "%.2f");
                    Object->SetRotation({Rotation[0], Rotation[1], Rotation[2]});

                    if (ImGui::Button(std::format("Destroy {}", Object->GetName()).c_str()))
                    {
                        Object->Destroy();
                    }
                }
            }
        }
    }
    ImGui::End();
}