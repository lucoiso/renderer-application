// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module Application.ScenePanel;

import Application.Window;
import RenderCore.Window.Control;

namespace Application
{
    export class AppScenePanel final : public RenderCore::Control
    {
        AppWindow *m_Window {nullptr};

    public:
        AppScenePanel(Control *, AppWindow *);

    protected:
        void Paint() override;

    private:
        void CreateInfoPanel() const;
        void CreateObjectsList() const;
    };
} // namespace Application
