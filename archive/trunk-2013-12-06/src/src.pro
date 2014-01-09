include(identify.pri)

DEFINES += NO_CONTACTS_CAPTCHA

# For verbose debugging
DEFINES += DBG_TP_VERBOSE
#DEFINES += DBG_VERBOSE

QT      *= core gui sql xml xmlpatterns script declarative
TARGET   = qgvdial
TEMPLATE = app

!win32 {
# Win32 stores all intermidiate files into the debug and release folders.
# There is no need to create a separate moc and obj folder for Win32.
    MOC_DIR = moc
    OBJECTS_DIR = obj
}

CONFIG *= precompile_header
PRECOMPILED_HEADER = ../src/global.h

CONFIG   *= mobility
!blackberry {
    MOBILITY *= systeminfo
}

!blackberry {
QT *= phonon
} else {
MOBILITY *= multimedia
}

# Meego requires Meegotouch
contains(DEFINES,MEEGO_HARMATTAN) {
    CONFIG += meegotouch
}

# Qt-Components required for Harmattan, S3 and Belle.
symbian | contains(DEFINES,MEEGO_HARMATTAN) {
    CONFIG += qt-components
}

# Symbian and Harmattan need feedback
symbian | contains(DEFINES,MEEGO_HARMATTAN) {
    MOBILITY *= feedback
}

# Except for Symbian S3 and S3 Belle add single application
!contains(DEFINES,IS_S3) : !contains(DEFINES,IS_S3_BELLE) : !blackberry {
    message(Single application library added)
    include(../qgvdial/features/qtsingleapplication/qtsingleapplication.pri)
}

INCLUDEPATH += .

OTHER_FILES += gen/api_server.xml

maemo5 {
    message(Maemo5 your world)
    QT *= maemo5
    OTHER_FILES += ../build-files/qgvdial/maemo/qgvdial.desktop
}

# Harmattan has its own desktop file
contains(DEFINES,MEEGO_HARMATTAN) {
    OTHER_FILES += ../build-files/qgvdial/harmattan/qgvdial.desktop
}

# In Linux, maemo and harmattan, add the telepathy related sources and headers,
# telepathy libraries and openssl library.
unix:!symbian:!blackberry {
   include(../src/tp-capable/tp-capable.pri)
}
# For blackberry, add the openssl library:
blackberry {
    LIBS += -lcrypto
}

SOURCES  += ../src/main.cpp                 \
            ../src/MainApp.cpp              \
            ../src/MainWindow.cpp           \
            ../src/MainWinLogs.cpp          \
            ../src/MainWinCallText.cpp      \
            ../src/MainWinLogin.cpp         \
            ../src/Singletons.cpp           \
            ../src/CacheDatabase.cpp        \
            ../src/OsDependent.cpp          \
            ../src/ObserverFactory.cpp      \
            ../src/CalloutInitiator.cpp     \
            ../src/CallInitiatorFactory.cpp \
            ../src/InboxModel.cpp           \
            ../src/PhoneNumberValidator.cpp \
            ../src/GVContactsTable.cpp      \
            ../src/SMSEntryDeleteButton.cpp \
            ../src/ChildWindowBase.cpp      \
            ../src/GVInbox.cpp              \
            ../src/RegNumberModel.cpp       \
            ../src/ContactsModel.cpp        \
            ../src/ContactDetailsModel.cpp  \
            ../src/DialContext.cpp          \
            ../src/FuzzyTimer.cpp           \
            ../src/NwInfo.cpp               \
            ../src/SkypeClientFactory.cpp   \
            ../src/SkypeClient.cpp          \
            ../src/Vmail.cpp

