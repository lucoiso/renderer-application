// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <memory>

export module UserInterface.ScenePanel;

import UserInterface.Window;
import RenderCore.UserInterface.Control;
import RenderCore.Types.Object;

namespace UserInterface
{
    export class AppScenePanel final : public RenderCore::Control
    {
    public:
        explicit AppScenePanel(Control *);

    protected:
        void Paint() override;

    private:
        static void CreateInfoPanel();
        static void CreateIlluminationPanel();
        void        CreateObjectsList() const;
        static void CreateObjectItem(std::shared_ptr<RenderCore::Object> const &);
    };
} // namespace UserInterface
