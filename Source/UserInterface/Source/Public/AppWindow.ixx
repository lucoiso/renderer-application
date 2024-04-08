// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module UserInterface.Window;

import RenderCore.UserInterface.Window;

namespace UserInterface
{
    export class AppWindow final : public RenderCore::Window
    {
    public:
        AppWindow();

    protected:
        void PrePaint() override;
        void PostPaint() override;

    private:
        static void SetDockingLayout();
    };
} // namespace UserInterface
