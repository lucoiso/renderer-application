// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

export module Application.Debugger;

import Application.Window;
import RenderCore.Window.Control;

namespace Application
{
    export class AppDebugger final : public RenderCore::Control
    {
        AppWindow *m_Window {nullptr};

    public:
        AppDebugger(Control *, AppWindow *);

    protected:
        void Paint() override;

        static void CreateCallstackPanel();
        static void CreateAllocationPanel();
    };
} // namespace Application
