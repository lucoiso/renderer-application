// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

export module Application.Window.StatusPanel;

import Application.Window;
import RenderCore.Window.Control;

namespace Application
{
    export class StatusPanel final : public RenderCore::Control
    {
        AppWindow* m_Window {nullptr};

    public:
        StatusPanel(Control*, AppWindow*);

    protected:
        void Paint() override;
    };
}// namespace Application