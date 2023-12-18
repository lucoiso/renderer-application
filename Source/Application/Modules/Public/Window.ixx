// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <memory>
#include <vector>

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
    };
}// namespace Application