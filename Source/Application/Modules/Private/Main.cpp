// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#if defined(_WIN32) && defined(_DEBUG)
#include <Windows.h>
#endif

import Application.Window;
import RadeonManager.Manager;

int main([[maybe_unused]] int const Argc, [[maybe_unused]] char *const Argv[])
{
#ifndef _DEBUG
    boost::log::core::get()->set_filter(boost::log::trivial::severity != boost::log::trivial::debug);

#ifdef _WIN32
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
#else
    boost::log::add_console_log(std::clog, boost::log::keywords::format = "[%TimeStamp%]: %Message%");
#endif

    boost::log::add_file_log("renderer-application.log", boost::log::keywords::format = "[%TimeStamp%]: %Message%");
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
