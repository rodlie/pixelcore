/*
#
# PixelCore
#
# https://pixelcore.org
# https://github.com/rodlie/pixelcore
#
# Copyright (c) 2021 Ole-André Rodlie. All rights reserved.
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

#ifndef PIXELCORECMS_H
#define PIXELCORECMS_H

#include <QObject>
#include <QImage>
#include <QVector>

#include <lcms2.h>

class PixelCoreCMS : public QObject
{
    Q_OBJECT
public:
    explicit PixelCoreCMS(QObject *parent = nullptr);
    static cmsUInt32Number toLcmsFormat(QImage::Format format);
    static QImage colorManageImage(QImage &image,
                                   QVector<cmsHPROFILE> profiles,
                                   int intent,
                                   cmsUInt32Number flags);
};

#endif // PIXELCORECMS_H