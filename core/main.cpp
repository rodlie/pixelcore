/*
#
# PIXELCORE
#
# https://pixelcore.org
# https://github.com/rodlie/pixelcore
#
# Copyright (c) Ole-André Rodlie. All rights reserved.
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
# The copyright holders of PIXELCORE hereby grant permission for
# non-GPL compatible plug-ins and add-ons to be used and distributed
# together with PIXELCORE, provided that you also meet the terms and conditions
# of the licenses of those plug-ins and add-ons.
#
*/

#include <QApplication>
#include <QStringList>
#include <QDebug>

#include <iostream>

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

    if (args.contains("-v") || args.contains("--version")) {
        return 0;
    }

    // enable gui in the future, for now console-only
    /*bool gui = (!args.contains("--cmd") &&
                !args.contains("-c") &&
                !args.contains("--help") &&
                !args.contains("-h") &&
                !args.contains("--icc"));

    if (gui) {
        PixelCoreWindow w(nullptr, args);
        w.show();
        return a.exec();
    }*/

    PixelCoreConsole c(nullptr, args);
    return 0;
}
