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

#include "pixelcoreconsole.h"
#include "cms.h"

#include <QMapIterator>
#include <QFile>
#include <QDebug>

#include <iostream>

using namespace PixelCore;

PixelCoreConsole::PixelCoreConsole(QObject *parent,
                                   QStringList args)
    : QObject(parent)
    , _args(args)
{
    if (args.contains("--help") || args.contains("-h")) {
        showHelp();
        return;
    }

    bool status = true;

    if (args.contains("--icc") && args.contains("list")) { status = showProfiles(); }
    else if (args.contains("--icc") && args.contains("check")) { status = checkImageProfile(); }
    else if (args.contains("--icc") && args.contains("extract")) { status = extractEmbeddedProfile(); }

    if (status) { showHelp(); }
}

bool PixelCoreConsole::showProfiles()
{
    if (_args.contains("--icc") && _args.contains("list")) {
        QMap<QString, QString> profiles;
        if (_args.contains("rgb")) {
            profiles = CMS::getColorProfiles();
        } else if (_args.contains("cmyk")) {
            profiles = CMS::getColorProfiles(cmsSigCmykData);
        } else if (_args.contains("gray")) {
            profiles = CMS::getColorProfiles(cmsSigGrayData);
        } else {
            profiles = CMS::getAllColorProfiles();
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

bool PixelCoreConsole::checkImageProfile()
{
    QString filename = _args.at(_args.count()-1);
    if (_args.contains("--icc") && _args.contains("check") && QFile::exists(filename)) {
        if (CMS::hasColorProfile(filename)) {
            return false;
        }
    }
    return true;
}

bool PixelCoreConsole::extractEmbeddedProfile()
{
    if (_args.contains("--icc") &&
        _args.contains("extract") &&
        _args.count() == 5)
    {
        QString input = _args.at(3);
        QString output = _args.at(4);
        if (input.isEmpty() ||
            output.isEmpty() ||
            !QFile::exists(input)) { return true; }
        if (!output.endsWith(".icc")) { output.append(".icc"); }
        QByteArray profile = CMS::getEmbeddedColorProfile(input);
        if (!CMS::isValidColorProfile(profile)) { return true; }
        if (saveProfile(output, profile)) { return false; }
    }
    return true;
}

bool PixelCoreConsole::saveProfile(const QString &filename,
                                   QByteArray profile)
{
    bool saved = false;
    if (!filename.isEmpty() &&
        profile.size() > 0 &&
        CMS::isValidColorProfile(profile))
    {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly)) {
            if (file.write(profile) > -1) { saved = true; }
            file.close();
        }
    }
    return saved;
}

void PixelCoreConsole::showHelp()
{
    std::cout << QString("PixelCore %1 @ https://pixelcore.org").arg(PIXELCORE_VERSION).toStdString() << std::endl;
    std::cout << std::endl << "Usage" << std::endl << std::endl;
    std::cout << "-h, --help                         Display this help" << std::endl;
    std::cout << std::endl;
    std::cout << "--icc list [rgb/cmyk/gray]         List available ICC color profiles" << std::endl;
    std::cout << "--icc check [image]                Check if file has an embedded ICC color profile" << std::endl;
    std::cout << "--icc extract [input] [output]     Extract embedded ICC color profile from image" << std::endl;
    std::cout << std::endl;
}
