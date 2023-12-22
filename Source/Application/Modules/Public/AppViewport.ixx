// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <volk.h>

export module Application.Viewport;

import Application.Window;
import RenderCore.Window.Control;

namespace Application
{
    export class AppViewport final : public RenderCore::Control
    {
        VkDescriptorSet m_ViewportDescriptorSet {VK_NULL_HANDLE};
        AppWindow* m_Window {nullptr};

    public:
        AppViewport(Control*, AppWindow*);

    protected:
        void Refresh() override;
        void PrePaint() override;
        void Paint() override;
        void PostPaint() override;
    };
}// namespace Application