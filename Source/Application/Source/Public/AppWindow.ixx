// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module Application.Window;

import RenderCore.Window;

namespace Application
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
} // namespace Application
