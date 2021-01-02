/*
    sys-clk manager, a sys-clk frontend homebrew
    Copyright (C) 2019  natinusala
    Copyright (C) 2019  p-sam
    Copyright (C) 2019  m4xw

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <borealis.hpp>

#include "main_frame.h"
#include "logo.h"

#include "ipc/client.h"
#include "ipc/ipc.h"

int main(int argc, char* argv[])
{
    // Init the app
    if (!brls::Application::init(APP_TITLE))
    {
        brls::Logger::error("无法初始化Borealis应用程序");
        return EXIT_FAILURE;
    }

    // Setup verbose logging on PC
#ifndef __SWITCH__
    brls::Logger::setLogLevel(brls::LogLevel::DEBUG);
#endif

    if (brls::Application::loadFont(LOGO_FONT_NAME, LOGO_FONT_PATH) < 0)
    {
        brls::Logger::error("无法加载logo字体");
    }

    uint32_t apiVersion;

    // Check that sys-clk is running
    if (!sysclkIpcRunning())
    {
        brls::Logger::error("sys-clk未运行");
        brls::Application::crash("sys-clk似乎没有运行，请检查它是否正确安装和启用。");
    }
    // Initialize sys-clk IPC client
    else if (R_FAILED(sysclkIpcInitialize()) || R_FAILED(sysclkIpcGetAPIVersion(&apiVersion)))
    {
        brls::Logger::error("无法初始化sys-clk IPC客户端");
        brls::Application::crash("无法连接到sys-clk，请检查它是否已正确安装和启用。");
    }
    else if (SYSCLK_IPC_API_VERSION != apiVersion) {
        brls::Logger::error("sys-clk IPC API版本不匹配（期望：%u；实际：%u)", SYSCLK_IPC_API_VERSION, apiVersion);
        brls::Application::crash("管理器与当前运行的sys-clk系统模块不兼容，请检查是否已正确安装最新版本（重新启动？）。");
    }
    else
    {
        // Set version string
        char version[0x100] = {0};
        Result rc = sysclkIpcGetVersionString(version, sizeof(version));
        if (R_SUCCEEDED(rc))
        {
            brls::Application::setCommonFooter(std::string(version));
        }
        else
        {
            brls::Logger::error("无法获取sys-clk版本字符串");
            brls::Application::setCommonFooter("[unknown]");
        }

        // Initialize services with a PC shim
        nsInitialize();

        // Create root view
        MainFrame *mainFrame = new MainFrame();

        // Add the root view to the stack
        brls::Application::pushView(mainFrame);
    }

    // Run the app
    while (brls::Application::mainLoop());

    // Exit
    nsExit();
    sysclkIpcExit();
    return EXIT_SUCCESS;
}
