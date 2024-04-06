// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module Application.StatusPanel;

import Application.Window;
import RenderCore.UserInterface.Control;

namespace Application
{
    export class AppStatusPanel final : public RenderCore::Control
    {
    public:
        AppStatusPanel(Control *);

    protected:
        void Paint() override;

        void CreateStatusPanel() const;
    };
} // namespace Application
