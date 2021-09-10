/*
    sys-clk manager, a sys-clk frontend homebrew
    Copyright (C) 2019-2020  natinusala
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

#include "about_tab.h"

#include "logo.h"
#include "ipc/ipc.h"
#include "ipc/client.h"

#include <borealis.hpp>

AboutTab::AboutTab()
{
    this->addView(new Logo(LogoStyle::ABOUT));

    // Subtitle
    brls::Label *subTitle = new brls::Label(
        brls::LabelStyle::REGULAR,
        "任天堂SWITCH超频、降频系统模块和前端应用程序由RetroNX团队制作", 
        true
    );
    subTitle->setHorizontalAlign(NVG_ALIGN_CENTER);
    this->addView(subTitle);

    // Copyright
    brls::Label *copyright = new brls::Label(
        brls::LabelStyle::DESCRIPTION,
        "根据beerware许可证许可的系统模块\n" \
        "根据GPL-3.0许可的前端应用程序\n" \
        "\u00A9 2019 - 2020 natinusala, p-sam, m4xw",
        true
    );
    copyright->setHorizontalAlign(NVG_ALIGN_CENTER);
    this->addView(copyright);

    // Links
    this->addView(new brls::Header("链接和资源"));
    brls::Label *links = new brls::Label(
        brls::LabelStyle::SMALL,
        "\uE016  用户指南和代码源代码可以在我们的GitHub存储库中找到\n" \
        "\uE016  sys-clk管理器由Borealis提供支持，Borealis是一个硬件加速的UI库\n" \
        "\uE016  加入RetroNX Discord服务器以获得支持、请求功能或只是闲逛！",
        true
    );
    this->addView(links);
}
