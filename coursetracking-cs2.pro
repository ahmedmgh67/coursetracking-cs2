QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    course.cpp \
    grade.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp \
    scandre.cpp \
    student.cpp \
    viewcourses.cpp \
    viewregistrations.cpp \
    viewstudents.cpp

HEADERS += \
    course.h \
    grade.h \
    mainwindow.h \
    registerdialog.h \
    scandre.h \
    student.h \
    viewcourses.h \
    viewregistrations.h \
    viewstudents.h

FORMS += \
    mainwindow.ui \
    registerdialog.ui \
    scandre.ui \
    viewcourses.ui \
    viewregistrations.ui \
    viewstudents.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc
