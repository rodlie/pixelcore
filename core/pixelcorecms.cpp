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

#include "pixelcorecms.h"

#include <QtGlobal>
#include <QCoreApplication>

#include <QDir>
#include <QDirIterator>
#include <QMapIterator>

PixelCoreCMS::PixelCoreCMS(QObject *parent)
    : QObject(parent)
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    case QImage::Format_RGBA64:
    case QImage::Format_RGBX64:
        return TYPE_RGBA_16;
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    case QImage::Format_BGR888:
        return TYPE_BGR_8;
#endif
    default:
        return 0;
    }
}

QImage PixelCoreCMS::colorManageRGB(QImage &image,
                                    std::vector<cmsHPROFILE> profiles,
                                    int intent,
                                    cmsUInt32Number flags)
{
    if (image.isNull()) { return QImage(); }
    cmsUInt32Number format = toLcmsFormat(image.format());
    if (format == 0) { return QImage(); }
    QImage result(image.width(), image.height(), image.format());
    cmsHTRANSFORM xform = cmsCreateMultiprofileTransform(profiles.data(),
                                                         profiles.size(),
                                                         format,
                                                         format,
                                                         intent,
                                                         flags);
    if (xform == nullptr) { return QImage(); }
    cmsDoTransformLineStride(xform,
                             image.constBits(),
                             result.bits(),
                             image.width(),
                             image.height(),
                             image.bytesPerLine(),
                             result.bytesPerLine(),
                             0,
                             0);
    cmsDeleteTransform(xform);
    return result;
}

QStringList PixelCoreCMS::getColorProfilesPath()
{
    QStringList folders;
    folders <<  QString("%1/WINDOWS/System32/spool/drivers/color").arg(QDir::rootPath());
    folders << "/Library/ColorSync/Profiles";
    folders << QString("%1/Library/ColorSync/Profiles").arg(QDir::homePath());
    folders << "/usr/share/color/icc";
    folders << "/usr/local/share/color/icc";
    folders << QString("%1/.color/icc").arg(QDir::homePath());
    folders << QString("%1/../share/color/icc").arg(qApp->applicationDirPath());
    folders << QString("%1/profiles").arg(qApp->applicationDirPath());
    folders << QString("%1/../Resources/Profiles").arg(qApp->applicationDirPath());
    folders << QString("%1/../share/pixelcore/profiles").arg(qApp->applicationDirPath());
    return folders;
}

QMap<QString, QString> PixelCoreCMS::getColorProfiles(cmsColorSpaceSignature colorspace)
{
    QMap<QString, QString> profiles;
    QStringList folders = getColorProfilesPath();

    for (int i = 0; i < folders.size(); ++i) {
        QDirIterator it(folders.at(i),
                        QStringList() << "*.icc" << "*.icm",
                        QDir::Files,
                        QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString iccFile = it.next();
            QString profile = getProfileTag(iccFile);
            if (iccFile.isEmpty() || profile.isEmpty()) { continue; }
            if (getProfileColorSpace(iccFile) != colorspace) { continue; }
            profiles[profile] = iccFile;
        }
    }

    return profiles;
}

QMap<QString, QString> PixelCoreCMS::getAllColorProfiles()
{
    QMap<QString, QString> profiles;

    QMapIterator<QString, QString> rgb(getColorProfiles(cmsSigRgbData));
    while (rgb.hasNext()) {
        rgb.next();
        profiles[rgb.key()] = rgb.value();
    }

    QMapIterator<QString, QString> cmyk(getColorProfiles(cmsSigCmykData));
    while (cmyk.hasNext()) {
        cmyk.next();
        profiles[cmyk.key()] = cmyk.value();
    }

    QMapIterator<QString, QString> gray(getColorProfiles(cmsSigGrayData));
    while (gray.hasNext()) {
        gray.next();
        profiles[gray.key()] = gray.value();
    }

    return profiles;
}

cmsColorSpaceSignature PixelCoreCMS::getProfileColorSpace(cmsHPROFILE profile)
{
    cmsColorSpaceSignature result = cmsGetColorSpace(profile);
    cmsCloseProfile(profile);
    return result;
}

cmsColorSpaceSignature PixelCoreCMS::getProfileColorSpace(const QString &filename)
{
    return getProfileColorSpace(cmsOpenProfileFromFile(filename.toStdString().c_str(), "r"));
}

cmsColorSpaceSignature PixelCoreCMS::getProfileColorSpace(std::vector<unsigned char> &buffer)
{
    return getProfileColorSpace(cmsOpenProfileFromMem(buffer.data(),
                                                      static_cast<cmsUInt32Number>(buffer.size())));
}

cmsColorSpaceSignature PixelCoreCMS::getProfileColorSpace(QByteArray buffer)
{
    return getProfileColorSpace(cmsOpenProfileFromMem(buffer.data(),
                                                      static_cast<cmsUInt32Number>(buffer.size())));
}

const QString PixelCoreCMS::getProfileTag(cmsHPROFILE profile,
                                          cmsInfoType tag)
{
    std::string result;
    if (profile) {
        cmsUInt32Number size = 0;
        size = cmsGetProfileInfoASCII(profile,
                                      tag,
                                      "en",
                                      "US",
                                      nullptr,
                                      0);
        if (size > 0) {
            std::vector<char> buffer(size);
            cmsUInt32Number newsize = cmsGetProfileInfoASCII(profile,
                                                             tag,
                                                             "en",
                                                             "US",
                                                             &buffer[0], size);
            if (size == newsize) { result = buffer.data(); }
        }
    }
    cmsCloseProfile(profile);
    return QString::fromStdString(result);
}

const QString PixelCoreCMS::getProfileTag(const QString &filename,
                                          cmsInfoType tag)
{
    if (QFile::exists(filename)) {
        return getProfileTag(cmsOpenProfileFromFile(filename.toStdString().c_str(), "r"),
                             tag);
    }
    return QString();
}

const QString PixelCoreCMS::getProfileTag(std::vector<unsigned char> &buffer,
                                          cmsInfoType tag)
{
    return getProfileTag(cmsOpenProfileFromMem(buffer.data(),
                                               static_cast<cmsUInt32Number>(buffer.size())),
                         tag);
}

const QString PixelCoreCMS::getProfileTag(QByteArray buffer,
                                          cmsInfoType tag)
{
    return getProfileTag(cmsOpenProfileFromMem(buffer.data(),
                                               static_cast<cmsUInt32Number>(buffer.size())),
                         tag);
}

bool PixelCoreCMS::isValidColorProfile(const QString &filename)
{
    if (QFile::exists(filename) && !getProfileTag(filename).isEmpty()) { return true; }
    return false;
}

bool PixelCoreCMS::isValidColorProfile(QByteArray profile)
{
    if (profile.size() > 0 && !getProfileTag(profile).isEmpty()) { return true; }
    return false;
}

