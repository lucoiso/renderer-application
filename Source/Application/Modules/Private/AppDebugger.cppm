// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <filesystem>
#include <format>
#include <imgui.h>
#include <numeric>
#include <source_location>
#include <string>
#include <vector>

module Application.Debugger;

import RuntimeInfo.Manager;
import RenderCore.Management.AllocationRegister;

using namespace Application;

std::vector<std::string> g_Logs {};

AppDebugger::AppDebugger(Control *const Parent, AppWindow *const Window) : Control(Parent), m_Window(Window)
{
}

void AppDebugger::Paint()
{
    if (ImGui::Begin("Debugging"))
    {
        CreateCallstackPanel();
        CreateAllocationPanel();
    }
    ImGui::End();
}

void AppDebugger::CreateCallstackPanel()
{
    if (ImGui::CollapsingHeader("Callstack", ImGuiTreeNodeFlags_DefaultOpen))
    {
        std::vector<std::source_location> const &Callstack = RuntimeInfo::Manager::Get().GetCallstack();

        if (auto CallstackLimit = static_cast<int>(RuntimeInfo::Manager::Get().GetCallstackLimit()); ImGui::InputInt("Limit", &CallstackLimit))
        {
            RuntimeInfo::Manager::Get().SetCallstackLimit(std::clamp<std::uint8_t>(static_cast<std::uint8_t>(CallstackLimit), 0U, UINT8_MAX));
        }

        bool Active = RuntimeInfo::Manager::Get().IsActive();
        ImGui::Checkbox("Active", &Active);
        RuntimeInfo::Manager::Get().SetActive(Active);

        if (ImGui::BeginListBox("Callstack"))
        {
            for (auto const &Location: Callstack)
            {
                std::string const FunctionID
                        = std::format("{}:{}:{}", std::filesystem::path {Location.file_name()}.filename().string(), Location.line(), Location.column());
                ImGui::Selectable(std::data(FunctionID));
            }
            ImGui::EndListBox();
        }
    }
}
void AppDebugger::CreateAllocationPanel()
{
    if (ImGui::CollapsingHeader("GPU Allocations", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto const &AllocationManager = RenderCore::AllocationRegister::Get();
        auto const &Register = AllocationManager.GetRegister();

        auto const AllocationCount = static_cast<std::int32_t>(std::size(Register));
        ImGui::Text("Allocation Count: %d", AllocationCount);

        auto const AllocationTotal = std::accumulate(std::cbegin(Register),
                                                     std::cend(Register),
                                                     0,
                                                     [](std::int32_t const Sum, auto const &Allocation)
                                                     {
                                                         return Sum + static_cast<std::int32_t>(Allocation.AllocationSize);
                                                     });
        ImGui::Text("Total Allocation: %d bytes", AllocationTotal);

        if (ImGui::BeginListBox("Allocations"))
        {
            for (auto const &Allocations = Register; const auto &[MemoryType, Memory, AllocationSize, UserData]: Allocations)
            {
                std::ostringstream MemoryAddressStream {};
                MemoryAddressStream << std::hex << Memory;

                std::string const AllocationID = std::format("Address: {}\nSize: {} bytes\nType: {}", MemoryAddressStream.str(), AllocationSize, MemoryType);

                ImGui::Separator();
                ImGui::Selectable(std::data(AllocationID));
                ImGui::Separator();
            }

            ImGui::EndListBox();
        }
    }
}
