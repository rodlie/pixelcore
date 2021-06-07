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

#include "pixelcorecms.h"

#include <QtGlobal>

PixelCoreCMS::PixelCoreCMS(QObject *parent) : QObject(parent)
{

}

cmsUInt32Number PixelCoreCMS::toLcmsFormat(QImage::Format format)
{
    switch (format) {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
        return TYPE_BGRA_8;
    case QImage::Format_RGB888:
        return TYPE_RGB_8;
    case QImage::Format_RGBX8888:
    case QImage::Format_RGBA8888:
        return TYPE_RGBA_8;
    case QImage::Format_Grayscale8:
        return TYPE_GRAY_8;
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
    case QImage::Format_Grayscale16:
        return TYPE_GRAY_16;
#endif
    case QImage::Format_RGBA64:
    case QImage::Format_RGBX64:
        return TYPE_RGBA_16;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    case QImage::Format_BGR888:
        return TYPE_BGR_8;
#endif
    default:
        return 0;
    }
}
