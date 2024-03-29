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

#ifndef PIXELCORE_CONSOLE_H
#define PIXELCORE_CONSOLE_H

#include <QObject>
#include <QStringList>
#include <QByteArray>

namespace PixelCore {
    class Console : public QObject
    {
        Q_OBJECT

    public:

        explicit Console(QObject *parent = nullptr,
                         QStringList args = QStringList());

    private:

        QStringList _args;

        bool showProfiles();
        bool checkImageProfile();
        bool extractEmbeddedProfile();
        bool saveProfile(const QString &filename,
                         QByteArray profile);
        void showHelp();
    };
}

#endif // PIXELCORE_CONSOLE_H
