/*
 * --------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <p-sam@d3vs.net>, <natinusala@gmail.com>, <m4x@m4xw.net>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If you meet any of us some day, and you think this
 * stuff is worth it, you can buy us a beer in return.  - The sys-clk authors
 * --------------------------------------------------------------------------
 */

#define TESLA_INIT_IMPL
#include <tesla.hpp>

#include "ui/gui/fatal_gui.h"
#include "ui/gui/main_gui.h"

using namespace tsl;

class AppOverlay : public tsl::Overlay
{
    public:
        AppOverlay() {}
        ~AppOverlay() {}

        virtual void initServices() override {
            std::string lanPath = std::string("sdmc:/switch/.overlays/lang/") + APPTITLE + "/";
            tsl::tr::InitTrans(lanPath);
        }

        virtual void exitServices() override {
            sysclkIpcExit();
        }

        virtual std::unique_ptr<tsl::Gui> loadInitialGui() override
        {
            uint32_t apiVersion;
            smInitialize();

            tsl::hlp::ScopeGuard smGuard([] { smExit(); });

            if(!sysclkIpcRunning())
            {
                return initially<FatalGui>(
                    "SysclkIpcNotRunningFatalGuiText"_tr,
                    ""
                );
            }

            if(R_FAILED(sysclkIpcInitialize()) || R_FAILED(sysclkIpcGetAPIVersion(&apiVersion)))
            {
                return initially<FatalGui>(
                    "SysclkIpcInitFailedFatalGuiText"_tr,
                    ""
                );
            }

            if(SYSCLK_IPC_API_VERSION != apiVersion)
            {
                return initially<FatalGui>(
                    "SysclkIpcVersionMismatchFatalGuiText"_tr,
                    ""
                );
            }

            return initially<MainGui>();
        }
};

int main(int argc, char **argv)
{
    return tsl::loop<AppOverlay>(argc, argv);
}
