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

#ifndef PIXELCOREUTILS_H
#define PIXELCOREUTILS_H

#include <QObject>
#include <QString>

class PixelCoreUtils : public QObject
{
    Q_OBJECT

public:

    explicit PixelCoreUtils(QObject *parent = nullptr);

    static unsigned int readUInt32Number(char *p);

    static bool fileHasColorProfile(QString &filename);
    static bool extractEmbeddedColorProfile(QString &filename,
                                            QString icc = QString());
};

#endif // PIXELCOREUTILS_H
