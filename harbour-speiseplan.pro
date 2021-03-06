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
TARGET = harbour-speiseplan

QT += core network

CONFIG += sailfishapp debug

SOURCES += src/harbour-speiseplan.cpp \
    src/parserbase.cpp

OTHER_FILES += qml/harbour-speiseplan.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FoodList.qml \
    qml/pages/SecondPage.qml \
    rpm/harbour-speiseplan.changes.in \
    rpm/harbour-speiseplan.spec \
    rpm/harbour-speiseplan.yaml \
    translations/*.ts \
    harbour-speiseplan.desktop

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-speiseplan-de.ts

HEADERS += \
    src/parserbase.h

