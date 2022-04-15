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
        AppOverlay() {
                std::string jsonStr = R"(
                {
                    "strings": [
                        {
                            "key": "PluginName",
                            "value": "sys-clk"
                        },
                        {
                            "key": "SysclkIpcNotRunningFatalGuiText",
                            "value": "sys-clk is not running.\n\n\nMake sure sys-clk is installed and enabled correctly."
                        },
                        {
                            "key": "SysclkIpcInitFailedFatalGuiText",
                            "value": "Can't connect to sys-clk.\n\n\nMake sure sys-clk is installed and enabled correctly."
                        },
                        {
                            "key": "SysclkIpcVersionMismatchFatalGuiText",
                            "value": "Overlay is incompatible with the running sys-clk version.\n\n\nMake sure everything is installed and up to date."
                        },
                        {
                            "key": "DefaultFreqFarmatListText",
                            "value": "No change"
                        },
                        {
                            "key": "AppIdMismatchFatalGuiText",
                            "value": "Application changed while editing,\n\n\nthe running application has been changed."
                        },
                        {
                            "key": "AppIDBaseMenuGuiText",
                            "value": "App ID:"
                        },
                        {
                            "key": "ProfileBaseMenuGuiText",
                            "value": "Profile:"
                        },
                        {
                            "key": "CPUBaseMenuGuiText",
                            "value": "CPU:"
                        },
                        {
                            "key": "GPUBaseMenuGuiText",
                            "value": "GPU:"
                        },
                        {
                            "key": "MemBaseMenuGuiText",
                            "value": "MEM:"
                        },
                        {
                            "key": "ChipBaseMenuGuiText",
                            "value": "Chip:"
                        },
                        {
                            "key": "PCBBaseMenuGuiText",
                            "value": "PCB:"
                        },
                        {
                            "key": "SkinBaseMenuGuiText",
                            "value": "Skin:"
                        },
                        {
                            "key": "FatalErrorFatalGuiText",
                            "value": "Fatal error"
                        },
                        {
                            "key": "ToggleListItemMainGuiText",
                            "value": "Enabled"
                        },
                        {
                            "key": "SysclkIpcSetEnabledFailedFatalGuiText",
                            "value": "sysclkIpcSetEnabled"
                        },
                        {
                            "key": "AppProfileListItemMainGuiText",
                            "value": "Edit profile"
                        },
                        {
                            "key": "AdvanceProfileCategoryHeaderMainGuiText",
                            "value": "Advance profile"
                        },
                        {
                            "key": "TeamOverrideListItemMainGuiText",
                            "value": "Temp override"
                        }
                    ]
                }
            )";
            std::string lanPath = std::string("sdmc:/switch/.overlays/lang/") + APPTITLE + "/";
            fsdevMountSdmc();
            tsl::hlp::doWithSmSession([&lanPath, &jsonStr]{
                tsl::tr::InitTrans(lanPath, jsonStr);
            });
            fsdevUnmountDevice("sdmc");
        }
        ~AppOverlay() {}

        virtual void initServices() override { }

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
