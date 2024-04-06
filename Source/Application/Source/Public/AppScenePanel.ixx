// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module Application.ScenePanel;

import Application.Window;
import RenderCore.UserInterface.Control;

namespace Application
{
    export class AppScenePanel final : public RenderCore::Control
    {
    public:
        AppScenePanel(Control *);

    protected:
        void Paint() override;

    private:
        void CreateInfoPanel() const;
        void CreateIlluminationPanel() const;
        void CreateObjectsList() const;
    };
} // namespace Application
