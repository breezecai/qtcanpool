/***************************************************************************
 **
 **  Copyright (C) 2018 MaMinJie <canpool@163.com>
 **  Contact: https://github.com/canpool
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see http://www.gnu.org/licenses/.
 **
****************************************************************************/
#include "fancywindow.h"
#include "fancybar.h"
#include "screenhelper.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QStatusBar>
#include <QSettings>
#include <QPainter>

class FancyWindowPrivate
{
public:
    FancyWindowPrivate();

    FancyBar *fancyBar;
};


FancyWindowPrivate::FancyWindowPrivate()
{
    fancyBar = nullptr;
}

FancyWindow::FancyWindow(QWidget *parent)
    : QMainWindow(parent), d(new FancyWindowPrivate())
{
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowSystemMenuHint
                   | Qt::WindowMinimizeButtonHint
                   | Qt::Window
                  );
    d->fancyBar = new FancyBar(this);
    connect(d->fancyBar, SIGNAL(maximizationChanged(bool)), this, SIGNAL(resizable(bool)));
    setMenuWidget(d->fancyBar);
    setMouseTracking(true);
    QRect geom = ScreenHelper::normalRect();
    resize(geom.width(), geom.height());
    raise();
    activateWindow();
}

FancyWindow::~FancyWindow()
{
    delete d;
}

FancyBar *FancyWindow::fancyBar() const
{
    return d->fancyBar;
}

void FancyWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QSettings settings("Canpool", "qtcanpool");
    QString skinName = settings.value("skin").toString();

    if (skinName.isEmpty()) {
        skinName = QString(":/skin/default");
    }

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.drawPixmap(QRect(0, 0, this->width(), this->height()),
                       QPixmap(skinName));
}

