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

#include "cms.h"

#include <QtGlobal>
#include <QCoreApplication>

#include <QDir>
#include <QDirIterator>
#include <QMapIterator>

#include <QFile>
#include <QDebug>

#include <iostream>
#include <fstream>

#define ICC_HEADER_LENGTH 128

#ifndef ORD8
#define ORD8 unsigned char
#endif

using namespace PixelCore;

cmsUInt32Number
CMS::toLcmsFormat(const QImage::Format &format)
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

unsigned int
CMS::readUInt32Number(char *p)
{
    unsigned int rv;
    rv = 16777216 * (unsigned int)((ORD8 *)p)[0]
            +    65536 * (unsigned int)((ORD8 *)p)[1]
            +      256 * (unsigned int)((ORD8 *)p)[2]
            +            (unsigned int)((ORD8 *)p)[3];
    return rv;
}

const QImage
CMS::manageImageLCMS(const QImage &image,
                     std::vector<cmsHPROFILE> &profiles,
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

const QStringList
CMS::getColorProfilesPath()
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

const QMap<QString, QString>
CMS::getColorProfiles(cmsColorSpaceSignature colorspace,
                      bool all)
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
            if (!all && getProfileColorSpace(iccFile) != colorspace) { continue; }
            profiles[profile] = iccFile;
        }
    }

    return profiles;
}

const QMap<QString, QString>
CMS::getAllColorProfiles()
{
    return getColorProfiles(cmsSigRgbData, true);
}

cmsColorSpaceSignature
CMS::getProfileColorSpace(cmsHPROFILE profile)
{
    cmsColorSpaceSignature result = cmsGetColorSpace(profile);
    cmsCloseProfile(profile);
    return result;
}

cmsColorSpaceSignature
CMS::getProfileColorSpace(const QString &filename)
{
    return getProfileColorSpace(cmsOpenProfileFromFile(filename.toStdString().c_str(), "r"));
}

cmsColorSpaceSignature
CMS::getProfileColorSpace(const std::vector<unsigned char> &buffer)
{
    return getProfileColorSpace(cmsOpenProfileFromMem(buffer.data(),
                                                      static_cast<cmsUInt32Number>(buffer.size())));
}

cmsColorSpaceSignature
CMS::getProfileColorSpace(const QByteArray &buffer)
{
    return getProfileColorSpace(cmsOpenProfileFromMem(buffer.data(),
                                                      static_cast<cmsUInt32Number>(buffer.size())));
}

const QString
CMS::getProfileTag(cmsHPROFILE profile,
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

const QString
CMS::getProfileTag(const QString &filename,
                   cmsInfoType tag)
{
    if (QFile::exists(filename)) {
        return getProfileTag(cmsOpenProfileFromFile(filename.toStdString().c_str(), "r"),
                             tag);
    }
    return QString();
}

const QString
CMS::getProfileTag(const std::vector<unsigned char> &buffer,
                   cmsInfoType tag)
{
    return getProfileTag(cmsOpenProfileFromMem(buffer.data(),
                                               static_cast<cmsUInt32Number>(buffer.size())),
                         tag);
}

const QString
CMS::getProfileTag(const QByteArray &buffer,
                   cmsInfoType tag)
{
    return getProfileTag(cmsOpenProfileFromMem(buffer.data(),
                                               static_cast<cmsUInt32Number>(buffer.size())),
                         tag);
}

const QByteArray
CMS::getEmbeddedColorProfile(const QString &filename)
{
    QByteArray result;
    std::ifstream file(filename.toStdString().c_str(),
                       std::ios::in|std::ios::binary|std::ios::ate);
    if (!file.is_open()) { return result; }
    unsigned int profileSize = 0;
    int profileOffset = 0;
    int offset = 0;
    int found;
    do {
        found = 0;
        int fc = 0;
        char c;
        file.seekg(offset, std::ios::beg);
        if (file.tellg() != offset) { break; }
        while(found == 0) {
            if (!file.read(&c,1)) { break; }
            offset++;
            switch(fc) {
            case 0:
                if (c == 'a') { fc++; } else { fc = 0; }
                break;
            case 1:
                if (c == 'c') { fc++; } else { fc = 0; }
                break;
            case 2:
                if (c == 's') { fc++; } else { fc = 0; }
                break;
            case 3:
                if (c == 'p') {
                    found = 1;
                    offset -= 40;
                } else { fc = 0; }
                break;
            }
        }
        if (found) {
            profileOffset = offset;
            file.seekg(offset, std::ios::beg);
            char *sizeBuffer = new char[4];
            if (file.read(sizeBuffer, 4)) {
                profileSize = readUInt32Number(sizeBuffer);
                qDebug() << "found an ICC profile at offset" << offset << profileSize;
            } else {
                qDebug() << "found an ICC profile header, but unable to get the profile size. Broken image/profile?";
            }
            delete[] sizeBuffer;
            offset += ICC_HEADER_LENGTH;
        }
    } while (found != 0);

    if (profileOffset > 0 && profileSize > ICC_HEADER_LENGTH) {
            file.clear();
            file.seekg(profileOffset, std::ios::beg);
            char *profileBuffer = new char[profileSize];
            if (file.read(profileBuffer, profileSize)) {
                result = QByteArray(profileBuffer, profileSize);
            }
            delete[] profileBuffer;
    }

    file.close();
    return result;
}

bool
CMS::hasColorProfile(const QString &filename)
{
    if ( QFile::exists(filename) ) {
        return (getEmbeddedColorProfile(filename).size() > 0);
    }
    return false;
}

bool
CMS::isValidColorProfile(const QString &filename)
{
    if (QFile::exists(filename) && !getProfileTag(filename).isEmpty()) { return true; }
    return false;
}

bool
CMS::isValidColorProfile(const QByteArray &profile)
{
    if (profile.size() > 0 && !getProfileTag(profile).isEmpty()) { return true; }
    return false;
}

