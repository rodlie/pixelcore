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

#include "pixelcoreutils.h"

#include <QFile>

#include <iostream>
#include <fstream>

#define ICC_HEADER_LENGTH 128

#ifndef ORD8
#define ORD8 unsigned char
#endif

PixelCoreUtils::PixelCoreUtils(QObject *parent)
    : QObject(parent)
{

}

unsigned int PixelCoreUtils::readUInt32Number(char *p) {
    unsigned int rv;
    rv = 16777216 * (unsigned int)((ORD8 *)p)[0]
            +    65536 * (unsigned int)((ORD8 *)p)[1]
            +      256 * (unsigned int)((ORD8 *)p)[2]
            +            (unsigned int)((ORD8 *)p)[3];
    return rv;
}

bool PixelCoreUtils::fileHasColorProfile(QString &filename)
{
    if (QFile::exists(filename)) {
        return extractEmbeddedColorProfile(filename);
    }
    return false;
}

bool PixelCoreUtils::extractEmbeddedColorProfile(QString &filename,
                                                 QString icc)
{
    std::ifstream file(filename.toStdString().c_str(),
                       std::ios::in|std::ios::binary|std::ios::ate);
    if (!file.is_open()) { return false; }
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
                std::cout << "found an ICC profile at offset " << offset << ", size is " << profileSize << std::endl;
            } else {
                std::cout << "found an ICC profile header, but unable to get the profile size." << std::endl;
            }
            delete[] sizeBuffer;
            offset += ICC_HEADER_LENGTH;
        }
    } while (found != 0);

    if (profileOffset > 0 && profileSize > ICC_HEADER_LENGTH) {
        if (icc.isEmpty()) { return true; }
        else {
            // save
        }
    }

    return false;
}
