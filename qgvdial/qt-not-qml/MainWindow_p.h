/*
qgvdial is a cross platform Google Voice Dialer
Copyright (C) 2009-2014  Yuvraaj Kelkar

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Contact: yuvraaj@gmail.com
*/

#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include "global.h"

namespace Ui {
    class MainWindow;
}

class MainWindowPrivate : public QMainWindow
{
    Q_OBJECT

public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindowPrivate(QWidget *parent = NULL);
    virtual ~MainWindowPrivate();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void setAllowClose(bool allow);

public slots:
    void showExpanded();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    bool m_allowClose;

    friend class MainWindow;
};


#endif // MAINWINDOW_P_H
