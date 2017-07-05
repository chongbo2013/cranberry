////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt5 framework.
// Copyright (C) 2017 Nicolas Kogler
//
// Cranberry is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Cranberry is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Cranberry. If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////


// Cranberry headers
#include <Cranberry/Graphics/GifAnimation.hpp>
#include <Cranberry/System/Debug.hpp>

// Qt headers
#include <QFileInfo>
#include <QImageReader>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - File %2 does not exist.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Frame %2 could not be read.")


bool GifAnimation::create(const QString& path, Window* renderTarget)
{
    QVector<QImage> frames;
    QVector<qreal> durations;

    // Attempts to load the file.
    QFileInfo info(path);
    if (!info.exists())
    {
        return cranError(ERRARG_1(e_01, path));
    }

    // Reads each image and their durations.
    QImageReader reader(path);
    QImage outputImage;
    qint32 imgCount = reader.imageCount();

    for (qint32 i = 0; i < imgCount; i++)
    {
        if (!reader.read(&outputImage))
        {
            return cranError(ERRARG_1(e_02, QString::number(i)));
        }

        frames.append(outputImage);
        durations.append(reader.nextImageDelay());
    }

    return createInternal(frames, durations, renderTarget);
}
