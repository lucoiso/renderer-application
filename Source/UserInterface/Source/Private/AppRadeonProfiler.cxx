// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <algorithm>
#include <chrono>
#include <imgui.h>

module UserInterface.RadeonProfiler;

import RadeonManager.Manager;
import RadeonManager.Profiler;

using namespace UserInterface;

RadeonProfiler::ProfileData g_ProfileData {};
float                       g_UpdateInterval { 1.F };

AppRadeonProfiler::AppRadeonProfiler(Control *const Parent)
    : Control(Parent)
{
}

AppRadeonProfiler::~AppRadeonProfiler()
{
    RadeonManager::Stop();
}

void AppRadeonProfiler::Paint()
{
    if (ImGui::Begin("Radeon Profiler"))
    {
        if (ImGui::Button(RadeonProfiler::IsRunning() ? "Stop" : "Start") && ImGui::IsItemVisible())
        {
            if (RadeonProfiler::IsRunning() || !RadeonProfiler::Start())
            {
                RadeonProfiler::Stop();
            }
        }

        if (RadeonProfiler::IsRunning())
        {
            ImGui::SameLine();
            CreateSettingsPanel();
            ImGui::Separator();

            UpdateIntervalTime();
            CreateInformationPanel();
        }
    }
    ImGui::End();
}

void AppRadeonProfiler::CreateSettingsPanel()
{
    if (ImGui::SliderFloat("Interval", &g_UpdateInterval, 0.1F, 10.F, "%.2f") && ImGui::IsItemVisible())
    {
        g_UpdateInterval = std::clamp(g_UpdateInterval, 0.1F, 10.F);
    }
}

void AppRadeonProfiler::UpdateIntervalTime()
{
    static auto LastTime    = std::chrono::high_resolution_clock::now();
    auto const  CurrentTime = std::chrono::high_resolution_clock::now();

    auto const     Milliseconds = std::chrono::duration<double, std::milli>(CurrentTime - LastTime).count();
    constexpr auto Denominator  = static_cast<double>(std::milli::den);

    if (auto const Seconds = Milliseconds / Denominator;
        Seconds >= g_UpdateInterval)
    {
        LastTime      = CurrentTime;
        g_ProfileData = RadeonProfiler::GetProfileData();
    }
}

void AppRadeonProfiler::CreateInformationPanel()
{
    if (ImGui::CollapsingHeader("System Metrics", ImGuiTreeNodeFlags_DefaultOpen) && ImGui::IsItemVisible())
    {
        ImGui::Text("System Time: %lld", g_ProfileData.TimeStamp);
        ImGui::Text("CPU Usage: %.2f%%", g_ProfileData.CPU.Usage);
        ImGui::Text("CPU RAM: %dMB", g_ProfileData.CPU.RAM);
        ImGui::Text("CPU SmartShift: %d", g_ProfileData.CPU.SmartShift);
    }

    if (ImGui::CollapsingHeader("GPU Metrics", ImGuiTreeNodeFlags_DefaultOpen) && ImGui::IsItemVisible())
    {
        ImGui::Text("GPU Time: %lld", g_ProfileData.TimeStamp);
        ImGui::Text("GPU Usage: %.2f%%", g_ProfileData.GPU.Usage);
        ImGui::Text("GPU Clock Speed: %dMHz", g_ProfileData.GPU.ClockSpeed);
        ImGui::Text("GPU VRAM Clock Speed: %dMHz", g_ProfileData.GPU.VRAMClockSpeed);
        ImGui::Text("GPU Temperature: %.2f°C", g_ProfileData.GPU.Temperature);
        ImGui::Text("GPU Hotspot Temperature: %.2f°C", g_ProfileData.GPU.HotspotTemperature);
        ImGui::Text("GPU Power: %.2fW", g_ProfileData.GPU.Power);
        ImGui::Text("GPU Total Board Power: %.2fW", g_ProfileData.GPU.TotalBoardPower);
        ImGui::Text("GPU Fan Speed: %d%%", g_ProfileData.GPU.FanSpeed);
        ImGui::Text("GPU VRAM: %dMB", g_ProfileData.GPU.VRAM);
        ImGui::Text("GPU Voltage: %dV", g_ProfileData.GPU.Voltage);
        ImGui::Text("GPU Intake Temperature: %.2f°C", g_ProfileData.GPU.IntakeTemperature);
    }

    if (ImGui::CollapsingHeader("FPS Metrics", ImGuiTreeNodeFlags_DefaultOpen) && ImGui::IsItemVisible())
    {
        ImGui::Text("FPS Time: %lld", g_ProfileData.TimeStamp);
        ImGui::Text("FPS: %d", g_ProfileData.FPS.FPS);
    }
}
