// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <filesystem>
#include <imgui.h>
#include <numeric>
#include <unordered_map>

module Application.ScenePanel;

using namespace Application;

import RenderCore.Renderer;
import RenderCore.Types.Camera;
import RenderCore.Types.Object;

static std::unordered_map<std::string, std::string> const s_OptionsMap = [](std::string_view const Root, std::vector<std::string> const &Extensions)
{
    std::unordered_map<std::string, std::string> OptionsMap {{"None", ""}};
    try
    {
        for (auto const &Entry: std::filesystem::recursive_directory_iterator(Root))
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
}("Resources/Assets", {".gltf", ".glb"});

constexpr auto OptionNone = "None";
static std::string s_SelectedItem = OptionNone;
static std::string s_ModelPath = s_OptionsMap.at(s_SelectedItem);

AppScenePanel::AppScenePanel(Control *const Parent, AppWindow *const Window) : Control(Parent), m_Window(Window)
{
}

void AppScenePanel::Paint()
{
    CreateInfoPanel();
}

void AppScenePanel::CreateInfoPanel() const
{
    if (m_Window && ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto const &Objects = m_Window->GetRenderer().GetObjects();

        if (std::empty(Objects))
        {
            s_SelectedItem = OptionNone;
        }

        if (ImGui::BeginCombo("glTF Scene", std::data(s_SelectedItem)))
        {
            bool SelectionChanged = false;

            for (auto const &[Name, Path]: s_OptionsMap)
            {
                bool const bIsSelected = s_SelectedItem == Name;
                if (ImGui::Selectable(std::data(Name), bIsSelected))
                {
                    s_SelectedItem = Name;
                    s_ModelPath = Path;
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
                m_Window->GetRenderer().UnloadAllScenes();
                if (!std::empty(s_ModelPath))
                {
                    [[maybe_unused]] auto const _ = m_Window->GetRenderer().LoadScene(s_ModelPath);
                }
            }
        }

        ImGui::Text("Loaded Objects: %d", m_Window->GetRenderer().GetNumObjects());
        ImGui::Text("Total Triangles: %d",
                    std::accumulate(std::begin(Objects),
                                    std::end(Objects),
                                    0U,
                                    [](std::uint32_t const Sum, auto const &Object)
                                    {
                                        return Sum + Object->GetTrianglesCount();
                                    }));

        if (!std::empty(Objects))
        {
            if (ImGui::Button("Reload Scene"))
            {
                m_Window->GetRenderer().UnloadAllScenes();
                [[maybe_unused]] auto const _ = m_Window->GetRenderer().LoadScene(s_ModelPath);
            }

            ImGui::SameLine();

            if (ImGui::Button("Destroy All"))
            {
                m_Window->GetRenderer().UnloadAllScenes();
            }
        }

        CreateObjectsList();
    }
}

void AppScenePanel::CreateObjectsList() const
{
    if (m_Window)
    {
        if (auto const &Objects = m_Window->GetRenderer().GetObjects();
            ImGui::CollapsingHeader(std::format("Loaded Objects: {} ", m_Window->GetRenderer().GetNumObjects()).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto const &Object: Objects)
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
}
