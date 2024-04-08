// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <filesystem>
#include <imgui.h>
#include <numeric>
#include <unordered_map>
#include <glm/gtc/quaternion.hpp>

module UserInterface.ScenePanel;

import RenderCore.Renderer;
import RenderCore.Types.Camera;
import RenderCore.Types.Illumination;
import RenderCore.Types.Object;

using namespace UserInterface;

static std::unordered_map<std::string, std::string> const s_OptionsMap = [](std::string_view const Root, std::vector<std::string> const &Extensions)
{
    std::unordered_map<std::string, std::string> OptionsMap { { "None", "" } };
    try
    {
        for (auto const &Entry : std::filesystem::recursive_directory_iterator(Root))
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
}("Models", { ".gltf", ".glb" });

constexpr auto     OptionNone     = "None";
static std::string s_SelectedItem = OptionNone;
static std::string s_ModelPath    = s_OptionsMap.at(s_SelectedItem);

AppScenePanel::AppScenePanel(Control *const Parent)
    : Control(Parent)
{
}

void AppScenePanel::Paint()
{
    if (ImGui::Begin("Scene"))
    {
        CreateIlluminationPanel();
        CreateInfoPanel();
        CreateObjectsList();
    }
    ImGui::End();
}

void AppScenePanel::CreateInfoPanel() const
{
    if (ImGui::CollapsingHeader("Current Scene", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto const &Objects = RenderCore::Renderer::GetObjects();

        if (std::empty(Objects))
        {
            s_SelectedItem = OptionNone;
        }

        if (ImGui::BeginCombo("glTF Scene", std::data(s_SelectedItem)))
        {
            bool SelectionChanged = false;

            for (auto const &[Name, Path] : s_OptionsMap)
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
                RenderCore::Renderer::RequestDestroyObjects();
                if (!std::empty(s_ModelPath))
                {
                    RenderCore::Renderer::RequestLoadObject(s_ModelPath);
                }
            }
        }

        ImGui::Text("Loaded Objects: %d", RenderCore::Renderer::GetNumObjects());
        ImGui::Text("Total Triangles: %d",
                    std::accumulate(std::begin(Objects),
                                    std::end(Objects),
                                    0U,
                                    [](std::uint32_t const Sum, auto const &Object)
                                    {
                                        return Sum + Object.GetNumTriangles();
                                    }));

        if (!std::empty(Objects))
        {
            if (ImGui::Button("Reload Scene"))
            {
                RenderCore::Renderer::RequestDestroyObjects();
                RenderCore::Renderer::RequestLoadObject(s_ModelPath);
            }

            ImGui::SameLine();

            if (ImGui::Button("Destroy All"))
            {
                RenderCore::Renderer::RequestDestroyObjects();
            }
        }
    }
}

void AppScenePanel::CreateIlluminationPanel() const
{
    if (ImGui::CollapsingHeader("Illumination", ImGuiTreeNodeFlags_DefaultOpen))
    {
        RenderCore::Illumination &IlluminationConfig = RenderCore::Renderer::GetMutableIllumination();

        float LightPosition[3] = { IlluminationConfig.GetPosition().x, IlluminationConfig.GetPosition().y, IlluminationConfig.GetPosition().z };
        ImGui::InputFloat3("Light Position", &LightPosition[0], "%.2f");
        IlluminationConfig.SetPosition({ LightPosition[0], LightPosition[1], LightPosition[2] });

        float LightColor[3] = { IlluminationConfig.GetColor().x, IlluminationConfig.GetColor().y, IlluminationConfig.GetColor().z };
        ImGui::InputFloat3("Light Color", &LightColor[0], "%.2f");
        IlluminationConfig.SetColor({
                                            std::clamp(LightColor[0], 0.F, FLT_MAX),
                                            std::clamp(LightColor[1], 0.F, FLT_MAX),
                                            std::clamp(LightColor[2], 0.F, FLT_MAX)
                                    });

        float LightIntensity = IlluminationConfig.GetIntensity();
        ImGui::InputFloat("Light Intensity", &LightIntensity, 0.1f);
        IlluminationConfig.SetIntensity(std::clamp(LightIntensity, 0.F, FLT_MAX));
    }
}

void AppScenePanel::CreateObjectsList() const
{
    if (auto &Objects = RenderCore::Renderer::GetMutableObjects();
        ImGui::CollapsingHeader(std::data(std::format("Loaded Objects: {} ", RenderCore::Renderer::GetNumObjects())), ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (auto &Object : Objects)
        {
            if (Object.IsPendingDestroy())
            {
                continue;
            }

            if (ImGui::CollapsingHeader(std::data(std::format("[{}] {}", Object.GetID(), Object.GetName()))))
            {
                ImGui::Text("ID: %d", Object.GetID());
                ImGui::Text("Name: %s", std::data(Object.GetName()));
                ImGui::Text("Path: %s", std::data(Object.GetPath()));
                ImGui::Text("Triangles Count: %d", Object.GetNumTriangles());

                ImGui::Separator();

                ImGui::Text("Transform");
                float Position[3] = { Object.GetPosition().x, Object.GetPosition().y, Object.GetPosition().z };
                ImGui::InputFloat3(std::data(std::format("{} Position", Object.GetName())), &Position[0], "%.2f");
                Object.SetPosition({ Position[0], Position[1], Position[2] });

                float Scale[3] = { Object.GetScale().x, Object.GetScale().y, Object.GetScale().z };
                ImGui::InputFloat3(std::data(std::format("{} Scale", Object.GetName())), &Scale[0], "%.2f");
                Object.SetScale({ Scale[0], Scale[1], Scale[2] });

                float Rotation[3] = { Object.GetRotation().x, Object.GetRotation().y, Object.GetRotation().z };
                ImGui::InputFloat3(std::data(std::format("{} Rotation", Object.GetName())), &Rotation[0], "%.2f");
                Object.SetRotation({ Rotation[0], Rotation[1], Rotation[2] });

                if (ImGui::Button(std::data(std::format("Destroy {}", Object.GetName()))))
                {
                    Object.Destroy();
                }
            }
        }
    }
}
