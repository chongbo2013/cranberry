////////////////////////////////////////////////////////////////////////////////
//
// Cranberry - C++ game engine based on the Qt framework.
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
#include <QApplication>
#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Constants
CRANBERRY_CONST_VAR(QString, e_01, "%0 [%1] - File %2 does not exist.")
CRANBERRY_CONST_VAR(QString, e_02, "%0 [%1] - Invalid version: %2.")
CRANBERRY_CONST_VAR(QString, e_03, "%0 [%1] - Frame %2 could not be read.")
CRANBERRY_CONST_VAR(QString, e_04, "%0 [%1] - Invalid spritesheet.")
CRANBERRY_CONST_VAR(QString, e_05, "%0 [%1] - Frame %2: Invalid rectangle.")


CRANBERRY_USING_NAMESPACE


bool CranAnimation::create(const QString& path, Window* renderTarget)
{
    QVector<QImage> frames;
    QVector<AnimationFrame> rects;
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
        auto array = top.value("frames").toArray();
        auto currentFrame = 0;

        // Determines whether there are spritesheets or not.
        auto sheets = top.value("spritesheets").toArray();
        if (!sheets.isEmpty())
        {
            frames.resize(sheets.count());

            // Loads all spritesheets.
            Q_FOREACH (QJsonValue value, sheets)
            {
                QJsonObject obj = value.toObject();
                QJsonValue index = obj.value("index");
                QJsonValue sheet = obj.value("sheet");
                QImage img(cranResourcePath(sheet.toString()));

                if (index.isNull() || sheet.isNull() || img.isNull() ||
                    index.toInt() < 0 || index.toInt() >= sheets.count())
                {
                    return cranError(ERRARG(e_04));
                }

                frames[index.toInt()] = img;
            }

            // Loads all frames.
            Q_FOREACH (QJsonValue value, array)
            {
                QJsonObject obj = value.toObject();
                QJsonValue index = obj.value("sheetindex");
                QJsonValue duration = obj.value("duration");
                QJsonObject rect = obj.value("rect").toObject();

                if (rect.isEmpty() || index.isNull() || duration.isNull())
                {
                    return cranError(ERRARG_1(e_03, QString::number(currentFrame)));
                }

                QJsonValue x = rect.value("x");
                QJsonValue y = rect.value("y");
                QJsonValue w = rect.value("w");
                QJsonValue h = rect.value("h");

                if (x.isNull() || y.isNull() || w.isNull() || h.isNull())
                {
                    return cranError(ERRARG_1(e_05, QString::number(currentFrame)));
                }

                AnimationFrame frame;
                frame.setRectangle(x.toDouble(), y.toDouble(), w.toDouble(), h.toDouble());
                frame.setDuration(duration.toDouble() / 1000.0);
                frame.setFrameId(currentFrame);
                frame.setAtlasId(index.toInt());

                rects.append(frame);
                currentFrame++;
            }

            return createInternal(frames, rects, renderTarget);
        }
        else
        {
            // Loads all frames.
            Q_FOREACH (QJsonValue value, array)
            {
                QJsonObject obj = value.toObject();
                QJsonValue path = obj.value("image");
                QJsonValue duration = obj.value("duration");
                QImage img(cranResourcePath(path.toString()));

                if (path.isNull() || duration.isNull() || img.isNull())
                {
                    return cranError(ERRARG_1(e_03, QString::number(currentFrame)));
                }

                frames.append(img);
                durations.append(duration.toDouble());
                currentFrame++;
            }

            return createInternal(frames, durations, renderTarget);
        }
    }

    return false;
}
