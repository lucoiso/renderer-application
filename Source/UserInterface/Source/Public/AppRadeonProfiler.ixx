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
        AppRadeonProfiler(Control *);
        ~AppRadeonProfiler() override;

    protected:
        void Paint() override;
    };
} // namespace UserInterface
