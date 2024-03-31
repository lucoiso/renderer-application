// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <Volk/volk.h>
#include <vector>

export module Application.Viewport;

import Application.Window;
import RenderCore.UserInterface.Control;

namespace Application
{
    export class AppViewport final : public RenderCore::Control
    {
        std::vector<VkDescriptorSet> m_ViewportDescriptorSets {};
        AppWindow                   *m_Window {nullptr};
        bool                         m_Open {false};

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
