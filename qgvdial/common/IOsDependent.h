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

#ifndef IOSDEPENDENT_H
#define IOSDEPENDENT_H

#include <QObject>
#include "global.h"

class IOsDependant : public QObject
{
    Q_OBJECT
public:
    explicit IOsDependant(QObject *parent = 0) : QObject(parent) {}
    virtual ~IOsDependant() {}

    virtual QString getTempDir() = 0;
    virtual QString getDbDir() = 0;
    virtual QString getLogsDir() = 0;
    virtual QString getOsDetails() = 0;
    virtual QString getVmailDir() = 0;

    virtual bool cipher(const QByteArray &byIn, QByteArray &byOut, bool bEncrypt) = 0;
};

IOsDependant *createOSD(QObject *parent = NULL);

#endif // IOSDEPENDENT_H
