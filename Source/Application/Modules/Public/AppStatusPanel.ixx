// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

export module Application.StatusPanel;

import Application.Window;
import RenderCore.Window.Control;

namespace Application
{
    export class AppStatusPanel final : public RenderCore::Control
    {
        AppWindow* m_Window {nullptr};

    public:
        AppStatusPanel(Control*, AppWindow*);

    protected:
        void Paint() override;
    };
}// namespace Application