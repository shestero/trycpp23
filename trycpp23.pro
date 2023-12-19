QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# https://github.com/xpack-dev-tools/mingw-w64-gcc-xpack/releases/download/v13.2.0-1/xpack-mingw-w64-gcc-13.2.0-1-win32-x64.zip
QMAKE_CXX=\xpack-mingw-w64-gcc-13.2.0-1\bin\x86_64-w64-mingw32-g++.exe
QMAKE_CXXFLAGS += -std=c++23
CONFIG += -std=c++23

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    drawboard.cpp \
    filelist.cpp \
    filereader.cpp \
    main.cpp \
    mainwindow.cpp \
    ssdfile.cpp \
    ssdfileheader.cpp

HEADERS += \
    drawboard.h \
    error.h \
    filelist.h \
    filereader.h \
    mainwindow.h \
    ssdfile.h \
    ssdfileheader.h

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
