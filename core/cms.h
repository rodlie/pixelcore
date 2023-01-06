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

#ifndef PIXELCORE_CMS_H
#define PIXELCORE_CMS_H

#include <QObject>
#include <QImage>
#include <QStringList>
#include <QByteArray>
#include <QMap>

#include <vector>

#define CMS_NO_REGISTER_KEYWORD
#include <lcms2.h>

namespace PIXELCORE {
    class CMS : public QObject
    {
        Q_OBJECT

    public:

        explicit CMS(QObject *parent = nullptr);

        static cmsUInt32Number toLcmsFormat(QImage::Format format);

        static QImage colorManageRGB(QImage &image,
                                     std::vector<cmsHPROFILE> profiles,
                                     int intent,
                                     cmsUInt32Number flags);

        static QStringList getColorProfilesPath();
        static QMap<QString, QString> getColorProfiles(cmsColorSpaceSignature colorspace = cmsSigRgbData);
        static QMap<QString, QString> getAllColorProfiles();

        static cmsColorSpaceSignature getProfileColorSpace(cmsHPROFILE profile);
        static cmsColorSpaceSignature getProfileColorSpace(const QString &filename);
        static cmsColorSpaceSignature getProfileColorSpace(std::vector<unsigned char> &buffer);
        static cmsColorSpaceSignature getProfileColorSpace(QByteArray buffer);

        static const QString getProfileTag(cmsHPROFILE profile,
                                           cmsInfoType tag = cmsInfoDescription);
        static const QString getProfileTag(const QString &filename,
                                           cmsInfoType tag = cmsInfoDescription);
        static const QString getProfileTag(std::vector<unsigned char> &buffer,
                                           cmsInfoType tag = cmsInfoDescription);
        static const QString getProfileTag(QByteArray buffer,
                                           cmsInfoType tag = cmsInfoDescription);

        static bool isValidColorProfile(const QString  &filename);
        static bool isValidColorProfile(QByteArray profile);
    };
}

#endif // PIXELCORE_CMS_H
