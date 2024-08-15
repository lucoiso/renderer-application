// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

export module UserInterface.Window;

import RenderCore.UserInterface.Window;
import RenderCore.UserInterface.Control;

namespace UserInterface
{
    export class AppWindow final : public RenderCore::Window
    {
    public:
        AppWindow();

    protected:
        void PrePaint() override;
        void OnInitialize() override;

    private:
        static void SetDockingLayout();
        static void SetStyle();
    };
} // namespace UserInterface
