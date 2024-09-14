// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include "UserInterfaceModule.hpp"

export module UserInterface.Window;

import luGUI.UserInterface.Controls.Window;

namespace UserInterface
{
    export class USERINTERFACEMODULE_API AppWindow : public luGUI::Window
    {
    public:
        AppWindow();

    protected:
        void PrePaint() override;
        void OnInitialize() override;

    private:
        static void SetDockingLayout();
        static void SetStyle();
    };
} // namespace UserInterface
