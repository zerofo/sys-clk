/*
 * --------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <p-sam@d3vs.net>, <natinusala@gmail.com>, <m4x@m4xw.net>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If you meet any of us some day, and you think this
 * stuff is worth it, you can buy us a beer in return.  - The sys-clk authors
 * --------------------------------------------------------------------------
 */

#pragma once

#include <tesla.hpp>

#include "../style.h"

static inline void InitTrans()
{
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
                    "key": "SysclkIpcSetOverrideFailedFatalGuiText",
                    "value": "sysclkIpcSetOverride"
                },
                {
                    "key": "SysclkIpcSetProfilesFailedFatalGuiText",
                    "value": "sysclkIpcSetProfiles"
                },
                {
                    "key": "SysclkIpcGetProfilesFailedFatalGuiText",
                    "value": "sysclkIpcGetProfiles"
                },
                {
                    "key": "SysclkIpcGetCurrentContextFailedFatalGuiText",
                    "value": "sysclkIpcGetCurrentContext"
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

class BaseGui : public tsl::Gui
{
    public:
        BaseGui() {}
        ~BaseGui() {}
         virtual void preDraw(tsl::gfx::Renderer* renderer);
        void update() override;
        tsl::elm::Element* createUI() override;
        virtual tsl::elm::Element* baseUI() = 0;
        virtual void refresh() {}
};
