// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <vector>
#include <Volk/volk.h>

export module Application.Viewport;

import Application.Window;
import RenderCore.Window.Control;

namespace Application
{
    export class AppViewport final : public RenderCore::Control
    {
        std::vector<VkDescriptorSet> m_ViewportDescriptorSets {};
        AppWindow *m_Window {nullptr};
        bool m_Open {false};

    public:
        AppViewport(Control *, AppWindow *);
        ~AppViewport() override;

    protected:
        void Refresh() override;

        void PrePaint() override;

        void Paint() override;

        void PostPaint() override;
    };
} // namespace Application
