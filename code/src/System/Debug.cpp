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
#include <Cranberry/System/Debug.hpp>

// Mode-dependent headers
#ifdef QT_DEBUG
    #include <QDebug>
#else
    #include <Cranberry/Game/Game.hpp>
    #include <QAbstractButton>
    #include <QMessageBox>
#endif


CRANBERRY_USING_NAMESPACE


bool priv::Debug::showError(
        const QString& msg,
        const uint line,
        const std::string file,
        const std::string func
        )
{
    QString fiStr = QString(file.c_str());
    QString fuStr = QString(func.c_str());
    QString liStr = QString::number(line);
    QString msgStr = QString(msg).replace("\n", "\n    ");

#ifdef QT_DEBUG
    qDebug() << "\n---";
    qDebug() << "/!\\ Cranberry threw an error /!\\";
    qDebug() << "in file" << fiStr;
    qDebug() << "in func" << fuStr;
    qDebug() << "in line" << liStr;
    qDebug().noquote() << "\n   " << msgStr;
    qDebug() << "---\n\n";
#else
    // Prepares the strings.
    QString strLine = QString::number(line);
    QString strFile(file.data());
    QString strFunc(func.data());
    QString strMsg(msg.data());
    QString strInfo("The Cranberry game engine reported an error "
                    "thrown in module \"%0\" within function \"%1\" "
                    "located in line %2. Please contact the author "
                    "of this game and forward the detailed message.");

    // Shows the message box with a 'detailed' message.
    QMessageBox box;
    box.setWindowTitle("Cranberry Error");
    box.setIcon(QMessageBox::Critical);
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Ignore);
    box.setText(strInfo.arg(strFile, strFunc, strLine));
    box.setDetailedText(strMsg);
    box.button(QMessageBox::Ignore)->setText("Terminate");
    box.exec();

    if (box.clickedButton()->text() == "Terminate")
    {
        Game::instance()->exit(CRANBERRY_EXIT_FATAL);
    }
#endif
    return false;
}


bool priv::Debug::showWarning(
        const QString& msg,
        const uint line,
        const std::string file,
        const std::string func)
{
    QString fiStr = QString(file.c_str());
    QString fuStr = QString(func.c_str());
    QString liStr = QString::number(line);
    QString msgStr = QString(msg).replace("\n", "\n    ");

#ifdef QT_DEBUG
    qDebug() << "\n---";
    qDebug() << "/!\\ Cranberry threw a warning /!\\";
    qDebug() << "in file" << fiStr;
    qDebug() << "in func" << fuStr;
    qDebug() << "in line" << liStr;
    qDebug().noquote() << "\n   " << msgStr;
    qDebug() << "---\n\n";
#else
    // Simply ignore warnings in release mode.
    Q_UNUSED(msg)
    Q_UNUSED(line)
    Q_UNUSED(file)
    Q_UNUSED(func)
#endif
    return false;
}


QString priv::StackWalker::stackTrace()
{
    return "No stack trace available for this platform.";
}
