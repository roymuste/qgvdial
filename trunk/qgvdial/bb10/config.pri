# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/BB10PhoneFactory.cpp) \
                 $$quote($$BASEDIR/src/BBPhoneAccount.cpp) \
                 $$quote($$BASEDIR/src/MainWindow.cpp) \
                 $$quote($$BASEDIR/src/OsDependant.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/BB10PhoneFactory.h) \
                 $$quote($$BASEDIR/src/BBPhoneAccount.h) \
                 $$quote($$BASEDIR/src/MainWindow.h) \
                 $$quote($$BASEDIR/src/OsDependant.h) \
                 $$quote($$BASEDIR/src/platform_specific.h)
    }

    CONFIG(release, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/BB10PhoneFactory.cpp) \
                 $$quote($$BASEDIR/src/BBPhoneAccount.cpp) \
                 $$quote($$BASEDIR/src/MainWindow.cpp) \
                 $$quote($$BASEDIR/src/OsDependant.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/BB10PhoneFactory.h) \
                 $$quote($$BASEDIR/src/BBPhoneAccount.h) \
                 $$quote($$BASEDIR/src/MainWindow.h) \
                 $$quote($$BASEDIR/src/OsDependant.h) \
                 $$quote($$BASEDIR/src/platform_specific.h)
    }
}

simulator {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/BB10PhoneFactory.cpp) \
                 $$quote($$BASEDIR/src/BBPhoneAccount.cpp) \
                 $$quote($$BASEDIR/src/MainWindow.cpp) \
                 $$quote($$BASEDIR/src/OsDependant.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/BB10PhoneFactory.h) \
                 $$quote($$BASEDIR/src/BBPhoneAccount.h) \
                 $$quote($$BASEDIR/src/MainWindow.h) \
                 $$quote($$BASEDIR/src/OsDependant.h) \
                 $$quote($$BASEDIR/src/platform_specific.h)
    }
}

INCLUDEPATH +=  $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs) \
             $$quote($$BASEDIR/../assets/icons/*.qml) \
             $$quote($$BASEDIR/../assets/icons/*.js) \
             $$quote($$BASEDIR/../assets/icons/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/01/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/01/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/01/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/0d/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/0d/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/0d/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/0e/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/0e/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/0e/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/10/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/10/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/10/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/1d/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/1d/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/1d/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/24/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/24/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/24/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/37/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/37/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/37/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/46/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/46/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/46/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/55/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/55/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/55/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/56/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/56/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/56/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/5c/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/5c/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/5c/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/64/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/64/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/64/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/72/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/72/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/72/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/75/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/75/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/75/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/76/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/76/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/76/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/86/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/86/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/86/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/91/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/91/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/91/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/9f/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/9f/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/9f/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/b4/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/b4/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/b4/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/b5/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/b5/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/b5/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c0/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c0/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c0/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c1/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c1/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c1/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c3/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c3/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c3/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c5/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c5/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c5/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c8/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c8/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/c8/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/d7/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/d7/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/d7/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/d8/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/d8/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/d8/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/e2/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/e2/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/e2/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/e7/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/e7/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/e7/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/ea/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/ea/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/ea/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/eb/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/eb/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/eb/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/f2/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/f2/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/f2/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/fd/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/fd/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/pristine/fd/*.qs) \
             $$quote($$BASEDIR/../assets/icons/.svn/tmp/*.qml) \
             $$quote($$BASEDIR/../assets/icons/.svn/tmp/*.js) \
             $$quote($$BASEDIR/../assets/icons/.svn/tmp/*.qs) \
             $$quote($$BASEDIR/../assets/icons/114/*.qml) \
             $$quote($$BASEDIR/../assets/icons/114/*.js) \
             $$quote($$BASEDIR/../assets/icons/114/*.qs) \
             $$quote($$BASEDIR/../assets/icons/48/*.qml) \
             $$quote($$BASEDIR/../assets/icons/48/*.js) \
             $$quote($$BASEDIR/../assets/icons/48/*.qs) \
             $$quote($$BASEDIR/../assets/icons/64/*.qml) \
             $$quote($$BASEDIR/../assets/icons/64/*.js) \
             $$quote($$BASEDIR/../assets/icons/64/*.qs) \
             $$quote($$BASEDIR/../assets/icons/80/*.qml) \
             $$quote($$BASEDIR/../assets/icons/80/*.js) \
             $$quote($$BASEDIR/../assets/icons/80/*.qs) \
             $$quote($$BASEDIR/../assets/icons/saved/*.qml) \
             $$quote($$BASEDIR/../assets/icons/saved/*.js) \
             $$quote($$BASEDIR/../assets/icons/saved/*.qs) \
             $$quote($$BASEDIR/../assets/images/*.qml) \
             $$quote($$BASEDIR/../assets/images/*.js) \
             $$quote($$BASEDIR/../assets/images/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}.ts)