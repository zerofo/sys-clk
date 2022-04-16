/*
 * --------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <p-sam@d3vs.net>, <natinusala@gmail.com>, <m4x@m4xw.net>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If you meet any of us some day, and you think this
 * stuff is worth it, you can buy us a beer in return.  - The sys-clk authors
 * --------------------------------------------------------------------------
 */

#include "app_profile_gui.h"

#include "../format.h"
#include "fatal_gui.h"

AppProfileGui::AppProfileGui(std::uint64_t applicationId, SysClkTitleProfileList* profileList)
{
    InitTrans();
    this->applicationId = applicationId;
    this->profileList = profileList;
}

AppProfileGui::~AppProfileGui()
{
    delete this->profileList;
}

void AppProfileGui::openFreqChoiceGui(tsl::elm::ListItem* listItem, SysClkProfile profile, SysClkModule module, std::uint32_t* hzList)
{
    tsl::changeTo<FreqChoiceGui>(this->profileList->mhzMap[profile][module] * 1000000, hzList, [this, listItem, profile, module](std::uint32_t hz) {
        this->profileList->mhzMap[profile][module] = hz / 1000000;
        listItem->setValue(formatListFreqMhz(this->profileList->mhzMap[profile][module]));
        Result rc = sysclkIpcSetProfiles(this->applicationId, this->profileList);
        if(R_FAILED(rc))
        {
            FatalGui::openWithResultCode("SysclkIpcSetProfilesFailedFatalGuiText"_tr, rc);
            return false;
        }

        return true;
    });
}

void AppProfileGui::addModuleListItem(SysClkProfile profile, SysClkModule module, std::uint32_t* hzList)
{
    std::string moduleFormat{" "};
    bool pretty = true;
    switch(module)
    {
        case SysClkModule_CPU:
            moduleFormat = pretty ? "CPUPrettySysclkFormatModuleText"_tr : "CPUSysclkFormatModuleText"_tr;
            break;
        case SysClkModule_GPU:
            moduleFormat = pretty ? "GPUPrettySysclkFormatModuleText"_tr : "GPUSysclkFormatModuleText"_tr;
            break;
        case SysClkModule_MEM:
            moduleFormat = pretty ? "MEMPrettySysclkFormatModuleText"_tr : "MEMSysclkFormatModuleText"_tr;
            break;
        default:
            break;
    }
    tsl::elm::ListItem* listItem = new tsl::elm::ListItem(moduleFormat);
    listItem->setValue(formatListFreqMhz(this->profileList->mhzMap[profile][module]));
    listItem->setClickListener([this, listItem, profile, module, hzList](u64 keys) {
        if((keys & HidNpadButton_A) == HidNpadButton_A)
        {
            this->openFreqChoiceGui(listItem, profile, module, hzList);
            return true;
        }

        return false;
    });

    this->listElement->addItem(listItem);
}

void AppProfileGui::addProfileUI(SysClkProfile profile)
{
    std::string profileFormat{" "};
    bool pretty = true;
    switch(profile)
    {
        case SysClkProfile_Docked:
            profileFormat = pretty ? "DockedPrettySysclkFormatProfileText"_tr : "DockedSysclkFormatProfileText"_tr;
        case SysClkProfile_Handheld:
            profileFormat = pretty ? "HandheldPrettySysclkFormatProfileText"_tr : "HandheldSysclkFormatProfileText"_tr;
        case SysClkProfile_HandheldCharging:
            profileFormat = pretty ? "HandheldChargingPrettySysclkFormatProfileText"_tr : "HandheldChargingSysclkFormatProfileText"_tr;
        case SysClkProfile_HandheldChargingUSB:
            profileFormat = pretty ? "HandheldChargingUSBPrettySysclkFormatProfileText"_tr : "HandheldChargingUSBSysclkFormatProfileText"_tr;
        case SysClkProfile_HandheldChargingOfficial:
            profileFormat = pretty ? "HandheldChargingOfficialPrettySysclkFormatProfileText"_tr : "HandheldChargingOfficialSysclkFormatProfileText"_tr;
        default:
            break;
    }
    this->listElement->addItem(new tsl::elm::CategoryHeader(profileFormat));
    this->addModuleListItem(profile, SysClkModule_CPU, &sysclk_g_freq_table_cpu_hz[0]);
    this->addModuleListItem(profile, SysClkModule_GPU, &sysclk_g_freq_table_gpu_hz[0]);
    this->addModuleListItem(profile, SysClkModule_MEM, &sysclk_g_freq_table_mem_hz[0]);
}

void AppProfileGui::listUI()
{
    this->addProfileUI(SysClkProfile_Docked);
    this->addProfileUI(SysClkProfile_Handheld);
    this->addProfileUI(SysClkProfile_HandheldCharging);
    this->addProfileUI(SysClkProfile_HandheldChargingOfficial);
    this->addProfileUI(SysClkProfile_HandheldChargingUSB);
}

void AppProfileGui::changeTo(std::uint64_t applicationId)
{
    SysClkTitleProfileList* profileList = new SysClkTitleProfileList;
    Result rc = sysclkIpcGetProfiles(applicationId, profileList);
    if(R_FAILED(rc))
    {
        delete profileList;
        FatalGui::openWithResultCode("SysclkIpcGetProfilesFailedFatalGuiText"_tr, rc);
        return;
    }

    tsl::changeTo<AppProfileGui>(applicationId, profileList);
}

void AppProfileGui::update()
{
    BaseMenuGui::update();

    if(this->context && this->applicationId != this->context->applicationId)
    {
        tsl::changeTo<FatalGui>(
            "AppIdMismatchFatalGuiText"_tr,
            ""
        );
    }
}
