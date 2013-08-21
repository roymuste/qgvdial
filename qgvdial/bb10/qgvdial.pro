APP_NAME = qgvdial

CONFIG += qt warn_on cascades10

include(config.pri)
include(api/api.pri)
include(common/common.pri)
include(features/openssl/openssl.pri)
include(features/bb10-dirs/bb10-dirs.pri)
include(thirdparty/kdab/kdab.pri)

device {
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
    }
}

simulator {
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
    }
}