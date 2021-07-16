QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += TT_BUILDING_DLL

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    ../src/TabToolbar/Builder.cpp \
    ../src/TabToolbar/CompactToolButton.cpp \
    ../src/TabToolbar/Group.cpp \
    ../src/TabToolbar/Page.cpp \
    ../src/TabToolbar/StyleTools.cpp \
    ../src/TabToolbar/Styles.cpp \
    ../src/TabToolbar/SubGroup.cpp \
    ../src/TabToolbar/TabToolbar.cpp \
    ../src/TabToolbar/ToolButtonStyle.cpp

HEADERS += \
    ../include/TabToolbar/API.h \
    ../include/TabToolbar/Builder.h \
    ../include/TabToolbar/Group.h \
    ../include/TabToolbar/Page.h \
    ../include/TabToolbar/StyleTools.h \
    ../include/TabToolbar/Styles.h \
    ../include/TabToolbar/SubGroup.h \
    ../include/TabToolbar/TabToolbar.h \
    ../src/TabToolbar/CompactToolButton.h \
    ../src/TabToolbar/ToolButtonStyle.h

RESOURCES += \
    ../src/TabToolbar/StyleTemplate.qrc

include($$PWD/../common.pri)