HEADERS  += ../src/global.h                 \
            ../src/IObserver.h              \
            ../src/MainApp.h                \
            ../src/MainWindow.h             \
            ../src/Singletons.h             \
            ../src/CacheDatabase.h          \
            ../src/CacheDatabase_int.h      \
            ../src/OsDependent.h            \
            ../src/ObserverFactory.h        \
            ../src/CalloutInitiator.h       \
            ../src/CallInitiatorFactory.h   \
            ../src/InboxModel.h             \
            ../src/PhoneNumberValidator.h   \
            ../src/GVContactsTable.h        \
            ../src/SMSEntryDeleteButton.h   \
            ../src/ChildWindowBase.h        \
            ../src/GVInbox.h                \
            ../src/RegNumberModel.h         \
            ../src/ContactsModel.h          \
            ../src/ContactDetailsModel.h    \
            ../src/DialContext.h            \
            ../src/FuzzyTimer.h             \
            ../src/NwInfo.h                 \
            ../src/PhoneIntegrationIface.h  \
            ../src/TpHeaders.h              \
            ../src/SkypeClientFactory.h     \
            ../src/SkypeClient.h            \
            ../src/Vmail.h

include(../api/api.pri)

contains(DEFINES,MEEGO_HARMATTAN) | contains(DEFINES,IS_S3_BELLE) {
    DEFINES += ENABLE_FUZZY_TIMER
}

contains (DEFINES, ENABLE_FUZZY_TIMER) {
    SOURCES += ../src/FuzzyTimerMobility.cpp
    HEADERS += ../src/FuzzyTimerMobility.h
} else {
    SOURCES += ../src/FuzzyTimerDefault.cpp
    HEADERS += ../src/FuzzyTimerDefault.h
}

!contains (DEFINES, NO_CONTACTS_CAPTCHA) {
QT *= webkit
SOURCES +=  ../src/CaptchaWidget.cpp
HEADERS +=  ../src/CaptchaWidget.h
}

RESOURCES += ../src/qgvdial.qrc
contains(DEFINES,MEEGO_HARMATTAN) {
    RESOURCES += ../src/qgvdial-meego.qrc
} else {
contains(DEFINES, IS_S3) | contains(DEFINES, IS_S3_BELLE) {
    RESOURCES += ../src/qgvdial-s3.qrc
} else {
maemo5 {
    RESOURCES += ../src/qgvdial-maemo.qrc
} else {
unix : !symbian : !blackberry {
# Unix && !Symbian && !maemo5 && !harmattan = Linux desktop
    RESOURCES += ../src/qgvdial-desktop-linux.qrc
} else {
    RESOURCES += ../src/qgvdial-generic.qrc
} } } }


# This is so that QtCreator can show these files in the files list.
OTHER_FILES  += ../src/winrsrc.rc               \
                ../qml/About.qml                \
                ../qml/ActionButtons.qml        \
                ../qml/ContactDetails.qml       \
                ../qml/ContactsList.qml         \
                ../qml/DialoutSelection.qml     \
                ../qml/DialSettings.qml         \
                ../qml/DigitButton.qml          \
                ../qml/ExpandView.qml           \
                ../qml/Fader.qml                \
                ../qml/helper.js                \
                ../qml/HMain.qml                \
                ../qml/InboxList.qml            \
                ../qml/Keypad.qml               \
                ../qml/ListRefreshComponent.qml \
                ../qml/LoginDetails.qml         \
                ../qml/LoginPage.qml            \
                ../qml/LogView.qml              \
                ../qml/Main.qml                 \
                ../qml/MainView.qml             \
                ../qml/MsgBox.qml               \
                ../qml/MyButton.qml             \
                ../qml/PinSetting.qml           \
                ../qml/Proxy.qml                \
                ../qml/RefreshSettings.qml      \
                ../qml/S3Main.qml               \
                ../qml/SaveCancel.qml           \
                ../qml/Scrollbar.qml            \
                ../qml/Settings.qml             \
                ../qml/SmsView.qml              \
                ../qml/Tab.qml                  \
                ../qml/TabbedUI.qml             \
                ../qml/generic/QGVButton.qml            \
                ../qml/generic/QGVLabel.qml             \
                ../qml/generic/QGVRadioButton.qml       \
                ../qml/generic/QGVTextEdit.qml          \
                ../qml/generic/QGVTextInput.qml         \
                ../qml/generic/PhoneIntegrationView.qml \
                ../qml/meego/QGVButton.qml              \
                ../qml/meego/QGVLabel.qml               \
                ../qml/meego/QGVRadioButton.qml         \
                ../qml/meego/QGVTextEdit.qml            \
                ../qml/meego/QGVTextInput.qml           \
                ../qml/meego/PhoneIntegrationView.qml   \
                ../qml/s3/QGVButton.qml         \
                ../qml/s3/QGVRadioButton.qml    \
                ../qml/s3/QGVTextEdit.qml       \
                ../qml/s3/QGVTextInput.qml      \
                readme.txt

