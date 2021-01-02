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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    SysClkProfile_Handheld = 0,
    SysClkProfile_HandheldCharging,
    SysClkProfile_HandheldChargingUSB,
    SysClkProfile_HandheldChargingOfficial,
    SysClkProfile_Docked,
    SysClkProfile_EnumMax
} SysClkProfile;

typedef enum
{
    SysClkModule_CPU = 0,
    SysClkModule_GPU,
    SysClkModule_MEM,
    SysClkModule_EnumMax
} SysClkModule;

typedef enum
{
    SysClkThermalSensor_SOC = 0,
    SysClkThermalSensor_PCB,
    SysClkThermalSensor_Skin,
    SysClkThermalSensor_EnumMax
} SysClkThermalSensor;

typedef struct
{
    uint8_t enabled;
    uint64_t applicationId;
    SysClkProfile profile;
    uint32_t freqs[SysClkModule_EnumMax];
    uint32_t overrideFreqs[SysClkModule_EnumMax];
    uint32_t temps[SysClkThermalSensor_EnumMax];
} SysClkContext;

typedef struct
{
    union {
        uint32_t mhz[SysClkProfile_EnumMax * SysClkModule_EnumMax];
        uint32_t mhzMap[SysClkProfile_EnumMax][SysClkModule_EnumMax];
    };
} SysClkTitleProfileList;

//#define SYSCLK_GPU_HANDHELD_MAX_HZ 460800000
//#define SYSCLK_GPU_UNOFFICIAL_CHARGER_MAX_HZ 768000000
#define SYSCLK_GPU_HANDHELD_MAX_HZ 768000000
#define SYSCLK_GPU_UNOFFICIAL_CHARGER_MAX_HZ 921000000

extern uint32_t sysclk_g_freq_table_mem_hz[];
extern uint32_t sysclk_g_freq_table_cpu_hz[];
extern uint32_t sysclk_g_freq_table_gpu_hz[];

#define SYSCLK_ENUM_VALID(n, v) ((v) < n##_EnumMax)

static inline const char* sysclkFormatModule(SysClkModule module, bool pretty)
{
    switch(module)
    {
        case SysClkModule_CPU:
            return pretty ? "CPU" : "中央处理器";
        case SysClkModule_GPU:
            return pretty ? "GPU" : "图形处理器";
        case SysClkModule_MEM:
            return pretty ? "内存" : "内存控制器";
        default:
            return NULL;
    }
}

static inline const char* sysclkFormatThermalSensor(SysClkThermalSensor thermSensor, bool pretty)
{
    switch(thermSensor)
    {
        case SysClkThermalSensor_SOC:
            return pretty ? "芯片" : "SOC";
        case SysClkThermalSensor_PCB:
            return pretty ? "主板" : "PCB";
        case SysClkThermalSensor_Skin:
            return pretty ? "表面" : "机器表面";
        default:
            return NULL;
    }
}

static inline const char* sysclkFormatProfile(SysClkProfile profile, bool pretty)
{
    switch(profile)
    {
        case SysClkProfile_Docked:
            return pretty ? "底座模式" : "底座模式";
        case SysClkProfile_Handheld:
            return pretty ? "手持模式" : "手持模式";
        case SysClkProfile_HandheldCharging:
            return pretty ? "手持充电模式" : "手持充电模式";
        case SysClkProfile_HandheldChargingUSB:
            return pretty ? "USB充电手持模式" : "USB充电手持充电模式";
        case SysClkProfile_HandheldChargingOfficial:
            return pretty ? "官方充电手持模式" : "官方充电器手持充电模式";
        default:
            return NULL;
    }
}
