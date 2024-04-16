// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <vector>
#include <Volk/volk.h>

export module UserInterface.Viewport;

import UserInterface.Window;
import RenderCore.UserInterface.Control;

namespace UserInterface
{
    export class AppViewport final : public RenderCore::Control
    {
        std::vector<VkDescriptorSet> m_ViewportDescriptorSets {};
        bool                         m_Open { false };

    public:
        AppViewport(Control *);
        ~AppViewport() override;

    protected:
        void Refresh() override;

        void PrePaint() override;

        void Paint() override;

        void PostPaint() override;
    };
} // namespace UserInterface
