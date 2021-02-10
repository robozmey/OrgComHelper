QT       += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console
CONFIG += static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SMTP_LIBRARY_LOCATION = 3rdparty/SMTPEmail

DEPENDPATH += $$SMTP_LIBRARY_LOCATION

INCLUDEPATH += $$SMTP_LIBRARY_LOCATION/include
    #C:/Program Files/OpenSSL-Win64/include \
    #C:/Program Files (x86)/OpenSSL-Win32/include \
    #../SmtpClient-for-Qt-1.1/src


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    renamewidget.cpp \
    sendwidget.cpp \
    util.cpp


HEADERS += \
    mainwindow.h \
    renamewidget.h \
    sendwidget.h \
    util.h


FORMS += \
    mainwindow.ui \
    renamewidget.ui \
    sendwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#win32:CONFIG(release, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/release/ -lSMTPEmail
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/debug/ -lSMTPEmail
#else:unix: LIBS += -L$$SMTP_LIBRARY_LOCATION -lSMTPEmail


RC_ICONS = myappico.ico

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../SmtpClient-for-Qt-1.1/build/SMTPEmail-Desktop-Debug/release/ -lSMTPEmail
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../SmtpClient-for-Qt-1.1/build/SMTPEmail-Desktop-Debug/debug/ -lSMTPEmail
#else:unix: LIBS += -L$$PWD/../SmtpClient-for-Qt-1.1/build/SMTPEmail-Desktop-Debug/ -lSMTPEmail



win32: LIBS += -L$$PWD/../SmtpClient-for-Qt-1.1/build/SMTPEmail-Desktop-Debug/release/ -lSMTPEmail

INCLUDEPATH += $$PWD/../SmtpClient-for-Qt-1.1/src
DEPENDPATH += $$PWD/../SmtpClient-for-Qt-1.1/src

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../SmtpClient-for-Qt-1.1/build/SMTPEmail-Desktop-Debug/release/SMTPEmail.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../SmtpClient-for-Qt-1.1/build/SMTPEmail-Desktop-Debug/release/libSMTPEmail.a

#win32: LIBS += -llibssl
