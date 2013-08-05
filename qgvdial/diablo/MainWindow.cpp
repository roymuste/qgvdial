/*
qgvdial is a cross platform Google Voice Dialer
Copyright (C) 2009-2013  Yuvraaj Kelkar

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

#include "MainWindow.h"
#include "MainWindow_p.h"
#include "ui_mainwindow.h"

QCoreApplication *
createApplication(int argc, char *argv[])
{
    return new QApplication(argc, argv);
}

MainWindow::MainWindow(QObject *parent)
: IMainWindow(parent)
, d(new MainWindowPrivate)
{
}//MainWindow::MainWindow

MainWindow::~MainWindow()
{
    delete d;
}//MainWindow::~MainWindow

void
MainWindow::init()
{
    IMainWindow::init ();

    d->setOrientation(MainWindowPrivate::ScreenOrientationAuto);
    d->showExpanded();
    connect(d->ui->loginButton, SIGNAL(clicked()),
            this, SLOT(onLoginClicked()));

    QTimer::singleShot (1, this, SLOT(onInitDone()));
}//MainWindow::init

void
MainWindow::uiRequestLoginDetails()
{
    QMessageBox msg;
    msg.setText ("Please enter a user and password");
    msg.exec ();

    d->ui->tabWidget->setCurrentIndex (3);
    d->ui->toolBox->setCurrentIndex (0);
}//MainWindow::uiRequestLoginDetails

void
MainWindow::onLoginClicked()
{
    QString user, pass;
    user = d->ui->textUsername->text ();
    pass = d->ui->textPassword->text ();

    if (user.isEmpty () || pass.isEmpty ()) {
        Q_WARN("Invalid username or password");
        QMessageBox msg;
        msg.setText (tr("Invalid username or password"));
        msg.exec ();
        return;
    }

    beginLogin (user, pass);
}//MainWindow::onLoginClicked

void
MainWindow::uiRequestTFALoginDetails(void *ctx)
{
    QString strPin = QInputDialog::getText(d, tr("Enter PIN"),
                                           tr("Two factor authentication"));

    int pin = strPin.toInt ();
    if (pin == 0) {
        resumeTFAAuth (ctx, pin, true);
    } else {
        resumeTFAAuth (ctx, pin, false);
    }
}//MainWindow::uiRequestTFALoginDetails

void
MainWindow::uiSetUserPass(const QString &user, const QString &pass,
                          bool editable)
{
    d->ui->textUsername->setText (user);
    d->ui->textPassword->setText (pass);

    d->ui->textUsername->setReadOnly (!editable);
    d->ui->textPassword->setReadOnly (!editable);
}//MainWindow::uiSetUserPass