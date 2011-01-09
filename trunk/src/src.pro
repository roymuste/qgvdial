QT      *= core gui webkit sql xml xmlpatterns script declarative
TARGET   = qgvdial
TEMPLATE = app

CONFIG  += mobility
MOBILITY *= multimedia

include(qtsingleapplication/qtsingleapplication.pri)

win32 {
CONFIG *= embed_manifest_exe
}

maemo5 {
    message(Maemo5 your world)
    QT *= maemo5
    OTHER_FILES += ../build-files/qgvdial.desktop
    MOBILITY += bearer
}

# In Linux and maemo, add the telepathy libraries, sources and headers
unix:!symbian {
    QT *= dbus
    INCLUDEPATH += $$QMAKESPEC/usr/include/telepathy-1.0/
    LIBS += -ltelepathy-qt4
}

SOURCES  += main.cpp                    \
            global.cpp                  \
            MainApp.cpp                 \
            MainWindow.cpp              \
            Singletons.cpp              \
            CacheDatabase.cpp           \
            OsDependent.cpp             \
            SkypeClientFactory.cpp      \
            SkypeClient.cpp             \
            ObserverFactory.cpp         \
            GVWebPage.cpp               \
            GVDataAccess.cpp            \
            GVAccess.cpp                \
            CalloutInitiator.cpp        \
            CallInitiatorFactory.cpp    \
            MobileWebPage.cpp           \
            InboxModel.cpp              \
            GVH_XMLJsonHandler.cpp      \
            PhoneNumberValidator.cpp    \
            GVContactsTable.cpp         \
            CaptchaWidget.cpp           \
            ContactsXmlHandler.cpp      \
            DialCancelDlg.cpp           \
            DlgSelectContactNumber.cpp  \
            SMSEntryDeleteButton.cpp    \
            SMSDlg.cpp                  \
            ChildWindowBase.cpp         \
            GVHistory.cpp               \
            VMailDialog.cpp             \
            WebWidget.cpp               \
            RegNumberModel.cpp          \
            ContactsModel.cpp           \
            ContactDetailsModel.cpp     \
            GVH_SMS_Handler.cpp

HEADERS  += global.h                    \
            IObserver.h                 \
            MainApp.h                   \
            MainWindow.h                \
            Singletons.h                \
            CacheDatabase.h             \
            OsDependent.h               \
            SkypeClientFactory.h        \
            SkypeClient.h               \
            ObserverFactory.h           \
            GVWebPage.h                 \
            GVDataAccess.h              \
            GVAccess.h                  \
            CalloutInitiator.h          \
            CallInitiatorFactory.h      \
            MobileWebPage.h             \
            InboxModel.h                \
            GVH_XMLJsonHandler.h        \
            PhoneNumberValidator.h      \
            GVContactsTable.h           \
            CaptchaWidget.h             \
            ContactsXmlHandler.h        \
            DialCancelDlg.h             \
            DlgSelectContactNumber.h    \
            SMSEntryDeleteButton.h      \
            SMSDlg.h                    \
            ChildWindowBase.h           \
            GVHistory.h                 \
            VMailDialog.h               \
            WebWidget.h                 \
            RegNumberModel.h            \
            ContactsModel.h             \
            ContactDetailsModel.h       \
            GVH_SMS_Handler.h

FORMS    += VMailDialog.ui              \
            WebWidget.ui

RESOURCES += qgvdial.qrc

# This is so that QtCreator can show these files in the files list.
OTHER_FILES  += winrsrc.rc                  \
                ../qml/ActionButtons.qml    \
                ../qml/ComboBoxPhones.qml   \
                ../qml/ContactDetails.qml   \
                ../qml/ContactsList.qml     \
                ../qml/DialDisp.qml         \
                ../qml/DigitButton.qml      \
                ../qml/helper.js            \
                ../qml/InboxList.qml        \
                ../qml/Keypad.qml           \
                ../qml/MainButtons.qml      \
                ../qml/Main.qml             \
                ../qml/MainView_l.qml       \
                ../qml/MainView_p.qml       \
                ../qml/MainView.qml         \
                ../qml/MyButton.qml         \
                ../qml/Proxy.qml            \
                ../qml/RadioButton.qml      \
                ../qml/Settings.qml         \
                ../qml/TextButton.qml       \
                readme.txt

# In Linux and maemo, add the telepathy related sources and headers
unix:!symbian {
    HEADERS  += TpObserver.h            \
                TpCalloutInitiator.h    \
                QGVDbusServer.h
    SOURCES  += TpObserver.cpp          \
                TpCalloutInitiator.cpp  \
                QGVDbusServer.cpp
}

# In desktop Linux, add the Skype client
unix:!symbian:!maemo5 {
    HEADERS  += SkypeLinuxClient.h              \
                SkypeObserver.h                 \
                DesktopSkypeCallInitiator.h
    SOURCES  += SkypeLinuxClient.cpp            \
                SkypeObserver.cpp               \
                DesktopSkypeCallInitiator.cpp
}

win32 {
# Resource file is for windows only - for the icon
    RC_FILE = winrsrc.rc

# In desktop Windows, add the Skype client
    HEADERS += SkypeWinClient.h             \
               SkypeObserver.h              \
               DesktopSkypeCallInitiator.h
    SOURCES += SkypeWinClient.cpp           \
               SkypeObserver.cpp            \
               DesktopSkypeCallInitiator.cpp
}

symbian {
    TARGET.UID3 = 0xe2d130b9
    # TARGET.CAPABILITY +=
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

###############################################################
# Installation related line go here
###############################################################

# Installation for maemo
maemo5 {
    exists (../../buildit.pl) {
        PREFIX = ../debian/qgvdial/usr
        message(Built using my scripts)
    }
    !exists (../../buildit.pl) {
        PREFIX = ../maemo/debian/qgvdial/usr
        message(Build using qtcreator)
    }

    message(maemo5 install)
    OPTPREFIX  = $$PREFIX/../opt/qgvdial
    BINDIR     = $$OPTPREFIX/bin
    DATADIR    = $$PREFIX/share
    OPTDATADIR = $$OPTPREFIX/share

    DEFINES += DATADIR=\"$$DATADIR\" PKGDATADIR=\"$$PKGDATADIR\"

    INSTALLS += target desktop icon qss dbusservice

    target.path =$$BINDIR

    desktop.path = $$DATADIR/applications/hildon
    desktop.files += ../build-files/qgvdial.desktop

    icon.path = $$OPTDATADIR
    icon.files += qgvdial.png

    qss.path = $$BINDIR/stylesheets
    qss.files += ./stylesheets/dialpad_maemo.qss

    dbusservice.path = $$DATADIR/dbus-1/services
    dbusservice.files += ../build-files/qgvdial.Call.service
    dbusservice.files += ../build-files/qgvdial.Text.service
}

# Installation for Linux
unix:!symbian:!maemo5 {
    BINDIR  = $$PREFIX/bin
    DATADIR = $$PREFIX/share
    message($$BINDIR)

    DEFINES += DATADIR=\"$$DATADIR\" PKGDATADIR=\"$$PKGDATADIR\"

    INSTALLS += target icon qss dbusservice

    target.path =$$BINDIR

    icon.path = $$DATADIR/qgvdial
    icon.files += qgvdial.png

    qss.path = $$BINDIR/stylesheets
    qss.files += ./stylesheets/dialpad_maemo.qss

    dbusservice.path = $$DATADIR/dbus-1/services
    dbusservice.files += ../build-files/qgvdial.Call.service
    dbusservice.files += ../build-files/qgvdial.Text.service
}
