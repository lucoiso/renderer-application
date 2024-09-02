// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/keywords/format.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <easy/profiler.h>

#if defined(_WIN32) && !defined(_DEBUG)
    #include <Windows.h>
#endif

module UserInterface.Library;

import UserInterface.Window;

import RenderCore.UserInterface.Window;
import RenderCore.UserInterface.Window.Flags;
import RenderCore.Utils.EnumHelpers;
import RadeonManager.Manager;

using namespace UserInterface;

void SetupBoostLog()
{
    EASY_FUNCTION(profiler::colors::Yellow);

    auto const FormatTimeStamp = boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");
    auto const FormatThreadId  = boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
    auto const FormatSeverity  = boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity");

    boost::log::formatter const LogFormatter = boost::log::expressions::format("[%1%] (%2%) [%3%] %4%") % FormatTimeStamp % FormatThreadId %
                                               FormatSeverity % boost::log::expressions::smessage;

    #ifndef _DEBUG
    boost::log::core::get()->set_filter(boost::log::trivial::severity != boost::log::trivial::debug);
    #endif

    #ifdef _WIN32
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    #endif
    auto const ConsoleSink = boost::log::add_console_log(std::clog);
    ConsoleSink->set_formatter(LogFormatter);

    auto const LogFileSink = boost::log::add_file_log("renderer-application.log");
    LogFileSink->set_formatter(LogFormatter);

    boost::log::add_common_attributes();
}

std::int32_t UserInterface::Execute()
{
    SetupBoostLog();

    std::int32_t Output{EXIT_FAILURE};

    auto const IsRadeonManagerActive = RadeonManager::Start();

    if (UserInterface::AppWindow Window;
        Window.Initialize(1280U,
                          600U,
                          "Renderer Application: Vulkan",
                          RenderCore::InitializationFlags::ENABLE_IMGUI | RenderCore::InitializationFlags::ENABLE_DOCKING |
                                  RenderCore::InitializationFlags::ENABLE_VIEWPORTS))
    {
        while (Window.IsOpen())
        {
            Window.PollEvents();
        }

        Window.Shutdown();
        Output = EXIT_SUCCESS;
    }

    if (IsRadeonManagerActive)
    {
        RadeonManager::Stop();
    }

    if (profiler::isListening())
    {
        profiler::dumpBlocksToFile("renderer-application.prof");

        profiler::stopListen();
        EASY_PROFILER_DISABLE
    }

    return Output;
}
