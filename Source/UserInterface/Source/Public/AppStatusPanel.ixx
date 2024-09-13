// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include "UserInterfaceModule.hpp"

export module UserInterface.StatusPanel;

export import luGUI.UserInterface.Control;

namespace UserInterface
{
    export class USERINTERFACEMODULE_API AppStatusPanel : public luGUI::Control
    {
    public:
        explicit AppStatusPanel(Control *);

    protected:
        void Paint() override;

        static void CreateRendererPanel();
        static void CreateCameraPanel();
    };
} // namespace UserInterface
