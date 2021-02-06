QT       += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    renamewidget.cpp \
    sendwidget.cpp \
    smtp.cpp \
    util.cpp


HEADERS += \
    mainwindow.h \
    renamewidget.h \
    sendwidget.h \
    smtp.h \
    smtp/SmtpMime \
    util.h


FORMS += \
    mainwindow.ui \
    renamemain.ui \
    renamewidget.ui \
    sendwidget.ui

INCLUDEPATH += C:/Program Files/OpenSSL-Win64/include

INCLUDEPATH += C:/Program Files (x86)/OpenSSL-Win32/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
