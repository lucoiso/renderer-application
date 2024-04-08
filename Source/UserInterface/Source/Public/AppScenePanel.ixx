// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module UserInterface.ScenePanel;

import UserInterface.Window;
import RenderCore.UserInterface.Control;

namespace UserInterface
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
} // namespace UserInterface
