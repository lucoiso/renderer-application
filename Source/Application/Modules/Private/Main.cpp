// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

import Application.Window;

int main([[maybe_unused]] int Argc, [[maybe_unused]] char* Argv[])
{
#ifndef _DEBUG
    boost::log::core::get()->set_filter(boost::log::trivial::severity != boost::log::trivial::debug);
#endif

    // boost::log::add_file_log("renderer.log");

    if (Application::AppWindow Window;
        Window.Initialize(600U, 600U, "Vulkan Renderer: Main Window", RenderCore::InitializationFlags::NONE))
    {
        while (Window.IsOpen())
        {
            Window.PollEvents();
        }

        return 0U;
    }

    return 1U;
}
