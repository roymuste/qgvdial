#ifndef __MAINAPP_H__
#define __MAINAPP_H__

#include "qtsingleapplication/inc/QtSingleApplication"

#ifdef Q_WS_WIN32
#include <windows.h>
#endif

// For some reason the symbian MOC doesn't like it if I don't include QObject
// even though it is present in QtCore which is included in global.h
#include <QObject>

class MainApp : public QtSingleApplication
{
    Q_OBJECT

public:
    MainApp (int &argc, char **argv);

#ifdef Q_WS_WIN32
signals:
    void skypeAttachStatus (bool bOk);
    void skypeNotify (const QString &strData);

public:
    UINT getDiscover ();
    UINT getAttach ();
    HWND getSkypeHandle ();
    void clearSkypeHandle ();

protected:
    bool winEventFilter ( MSG * msg, long * result );

    UINT uidDiscover, uidAttach;
    HWND hSkypeWindow;
#endif

};

#endif //__MAINAPP_H__