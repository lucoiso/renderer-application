// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

export module Application.Window.ScenePanel;

import Application.Window;
import RenderCore.Window.Control;

namespace Application
{
    export class ScenePanel final : public RenderCore::Control
    {
        AppWindow* m_Window {nullptr};

    public:
        ScenePanel(Control*, AppWindow*);

    protected:
        void Paint() override;

    private:
        void CreateInfoPanel() const;
        void CreateObjectsList() const;
    };
}// namespace Application