/*
   Copyright (C) 2020 David Edmundson <davidedmundson@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the Lesser GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the Lesser GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "systemclipboard.h"

#include "waylandclipboard.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QGuiApplication>

SystemClipboard *SystemClipboard::instance()
{
    if (!qApp || qApp->closingDown()) {
        return nullptr;
    }
    static WaylandClipboard *systemClipboard = nullptr;
    if (!systemClipboard) {
        qInfo() << "Using Wayland clipboard access";
        systemClipboard = new WaylandClipboard(qApp);

        QElapsedTimer timer;
        timer.start();
        while ( !systemClipboard->isActive() && timer.elapsed() < 5000 ) {
            QCoreApplication::processEvents();
        }
        if ( timer.elapsed() > 100 ) {
            qWarning() << "Activating Wayland clipboard took" << timer.elapsed() << "ms";
        }
        if ( !systemClipboard->isActive() ) {
            qCritical() << "Failed to activate Wayland clipboard";
        }
    }
    return systemClipboard;
}

SystemClipboard::SystemClipboard(QObject *parent)
    : QObject(parent)
{
}

