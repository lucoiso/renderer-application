// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <algorithm>
#include <chrono>
#include <imgui.h>

module Application.RadeonProfiler;

import RadeonManager.Manager;
import RadeonManager.Profiler;

using namespace Application;

RadeonProfiler::ProfileData g_ProfileData{};

AppRadeonProfiler::AppRadeonProfiler(Control *const Parent, AppWindow *const Window)
    : Control(Parent)
    , m_Window(Window)
{
    [[maybe_unused]] auto const _ = RadeonManager::Start();
}

AppRadeonProfiler::~AppRadeonProfiler()
{
    RadeonManager::Stop();
}

void AppRadeonProfiler::Paint()
{
    if (ImGui::Begin("Radeon Profiler"))
    {
        if (ImGui::Button(RadeonProfiler::IsRunning() ? "Stop" : "Start"))
        {
            if (RadeonProfiler::IsRunning() || !RadeonProfiler::Start())
            {
                RadeonProfiler::Stop();
            }
        }

        ImGui::SameLine();

        static float UpdateInterval{1.F};
        ImGui::InputFloat("Interval (seconds)", &UpdateInterval, 0.1F, 1.F, "%.2f");
        UpdateInterval = std::clamp(UpdateInterval, 0.1F, 10.F);

        ImGui::Separator();

        static auto BeginTime = std::chrono::high_resolution_clock::now();
        auto const CurrentTime = std::chrono::high_resolution_clock::now();

        if (auto const DurationSec = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BeginTime);
            RadeonProfiler::IsRunning() && static_cast<float>(DurationSec.count()) * 0.001f >= UpdateInterval)
        {
            BeginTime = CurrentTime;
            g_ProfileData = RadeonProfiler::GetProfileData();
        }

        if (ImGui::CollapsingHeader("System Metrics", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("System Time: %lld", g_ProfileData.TimeStamp);
            ImGui::Text("CPU Usage: %.2f%%", g_ProfileData.CPU.Usage);
            ImGui::Text("CPU RAM: %dMB", g_ProfileData.CPU.RAM);
            ImGui::Text("CPU SmartShift: %d", g_ProfileData.CPU.SmartShift);
        }

        if (ImGui::CollapsingHeader("GPU Metrics", ImGuiTreeNodeFlags_DefaultOpen))
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
        if (ImGui::CollapsingHeader("FPS Metrics", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("FPS Time: %lld", g_ProfileData.TimeStamp);
            ImGui::Text("FPS: %d", g_ProfileData.FPS.FPS);
        }
    }
    ImGui::End();
}
