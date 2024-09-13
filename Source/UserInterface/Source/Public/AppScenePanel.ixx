// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include "UserInterfaceModule.hpp"

export module UserInterface.ScenePanel;

export import luGUI.UserInterface.Control;
import RenderCore.Types.Object;

namespace UserInterface
{
    export class USERINTERFACEMODULE_API AppScenePanel : public luGUI::Control
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
