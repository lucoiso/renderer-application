// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

export module Application.Window;

import RenderCore.Window;

namespace Application
{
    export class AppWindow : public RenderCore::Window
    {
    protected:
        virtual void CreateOverlay() override;
    };
}// namespace Application