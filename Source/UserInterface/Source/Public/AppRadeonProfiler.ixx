// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/renderer-application

export module UserInterface.RadeonProfiler;

import UserInterface.Window;
import RenderCore.UserInterface.Control;

namespace UserInterface
{
    export class AppRadeonProfiler final : public RenderCore::Control
    {
    public:
        explicit AppRadeonProfiler(Control *);
        ~AppRadeonProfiler() override;

    protected:
        void Paint() override;

        static void CreateSettingsPanel();
        static void UpdateIntervalTime();
        static void CreateInformationPanel();
    };
} // namespace UserInterface
