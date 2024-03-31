// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module Application.Debugger;

import Application.Window;
import RenderCore.UserInterface.Control;

namespace Application
{
    export class AppDebugger final : public RenderCore::Control
    {
        AppWindow *m_Window {nullptr};

    public:
        AppDebugger(Control *, AppWindow *);

    protected:
        void Paint() override;

#ifdef _DEBUG
        static void CreateCallstackPanel();
#endif
        static void CreateAllocationPanel();
    };
} // namespace Application
