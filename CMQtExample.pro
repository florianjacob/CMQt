# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = CMQtExample
QT += qml quick bluetooth

CONFIG += c++11

CONFIG += sailfishapp

SOURCES += src/main.cpp \
    src/ledmatrix.cpp \
    src/cmapplication.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    rpm/CMQtExample.changes.in \
    rpm/CMQtExample.spec \
    rpm/CMQtExample.yaml \
    translations/*.ts \
    CMQtExample.desktop \
    qml/pages/MainPage.qml \
    qml/main.qml

CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/CMQtExample-de.ts

HEADERS += \
    src/ledmatrix.h \
    src/cmapplication.h

