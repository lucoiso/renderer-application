// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module UserInterface.StatusPanel;

import UserInterface.Window;
import RenderCore.UserInterface.Control;

namespace UserInterface
{
    export class AppStatusPanel final : public RenderCore::Control
    {
    public:
        explicit AppStatusPanel(Control *);

    protected:
        void Paint() override;

        static void CreateStatusPanel();
        static void CreateRendererPanel();
        static void CreateCameraPanel();
    };
} // namespace UserInterface
