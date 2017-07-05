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
#include <Cranberry/Graphics/CranAnimation.hpp>
#include <Cranberry/System/Debug.hpp>

// Qt headers
#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


CRANBERRY_USING_NAMESPACE


CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - File %2 does not exist.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Invalid version: %2.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Frame %2 could not be read.")


bool CranAnimation::create(const QString& path, Window* renderTarget)
{
    QVector<QImage> frames;
    QVector<qreal> durations;

    // Attempts to load the file.
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
    {
        return cranError(ERRARG_1(e_01, path));
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    file.close();

    // Reads each image and their durations.
    QJsonObject top = json.object();
    QJsonValue version = top.value("version");

    int v = version.toInt();
    if (version.isNull() || v <= 0 || v >= 2)
    {
        return cranError(ERRARG_1(e_02, QString::number(v)));
    }
    else if (v == 1)
    {
        QJsonArray array = top.value("frames").toArray();
        qint32 currentFrame = 0;

        foreach (QJsonValue value, array)
        {
            QJsonObject obj = value.toObject();
            QJsonValue path = obj.value("image");
            QJsonValue duration = obj.value("duration");
            QImage img(path.toString());

            if (path.isNull() || duration.isNull() || img.isNull())
            {
                return cranError(ERRARG_1(e_03, QString::number(currentFrame)));
            }

            frames.append(img);
            durations.append(duration.toInt());
            currentFrame++;
        }
    }

    return createInternal(frames, durations, renderTarget);
}
