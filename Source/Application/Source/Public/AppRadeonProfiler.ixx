// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module Application.RadeonProfiler;

import Application.Window;
import RenderCore.Window.Control;

namespace Application
{
    export class AppRadeonProfiler final : public RenderCore::Control
    {
        AppWindow *m_Window {nullptr};

    public:
        AppRadeonProfiler(Control *, AppWindow *);
        ~AppRadeonProfiler() override;

    protected:
        void Paint() override;
    };
} // namespace Application
