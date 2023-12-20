// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/renderer-application

module;

#include <volk.h>

export module Application.Window;

import RenderCore.Window;

namespace Application
{
    export class AppWindow final : public RenderCore::Window
    {
        VkDescriptorSet m_ViewportDescriptorSet {VK_NULL_HANDLE};

    public:
        AppWindow();

    protected:
        void OnInitialized() override;
        void Refresh() override;
        void PrePaint() override;
        void PostPaint() override;

    private:
        static void SetDockingLayout();
    };
}// namespace Application