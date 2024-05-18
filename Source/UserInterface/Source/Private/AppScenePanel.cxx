// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <filesystem>
#include <imgui.h>
#include <numeric>
#include <unordered_map>

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

void AppScenePanel::CreateInfoPanel()
{
    if (ImGui::CollapsingHeader("Current Scene", ImGuiTreeNodeFlags_DefaultOpen) && ImGui::IsItemVisible())
    {
        auto const &Objects = RenderCore::Renderer::GetObjects();

        if (ImGui::BeginCombo("glTF Scene", std::data(s_SelectedItem)))
        {
            for (auto const &[Name, Path] : s_OptionsMap)
            {
                bool const bIsSelected = s_SelectedItem == Name;
                if (ImGui::Selectable(std::data(Name), bIsSelected))
                {
                    s_SelectedItem = Name;
                    s_ModelPath    = Path;
                }

                if (bIsSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        static bool s_AutoUnload = true;
        if (ImGui::Button("Load Scene") && !std::empty(s_ModelPath))
        {
            if (s_AutoUnload)
            {
                RenderCore::Renderer::RequestClearScene();
            }

            RenderCore::Renderer::RequestLoadObject(s_ModelPath);
        }

        ImGui::SameLine();

        ImGui::Checkbox("Auto Unload", &s_AutoUnload);

        static std::uint32_t NumObjects     = 0U;
        static std::uint32_t TotalTriangles = 0U;

        if (std::uint32_t const NewNumObjects = RenderCore::Renderer::GetNumObjects();
            NumObjects != NewNumObjects)
        {
            NumObjects     = NewNumObjects;
            TotalTriangles = std::reduce(std::cbegin(Objects),
                                         std::cend(Objects),
                                         0U,
                                         [](std::uint32_t const Accumulator, auto const &Object)
                                         {
                                             return Accumulator + Object->GetMesh()->GetNumTriangles();
                                         });
        }

        ImGui::Text(std::data(std::format("Objects: {}\nTriangles: {}", NumObjects, TotalTriangles)));

        if (!std::empty(Objects))
        {
            if (ImGui::Button("Reload"))
            {
                RenderCore::Renderer::RequestClearScene();
                RenderCore::Renderer::RequestLoadObject(s_ModelPath);
            }

            ImGui::SameLine();

            if (ImGui::Button("Unload"))
            {
                RenderCore::Renderer::RequestClearScene();
            }
        }
    }
}

void AppScenePanel::CreateIlluminationPanel()
{
    if (ImGui::CollapsingHeader("Illumination", ImGuiTreeNodeFlags_DefaultOpen) && ImGui::IsItemVisible())
    {
        RenderCore::Illumination &IlluminationConfig = RenderCore::Renderer::GetMutableIllumination();

        static float LightPosition[3] = {
                IlluminationConfig.GetPosition().x,
                IlluminationConfig.GetPosition().y,
                IlluminationConfig.GetPosition().z
        };
        if (ImGui::SliderFloat3("Light Position", &LightPosition[0], -180.F, 180.F, "%.2f") && ImGui::IsItemVisible())
        {
            IlluminationConfig.SetPosition({ LightPosition[0], LightPosition[1], LightPosition[2] });
        }

        static float LightColor[3] = { IlluminationConfig.GetColor().x, IlluminationConfig.GetColor().y, IlluminationConfig.GetColor().z };
        if (ImGui::SliderFloat3("Light Color", &LightColor[0], 0.F, 1.F, "%.2f") && ImGui::IsItemVisible())
        {
            IlluminationConfig.SetColor({
                                                std::clamp(LightColor[0], 0.F, 1.F),
                                                std::clamp(LightColor[1], 0.F, 1.F),
                                                std::clamp(LightColor[2], 0.F, 1.F)
                                        });
        }

        static float LightIntensity = IlluminationConfig.GetIntensity();
        if (ImGui::SliderFloat("Light Intensity", &LightIntensity, 0.1F, 10.F) && ImGui::IsItemVisible())
        {
            IlluminationConfig.SetIntensity(std::clamp(LightIntensity, 0.F, FLT_MAX));
        }

        static float AmbientLight = IlluminationConfig.GetAmbient();
        if (ImGui::SliderFloat("Ambient Light", &AmbientLight, 0.1F, 10.F) && ImGui::IsItemVisible())
        {
            IlluminationConfig.SetAmbient(std::clamp(AmbientLight, 0.F, FLT_MAX));
        }
    }
}

void AppScenePanel::CreateObjectsList() const
{
    if (auto const &Objects = RenderCore::Renderer::GetObjects();
        ImGui::CollapsingHeader(std::data(std::format("Loaded Objects: {} ", RenderCore::Renderer::GetNumObjects())), ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (auto const &Object : Objects)
        {
            CreateObjectItem(Object);
        }
    }
}

void AppScenePanel::CreateObjectItem(std::shared_ptr<RenderCore::Object> const &Object)
{
    if (ImGui::CollapsingHeader(std::data(std::format("[{}] {}", Object->GetID(), Object->GetName())), ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text(std::data(std::format("ID: {}\nName: {}\nPath: {}\nTriangles Count: {}",
                                          Object->GetID(),
                                          Object->GetName(),
                                          Object->GetPath(),
                                          Object->GetMesh()->GetNumTriangles())));

        ImGui::Separator();

        float Position[3] = { Object->GetPosition().x, Object->GetPosition().y, Object->GetPosition().z };
        if (ImGui::SliderFloat3(std::data(std::format("[{}] {} Position", Object->GetID(), Object->GetName())), &Position[0], -250.F, 250.F, "%.2f")
            && ImGui::IsItemVisible())
        {
            Object->SetPosition({ Position[0], Position[1], Position[2] });
        }

        float Scale[3] = { Object->GetScale().x, Object->GetScale().y, Object->GetScale().z };
        if (ImGui::SliderFloat3(std::data(std::format("[{}] {} Scale", Object->GetID(), Object->GetName())), &Scale[0], 0.001F, 100.F, "%.2f") &&
            ImGui::IsItemVisible())
        {
            Object->SetScale({ Scale[0], Scale[1], Scale[2] });
        }

        float Rotation[3] = { Object->GetRotation().x, Object->GetRotation().y, Object->GetRotation().z };
        if (ImGui::SliderFloat3(std::data(std::format("[{}] {} Rotation", Object->GetID(), Object->GetName())), &Rotation[0], -180.F, 180.F, "%.2f") &&
            ImGui::IsItemVisible())
        {
            Object->SetRotation({ Rotation[0], Rotation[1], Rotation[2] });
        }

        if (ImGui::Button("Destroy"))
        {
            Object->Destroy();
        }
    }
}
