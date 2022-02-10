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

#include <QApplication>
#include <QStringList>
#include <QDebug>

#include <iostream>

#include "pixelcorecms.h"
#include "pixelcoreconsole.h"
#include "pixelcorewindow.h"

int main(int argc, char *argv[])
{
    std::cout << QString("PixelCore %1 @ https://pixelcore.org").arg(PIXELCORE_VERSION).toStdString() << std::endl;
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(QString("PixelCore"));
    QCoreApplication::setOrganizationName(QString("PixelCore"));
    QCoreApplication::setOrganizationDomain(QString("pixelcore.org"));
    QCoreApplication::setApplicationVersion(QString(PIXELCORE_VERSION));

    QStringList args = a.arguments();

#ifdef PIXELCORE_DEBUG
    qDebug() << "arguments" << args;
    qDebug() << "RGB profiles" << PixelCoreCMS::getColorProfiles().size();
    qDebug() << "CMYK profiles" << PixelCoreCMS::getColorProfiles(cmsSigCmykData).size();
    qDebug() << "GRAY profiles" << PixelCoreCMS::getColorProfiles(cmsSigGrayData).size();
#endif

    if (!args.contains("-cmd")) {
        PixelCoreWindow w(nullptr, args);
        w.show();
        return a.exec();
    }

    PixelCoreConsole c(nullptr, args);
    return 0;
}
