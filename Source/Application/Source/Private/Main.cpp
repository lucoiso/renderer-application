// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/keywords/format.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#if defined(_WIN32) && !defined(_DEBUG)
#include <Windows.h>
#endif

import Application.Window;
import RadeonManager.Manager;

int main([[maybe_unused]] int const Argc, [[maybe_unused]] char *const Argv[ ])
{
    auto const FormatTimeStamp = boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");
    auto const FormatThreadId = boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
    auto const FormatSeverity = boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity");

    boost::log::formatter const LogFormatter = boost::log::expressions::format("[%1%] (%2%) [%3%] %4%") % FormatTimeStamp % FormatThreadId
                                               % FormatSeverity % boost::log::expressions::smessage;

#ifndef _DEBUG
    boost::log::core::get()->set_filter(boost::log::trivial::severity != boost::log::trivial::debug);

#ifdef _WIN32
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
#else
    auto const ConsoleSink = boost::log::add_console_log(std::clog);
    ConsoleSink->set_formatter(LogFormatter);
#endif

    auto const LogFileSink = boost::log::add_file_log("renderer-application.log");
    LogFileSink->set_formatter(LogFormatter);

    boost::log::add_common_attributes();

    std::int32_t Output {EXIT_FAILURE};

    if (Application::AppWindow Window; Window.Initialize(800U, 600U, "Vulkan Renderer: Main Window", RenderCore::InitializationFlags::NONE))
    {
        while (Window.IsOpen())
        {
            Window.PollEvents();
        }

        Output = EXIT_SUCCESS;
    }

    RadeonManager::Stop();
    return Output;
}
