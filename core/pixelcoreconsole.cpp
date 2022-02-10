/*
#
# PixelCore
#
# https://pixelcore.org
# https://github.com/rodlie/pixelcore
#
# Copyright (c) 2021, 2022 Ole-André Rodlie. All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#
*/

#include "pixelcoreconsole.h"
#include "pixelcorecms.h"

#include <QMapIterator>

#include <iostream>

PixelCoreConsole::PixelCoreConsole(QObject *parent,
                                   QStringList args)
    : QObject(parent)
    , _args(args)
{
    bool unknownArgs = true;

    if (args.contains("--icc")) { unknownArgs = showProfiles(); }

    if (args.contains("--help") || args.contains("-h") || unknownArgs) {
        showHelp();
    }
}

bool PixelCoreConsole::showProfiles()
{
    if (_args.contains("--icc")) {
        QMap<QString, QString> profiles;
        if (_args.contains("rgb")) {
            profiles = PixelCoreCMS::getColorProfiles();
        } else if (_args.contains("cmyk")) {
            profiles = PixelCoreCMS::getColorProfiles(cmsSigCmykData);
        } else if (_args.contains("gray")) {
            profiles = PixelCoreCMS::getColorProfiles(cmsSigGrayData);
        } else {
            profiles = PixelCoreCMS::getAllColorProfiles();
        }
        if (profiles.size() > 0) {
            std::cout << std::endl;
            QMapIterator<QString, QString> icc(profiles);
            while (icc.hasNext()) {
                icc.next();
                std::cout << QString(icc.key()).toStdString() << " => " << QString(icc.value()).toStdString() << std::endl;
            }
            std::cout << std::endl;
        } else {
            std::cout << std::endl << "No ICC color profiles found!" << std::endl << std::endl;
        }
    } else { return true; }
    return false;
}

void PixelCoreConsole::showHelp()
{
    std::cout << std::endl << "Usage" << std::endl << std::endl;
    std::cout << "-c, --cmd                      Enable console mode" << std::endl;
    std::cout << "-h, --help                     Display this help and exit" << std::endl;
    std::cout << "-v, --version                  Output version information and exit" << std::endl;
    std::cout << std::endl;
    std::cout << "--icc [rgb/cmyk/gray]          List available ICC color profiles and exit" << std::endl;
    std::cout << std::endl;
}