# In desktop Linux, add the Skype client
contains(DEFINES,IS_LINUX_DESKTOP) {
   include(../src/desktop/desktop.pri)
}
win32 {
   include(../src/desktop/desktop.pri)
}

############################## Mosquitto ##############################
# Add mosquitto support sources to EVERYONE.
    HEADERS  += ../src/MqClientThread.h
    SOURCES  += ../src/MqClientThread.cpp

exists(../src/mqlib-build) {
    message(Forcible inclusion of mqlib!)
    include(../src/mqlib/mqlib.pri)
} else {
    message(Forcible exclusion of mqlib!)
    LIBS += -lmosquitto
}
#######################################################################

symbian {
    include(symbian/symbian.pri)
}

###############################################################
# Installation related line go here
###############################################################

# Installation for Maemo and Harmattan
maemo5 {
    message(maemo5 install)
    exists(../../buildit.sh) || exists(../../buildit.pl) || exists(.svn) {
        PREFIX = ../debian/qgvdial/usr
        message(Built using my scripts... probably inside scratchbox)
    } else {
        PREFIX = ../maemo/debian/qgvdial/usr
        message(Build using qtcreator)
    }

    OPTPREFIX  = $$PREFIX/../opt
    DATADIR    = $$PREFIX/share
}

contains(DEFINES,MEEGO_HARMATTAN) {
    message(Harmattan install)

    OPTPREFIX  = /opt
    DATADIR    = /usr/share
}

maemo5|contains(DEFINES,MEEGO_HARMATTAN) {
    INSTALLS += target desktop icon icon48 icon64 icon_scalable dbusservice

    target.path = $$OPTPREFIX/qgvdial/bin

    desktop.path  = $$DATADIR/applications/hildon
    desktop.files = ../build-files/qgvdial/maemo/qgvdial.desktop

    icon.path = $$DATADIR/qgvdial/icons
    icon.files += qgvdial.png

    icon48.path = $$DATADIR/icons/hicolor/48x48/hildon
    icon48.files = ../icons/48/qgvdial.png

    icon64.path = $$DATADIR/icons/hicolor/64x64/hildon
    icon64.files = ../icons/64/qgvdial.png

    icon_scalable.path = $$DATADIR/icons/hicolor/scalable/hildon
    icon_scalable.files = qgvdial.png

    dbusservice.path = $$DATADIR/dbus-1/services
    dbusservice.files = ../build-files/qgvdial/maemo/qgvdial.Call.service \
                        ../build-files/qgvdial/maemo/qgvdial.Text.service \
                        ../build-files/qgvdial/maemo/qgvdial.UI.service
}

# Harmattan specific install section
contains(DEFINES,MEEGO_HARMATTAN) {
    INSTALLS += icon80

    desktop.path  = $$DATADIR/applications
    desktop.files = ../build-files/qgvdial/harmattan/qgvdial.desktop

    icon48.path = $$DATADIR/icons/hicolor/48x48/apps

    icon64.path = $$DATADIR/icons/hicolor/64x64/apps

    icon80.path = $$DATADIR/icons/hicolor/80x80/apps
    icon80.files = ../icons/80/qgvdial.png

    icon_scalable.path = $$DATADIR/icons/hicolor/scalable/apps
    icon_scalable.files = qgvdial.png

    dbusservice.files = ../build-files/qgvdial/harmattan/qgvdial.Call.service \
                        ../build-files/qgvdial/harmattan/qgvdial.Text.service \
                        ../build-files/qgvdial/harmattan/qgvdial.UI.service
}

# For blackberry:
OTHER_FILES += ../build-files/qgvdial/bb10/bar-descriptor.xml